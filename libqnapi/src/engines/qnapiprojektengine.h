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

#ifndef __QNAPIPROJEKTENGINE__H__
#define __QNAPIPROJEKTENGINE__H__

#include "qnapiabstractengine.h"

class QNapiProjektEngine : public QNapiAbstractEngine
{
public:

    QNapiProjektEngine();
    ~QNapiProjektEngine();

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

    static bool checkUser(const QString & nick, const QString & pass);

private:

    QString p7zipPath, nick, pass;

    QString checksum(QString filename, bool limit10M);
    Maybe<QString> downloadByLangAndChecksum(QString lang, QString checksum) const;
    QString npFDigest(const QString & input) const;
    QString npLangWrapper(QString lang) const;
    QString napiOS() const;

};

#endif
