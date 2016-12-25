/*****************************************************************************
** QNapi
** Copyright (C) 2008-2015 Piotr Krzemiński <pio.krzeminski@gmail.com>
**
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
*****************************************************************************/

#include "subtitleformatsregistry.h"

SubtitleFormatsRegistry::SubtitleFormatsRegistry()
{}

SubtitleFormatsRegistry::~SubtitleFormatsRegistry()
{
    foreach(QString formatName, formats.keys())
    {
        delete formats[formatName];
    }
}

void SubtitleFormatsRegistry::registerFormat(SubtitleFormat *format)
{
    formats.insert(format->formatName(), format);
}

QStringList SubtitleFormatsRegistry::enumerateFormats() const
{
    return formats.keys();
}

SubtitleFormat* SubtitleFormatsRegistry::select(const QString & format) const
{
    return formats.value(format, 0);
}

