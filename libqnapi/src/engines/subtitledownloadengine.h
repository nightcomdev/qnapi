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

#ifndef SUBTITLEDOWNLOADENGINE_H
#define SUBTITLEDOWNLOADENGINE_H

#include <QString>
#include <QFileInfo>
#include <QFile>
#include <QUrl>

#include <ctime>

#include <Maybe.h>

#include "qnapisubtitleinfo.h"

class SubtitleDownloadEngine
{
public:

    virtual ~SubtitleDownloadEngine() {}

    void setMoviePath(const QString & path);
    QString moviePath();

    virtual QString engineName() const = 0;
    virtual QString engineInfo() const = 0;
    virtual QUrl registrationUrl() const = 0;
    virtual const char * const * enginePixmapData() const = 0;

    virtual void clearSubtitlesList();
    virtual QString checksum(QString filename = "") = 0;
    virtual bool lookForSubtitles(QString lang) = 0;
    virtual QList<QNapiSubtitleInfo> listSubtitles() = 0;
    virtual bool download(QUuid id) = 0;
    virtual bool unpack(QUuid id) = 0;
    virtual void cleanup() = 0;

protected:

    QString tmpPath;

    QList<QNapiSubtitleInfo> subtitlesList;
    QString movie;
    QString subtitles;
    QString subtitlesTmp;
    QString checkSum;

    SubtitleDownloadEngine(const QString & tmpPath);

    Maybe<QNapiSubtitleInfo> resolveById(QUuid id);
    void updateSubtitleInfo(const QNapiSubtitleInfo & si);

    QString generateTmpFileName() const;
    QString generateTmpPath() const;

    friend class QNapi;
};

#endif
