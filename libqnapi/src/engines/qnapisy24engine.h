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

#ifndef QNAPISY24ENGINE_H
#define QNAPISY24ENGINE_H

#include "qnapiabstractengine.h"
#include "utils/synchttp.h"
#include "config/staticconfig.h"

#include <QSharedPointer>

class QNapisy24Engine : public QNapiAbstractEngine
{
public:
    QNapisy24Engine();
    ~QNapisy24Engine();

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

#endif // QNAPISY24ENGINE_H
