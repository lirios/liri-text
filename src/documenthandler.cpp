/*
 * Copyright © 2016-2017 Andrew Penkrat
 *
 * This file is part of Liri Text.
 *
 * Liri Text is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Liri Text is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Liri Text.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "documenthandler.h"

#include <QTextDocument>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QTextDocumentFragment>
#include <QDebug>
#include "languageloader.h"
#include "languagemanager.h"

DocumentHandler::DocumentHandler(QObject *parent)
    : QObject(parent)
    , m_target(0)
    , m_document(0)
    , m_highlighter(0)
{

#ifndef QT_NO_FILESYSTEMWATCHER
    m_watcher = new QFileSystemWatcher(this);
    connect(m_watcher, &QFileSystemWatcher::fileChanged, this, &DocumentHandler::fileChanged);
#else
    qWarning() << "Document change notification is not available on this platform";
#endif

    m_defStyles = QSharedPointer<LanguageDefaultStyles>::create();
}

DocumentHandler::~DocumentHandler()
{
#ifndef QT_NO_FILESYSTEMWATCHER
    delete m_watcher;
#endif
    delete m_highlighter;
}

void DocumentHandler::setTarget(QQuickItem *target)
{
    m_document = nullptr;
    m_target = target;
    if (!m_target)
        return;

    QVariant doc = m_target->property("textDocument");
    if (doc.canConvert<QQuickTextDocument *>()) {
        QQuickTextDocument *qqdoc = doc.value<QQuickTextDocument *>();
        if (qqdoc) {
            m_document = qqdoc->textDocument();
            connect(m_document, &QTextDocument::modificationChanged, this,
                    &DocumentHandler::modifiedChanged);
            if (m_highlighter != nullptr)
                delete m_highlighter;
            m_highlighter = new LiriSyntaxHighlighter(m_document);
            m_highlighter->setDefaultStyles(m_defStyles);
        }
    }
    emit targetChanged();
}

bool DocumentHandler::setFileUrl(const QUrl &fileUrl)
{
    if (fileUrl != m_fileUrl) {
#ifndef QT_NO_FILESYSTEMWATCHER
        if (m_watcher->files().contains(m_fileUrl.toLocalFile()))
            m_watcher->removePath(m_fileUrl.toLocalFile());
        m_watcher->addPath(fileUrl.toLocalFile());
#endif
        m_fileUrl = fileUrl;
        QString filename = m_fileUrl.toLocalFile();
        qDebug() << m_fileUrl << filename;
        QFile file(filename);
        if (!file.open(QFile::ReadOnly)) {
            emit error(file.errorString());
            return false;
        }
        QByteArray data = file.readAll();
        if (file.error() != QFileDevice::NoError) {
            emit error(file.errorString());
            file.close();
            return false;
        }
        QTextCodec *codec = QTextCodec::codecForUtfText(data, QTextCodec::codecForLocale());
        setText(codec->toUnicode(data));
        if (m_document) {
            m_document->setModified(false);

            // Enable syntax highlighting
            QMimeDatabase db;
            QMimeType mimeType = db.mimeTypeForFileNameAndData(m_fileUrl.toString(), data);
            LanguageLoader ll(m_defStyles);
            auto language = ll.loadMainContextByMimeType(mimeType, m_fileUrl.fileName());
            m_highlighter->setLanguage(language, ll.styleMap());
        }
        if (m_fileUrl.isEmpty())
            m_documentTitle = QStringLiteral("New Document");
        else
            m_documentTitle = QFileInfo(filename).fileName();

        emit documentTitleChanged();
        emit fileUrlChanged();
    }
    return true;
}

void DocumentHandler::setDocumentTitle(const QString &title)
{
    if (title != m_documentTitle) {
        m_documentTitle = title;
        emit documentTitleChanged();
    }
}

QString DocumentHandler::textFragment(int position, int blockCount)
{
    if (m_highlighter) {
        return m_highlighter->highlightedFragment(position, blockCount, m_document->defaultFont());
    } else {
        QTextCursor cursor(m_document->findBlock(position));
        int blockNumber = cursor.blockNumber();
        for (int i = 1; i < blockCount - std::min(blockNumber, blockCount / 2); ++i)
            cursor.movePosition(QTextCursor::Down, QTextCursor::MoveAnchor);
        cursor.movePosition(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
        for (int i = 1; i < blockCount; ++i)
            cursor.movePosition(QTextCursor::Up, QTextCursor::KeepAnchor);
        cursor.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);

        QTextDocumentFragment fragment = cursor.selection();
        return fragment.toHtml();
    }
}

void DocumentHandler::setText(const QString &text)
{
    if (text != m_text) {
        m_text = text;
        emit textChanged();
    }
}

bool DocumentHandler::saveAs(const QUrl &filename)
{
#ifndef QT_NO_FILESYSTEMWATCHER
    // Stop monitoring file while saving
    if (m_watcher->files().contains(m_fileUrl.toLocalFile()))
        m_watcher->removePath(m_fileUrl.toLocalFile());
#endif

    bool success = true;
    QString localPath = filename.toLocalFile();
    QFile file(localPath);
    if (!file.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
        emit error(file.errorString());
        success = false;
    } else {
        if (file.write(m_document->toPlainText().toLocal8Bit()) == -1) {
            emit error(file.errorString());
            success = false;
        }
        file.close();
        qDebug() << "saved to" << localPath;
        setFileUrl(QUrl::fromLocalFile(localPath));

        m_document->setModified(false);
    }

#ifndef QT_NO_FILESYSTEMWATCHER
    // Restart file watcher back after saving completes
    if (!m_watcher->files().contains(m_fileUrl.toLocalFile()))
        m_watcher->addPath(m_fileUrl.toLocalFile());
#endif

    return success;
}

bool DocumentHandler::reloadText()
{
    QString filename = m_fileUrl.toLocalFile();
    QFile file(filename);
    if (!file.open(QFile::ReadOnly)) {
        emit error(file.errorString());
        return false;
    }
    QByteArray data = file.readAll();
    QTextCodec *codec = QTextCodec::codecForUtfText(data, QTextCodec::codecForLocale());
    setText(codec->toUnicode(data));
    if (file.error() == QFileDevice::NoError) {
        file.close();
        return true;
    } else {
        emit error(file.errorString());
        file.close();
        return false;
    }
}

void DocumentHandler::fileChanged(const QString &file)
{
    emit fileChangedOnDisk();
#ifndef QT_NO_FILESYSTEMWATCHER
    if (!m_watcher->files().contains(file))
        m_watcher->addPath(file);
#endif
}
