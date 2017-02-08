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

#ifndef LIRISYNTAXHIGHLIGHTER_H
#define LIRISYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QRegularExpressionMatch>
#include "languagecontextcontainer.h"
#include "languagecontextsimple.h"
#include "highlightdata.h"
#include "languagedefaultstyles.h"

class LiriSyntaxHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT
public:
    LiriSyntaxHighlighter(QTextDocument *parent);
    ~LiriSyntaxHighlighter();
    void setLanguage(QSharedPointer<LanguageContextReference> lang, const QHash<QString, QString> &styleMap);
    void setDefaultStyles(QSharedPointer<LanguageDefaultStyles> defStyles);

    QString highlightedFragment(int position, int blockCount, QFont font);

protected:
    struct Match {
        QRegularExpressionMatch match;
        QSharedPointer<LanguageContextReference> contextRef;

        inline bool operator <(const Match &other);
    };

    void highlightBlock(const QString &text);

    /*
     * This function is responsible for setting text format inside container and for maintaining stateData
     * It calls highlightPart to highlight container internals
     * Returns match for container end or an empty match if it wasn't found
     * offset is reset to the end of highlighted region
     */
    QRegularExpressionMatch highlightTillContainerEnd(const QString &text, int &offset, HighlightData::ContainerInfo containerInfo,
                                                      HighlightData *stateData, int startLength = 0);

    /*
     * Highlights container internals and finds it's end. Doesn't change stateData.
     * Returns match for container end or an empty match if it wasn't found
     * offset is reset to the end of highlighted region
     */
    QRegularExpressionMatch highlightPart(const QString &text, int &offset,
                                          HighlightData::ContainerInfo &currentContainerInfo, HighlightData *stateData);

    Match findMatch(const QString &text, int offset, int potentialEnd,
                    QSharedPointer<LanguageContextReference> contextRef,
                    HighlightData::ContainerInfo &currentContainerInfo, bool rootContext = true);

    QSharedPointer<LanguageContextReference> m_lang;
    QSharedPointer<LanguageDefaultStyles> m_defStyles;
    QHash<QString, QString> m_styleMap;
};

#endif // LIRISYNTAXHIGHLIGHTER_H
