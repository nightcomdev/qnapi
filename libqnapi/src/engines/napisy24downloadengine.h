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

#ifndef NAPISY24DOWNLOADENGINE_H
#define NAPISY24DOWNLOADENGINE_H

#include "subtitledownloadengine.h"
#include "utils/synchttp.h"
#include "config/staticconfig.h"

#include <QSharedPointer>

class Napisy24DownloadEngine : public SubtitleDownloadEngine
{
public:
    Napisy24DownloadEngine();
    ~Napisy24DownloadEngine();

    static QString name;

    QString engineName() const;
    QString engineInfo() const;
    QUrl registrationUrl() const;
    const char * const * enginePixmapData() const;


    QString checksum(QString filename = "");
    bool lookForSubtitles(QString lang);
    QList<QNapiSubtitleInfo> listSubtitles();
    bool download(QUuid id);
    bool unpack(QUuid id);
    void cleanup();

private:
    QPair<QString, QString> getCredentials() const;

    QSharedPointer<const StaticConfig> staticConfig;

    quint64 fileSize;
    QString p7zipPath;
};

#endif // NAPISY24DOWNLOADENGINE_H
