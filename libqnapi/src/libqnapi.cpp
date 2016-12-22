/*****************************************************************************
** QNapi
** Copyright (C) 2008-2016 Piotr Krzemiński <pio.krzeminski@gmail.com>
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

#include "libqnapi.h"
#include "version.h"
#include "movieinfo/libmediainfomovieinfoprovider.h"

LibQNapi::LibQNapi()
{
}

QString LibQNapi::version()
{
    return QString(QNAPI_VERSION);
}

QString LibQNapi::displayableVersion()
{
    return QString(QNAPI_DISPLAYABLE_VERSION);
}

QString LibQNapi::webpageUrl()
{
    return QString(QNAPI_URL);
}

QSharedPointer<const MovieInfoProvider> LibQNapi::movieInfoProvider()
{
    return QSharedPointer<const MovieInfoProvider>(new LibmediainfoMovieInfoProvider());
}

QSharedPointer<const StaticConfig> LibQNapi::staticConfigProvider()
{
    return QSharedPointer<const StaticConfig>(new StaticConfig());
}

QSharedPointer<const ConfigReader> LibQNapi::configReader()
{
    return QSharedPointer<const ConfigReader>(new ConfigReader(staticConfigProvider()));
}

QSharedPointer<const ConfigWriter> LibQNapi::configWriter()
{
    QString ver = version();
    return QSharedPointer<const ConfigWriter>(new ConfigWriter(ver));
}

