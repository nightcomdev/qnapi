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

#include "libqnapi.h"
#include "qnapi.h"
#include "qnapiconfig.h"
#include "qsubmatcher.h"
#include "qsubpostprocess.h"

#include "config/configreader.h"

#include "engines/napiprojektdownloadengine.h"
#include "engines/napisy24downloadengine.h"
#include "engines/opensubtitlesdownloadengine.h"
#include <QtAlgorithms>

QNapi::QNapi()
    : enginesRegistry(LibQNapi::subtitleDownloadEngineRegistry()),
      config(LibQNapi::loadConfig())
{}


QNapi::~QNapi()
{
    cleanup();
}

bool QNapi::checkP7ZipPath()
{
    return QFileInfo(GlobalConfig().p7zipPath()).isExecutable();
}

bool QNapi::checkTmpPath()
{
    QFileInfo f(GlobalConfig().tmpPath());
    return f.isDir() && f.isWritable();
}

bool QNapi::ppEnabled()
{
    return GlobalConfig().ppEnabled();
}

QStringList QNapi::enumerateEngines()
{
    return enginesRegistry->listEngineNames();
}

bool QNapi::addEngine(QString engine)
{
    enginesList << enginesRegistry->createEngine(engine, config);
    return true;
}

bool QNapi::addEngines(QStringList engines)
{
    enginesList << enginesRegistry->createEngines(engines, config);
    return true;
}

void QNapi::setMoviePath(QString path)
{
    movie = path;
    currentEngine = QSharedPointer<SubtitleDownloadEngine>();
}

QString QNapi::moviePath()
{
    return movie;
}

bool QNapi::checkWritePermissions()
{
    return QFileInfo(QFileInfo(movie).path()).isWritable();
}

void QNapi::clearSubtitlesList()
{
    subtitlesList.clear();
    foreach(QSharedPointer<SubtitleDownloadEngine> e, enginesList)
    {
        e->clearSubtitlesList();
    }
}

void QNapi::checksum()
{
    foreach(QSharedPointer<SubtitleDownloadEngine> e, enginesList)
    {
        e->checksum(movie);
    }
}

bool QNapi::lookForSubtitles(QString lang, QString engine)
{
    bool result = false;


    if(engine.isEmpty())
    {
        foreach(QSharedPointer<SubtitleDownloadEngine> e, enginesList)
        {
            e->setMoviePath(movie);
            result = e->lookForSubtitles(lang) || result;
        }
    }
    else
    {
        QSharedPointer<SubtitleDownloadEngine> e = engineByName(engine);
        if(e)
        {
            e->setMoviePath(movie);
            result = e->lookForSubtitles(lang);
        }
    }

    if(!result)
    {
        errorMsg = "Nie znaleziono napisów!";
    }

    return result;
}

QList<QNapiSubtitleInfo> QNapi::listSubtitles()
{
    subtitlesList.clear();

    foreach(QSharedPointer<SubtitleDownloadEngine> e, enginesList)
    {
        QList<QNapiSubtitleInfo> list =  e->listSubtitles();
        subtitlesList << list;
    }

    QString mainLang = GlobalConfig().language();
    QString backupLang = GlobalConfig().languageBackup();

    auto langRank = [&](QString lang) {
        if(lang == mainLang) return 0;
        if(lang == backupLang) return 1;
        return 2;
    };

    qStableSort(subtitlesList.begin(), subtitlesList.end(),
                [&](const QNapiSubtitleInfo & si1, const QNapiSubtitleInfo & si2) {
       return langRank(si1.lang) < langRank(si2.lang);
    });

    return subtitlesList;
}

bool QNapi::needToShowList()
{
    theBestIdx = 0;

    int i = 0;
    bool foundBestIdx = false;
    foreach(QNapiSubtitleInfo s, listSubtitles())
    {
        if(s.resolution == SUBTITLE_GOOD)
        {
            theBestIdx = i;
            foundBestIdx = true;
            break;
        }
        ++i;
    }

    if(GlobalConfig().downloadPolicy() == DP_ALWAYS_SHOW_LIST)  
        return true;
    if(GlobalConfig().downloadPolicy() == DP_NEVER_SHOW_LIST)   
        return false;

    if(listSubtitles().size() <= 1)
        return false;

    return !foundBestIdx;
}

int QNapi::bestIdx()
{
    return theBestIdx;
}

bool QNapi::download(int i)
{
    QNapiSubtitleInfo s = subtitlesList[i];
    currentEngine = engineByName(s.engine);
    if(!currentEngine) return false;
    return currentEngine->download(s.id);
}

bool QNapi::unpack(int i)
{
    return currentEngine
            ? currentEngine->unpack(subtitlesList[i].id)
            : false;
}

bool QNapi::match()
{
    QNapiConfig & config = GlobalConfig();

    QSubMatcher subMatcher(config.noBackup(),
                           config.ppEnabled(),
                           config.ppSubFormat(),
                           config.ppSubExtension(),
                           config.changePermissions(),
                           config.changePermissionsTo());

    return currentEngine
             ? subMatcher.matchSubtitles(currentEngine->subtitlesTmp,
                                         currentEngine->movie)
             : false;
}

void QNapi::pp()
{
    if(currentEngine) {
        QSubPostProcess pp(currentEngine->movie,
                           currentEngine->subtitlesTmp);
        pp.perform();
    }
}

void QNapi::cleanup()
{
    foreach(QSharedPointer<SubtitleDownloadEngine> e, enginesList)
    {
        e->cleanup();
    }
}

QString QNapi::error()
{
    return errorMsg;
}

QSharedPointer<SubtitleDownloadEngine> QNapi::engineByName(QString name)
{
    foreach(QSharedPointer<SubtitleDownloadEngine> e, enginesList)
    {
        if(name == (e->engineName()))
        {
            return e;
        }
    }

    return QSharedPointer<SubtitleDownloadEngine>();
}

QStringList QNapi::listLoadedEngines()
{
    QStringList list;
    foreach(QSharedPointer<SubtitleDownloadEngine> e, enginesList)
    {
        list << e->engineName();
    }
    return list;
}
