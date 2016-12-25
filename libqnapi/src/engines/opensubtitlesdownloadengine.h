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

#ifndef OPENSUBTITLESDOWNLOADENGINE_H
#define OPENSUBTITLESDOWNLOADENGINE_H

#include "subtitledownloadengine.h"
#include "utils/syncxmlrpc.h"

const QString openSubtitlesXmlRpcUrl = "http://api.opensubtitles.org/xml-rpc";

class OpenSubtitlesDownloadEngine : public SubtitleDownloadEngine
{
public:

    OpenSubtitlesDownloadEngine(const QString & qnapiDisplayableVersion);
    ~OpenSubtitlesDownloadEngine();

    static QString name;

    QString engineName() const;
    QString engineInfo() const;
    QUrl registrationUrl() const;
    const char * const * enginePixmapData() const;

    // oblicza sume kontrolna pliku filmowego
    QString checksum(QString filename = "");
    // szuka napisow
    bool lookForSubtitles(QString lang);
    // wyniki wyszukiwania
    QList<QNapiSubtitleInfo> listSubtitles();
    // probuje pobrac napisy
    bool download(QUuid id);
    // probuje rozpakowac napisy
    bool unpack(QUuid id);
    // czysci smieci, pliki tymczasowe
    void cleanup();

private:

    QString p7zipPath, lang, subFileName;
    quint64 fileSize;

    SyncXmlRpc rpc;
    QString token;
    QString qnapiDisplayableVersion;

    // sprawdza czy dana instancja klasy jest zalogowana na sewerze
    bool isLogged() { return !token.isEmpty(); }
    // loguje na serwer OpenSubtitles
    bool login();
    // wylogowuje z serwera
    void logout();
};

#endif
