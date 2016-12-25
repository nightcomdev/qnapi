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

#ifndef __QNAPI__H__
#define __QNAPI__H__

#include "config/configreader.h"
#include "engines/subtitledownloadengine.h"
#include "engines/subtitledownloadenginesregistry.h"
#include "qnapisubtitleinfo.h"

#include <QList>
#include <QSharedPointer>
#include <QString>
#include <QStringList>


// globalny menedzer pobierania napisow
class QNapi
{
    public:

        QNapi();
        ~QNapi();

        static bool checkP7ZipPath();
        static bool checkTmpPath();
        static bool ppEnabled();

        QStringList enumerateEngines();
        bool addEngine(QString engine);
        bool addEngines(QStringList engines);

        void setMoviePath(QString path);
        QString moviePath();
        bool checkWritePermissions();

        void clearSubtitlesList();
        void checksum();
        bool lookForSubtitles(QString lang, QString engine = "");
        bool lookForSubtitles(QStringList languages, QString engine = "");
        QList<QNapiSubtitleInfo> listSubtitles();

        bool needToShowList();
        int bestIdx();

        bool download(int i);
        bool unpack(int i);
        bool match();
        void pp();

        void cleanup();
        QString error();

        QSharedPointer<SubtitleDownloadEngine> engineByName(QString name);
        QStringList listLoadedEngines();

    private:

        QString movie;
        QString errorMsg;
        QList<QSharedPointer<SubtitleDownloadEngine>> enginesList;
        QList<QNapiSubtitleInfo> subtitlesList;
        QSharedPointer<SubtitleDownloadEngine> currentEngine;
        
        // najlepszy indeks napisow
        int theBestIdx;

        const QSharedPointer<const SubtitleDownloadEnginesRegistry> enginesRegistry;
        const QNapiConfig2 config;
};


#endif // __QNAPI__H__
