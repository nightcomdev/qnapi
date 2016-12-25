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

#include "config/engineconfig.h"
#include "config/staticconfig.h"
#include "engines/subtitledownloadengine.h"
#include "utils/synchttp.h"
#include "utils/p7zipdecoder.h"

#include <QSharedPointer>

class Napisy24DownloadEngine : public SubtitleDownloadEngine
{
public:
    Napisy24DownloadEngine(const QString & tmpPath,
                           const EngineConfig & config,
                           const QSharedPointer<const P7ZipDecoder> & p7zipDecoder,
                           const QStringList & subtitleExtensions);
    ~Napisy24DownloadEngine();

    static QString name;
    static const char * const pixmapData[];

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

    const EngineConfig & engineConfig;
    const QSharedPointer<const P7ZipDecoder> p7zipDecoder;
    const QStringList & subtitleExtensions;

    quint64 fileSize;
};

#endif // NAPISY24DOWNLOADENGINE_H
