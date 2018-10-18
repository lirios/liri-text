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

#ifndef HIGHLIGHTDATA_H
#define HIGHLIGHTDATA_H

#include <QTextBlockUserData>
#include <QHash>
#include <QRegularExpressionMatch>
#include "languagecontext.h"

class HighlightData : public QTextBlockUserData
{
    Q_DISABLE_COPY(HighlightData)
public:
    HighlightData();
    ~HighlightData();

    struct ContainerInfo
    {
        QSharedPointer<LanguageContext> containerRef;
        QRegularExpression endRegex;
        QList<QSharedPointer<LanguageContext>> forbiddenContexts;
    };

    QList<ContainerInfo> containers;
};

uint qHash(const HighlightData::ContainerInfo &t, uint seed = 0);

#endif // HIGHLIGHTDATA_H
