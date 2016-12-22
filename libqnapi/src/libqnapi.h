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

#ifndef LIBQNAPI_H
#define LIBQNAPI_H

#include "movieinfo/movieinfoprovider.h"
#include "config/staticconfig.h"
#include "config/configreader.h"
#include "config/configwriter.h"
#include <QString>
#include <QSharedPointer>

class LibQNapi
{

public:
    LibQNapi();

    static QString version();
    static QString displayableVersion();
    static QString webpageUrl();
    static QSharedPointer<const MovieInfoProvider> movieInfoProvider();
    static QSharedPointer<const StaticConfig> staticConfigProvider();
    static QSharedPointer<const ConfigReader> configReader();
    static QSharedPointer<const ConfigWriter> configWriter();
};

#endif // LIBQNAPI_H
