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
#include "qnapisubtitledownloader.h"


QNapiSubtitleDownloader::QNapiSubtitleDownloader(const QNapiConfig & config, QObject *parent)
    : QObject(parent),
      config(config),
      engines(LibQNapi::subtitleDownloadEngineRegistry()->createEnabledEngines(config))
{}

void QNapiSubtitleDownloader::enqueueMovieFiles(const QStringList & movieFilePaths)
{
    queueMutex.lock();
    movieFileNamesQueue << movieFilePaths;
    totalFiles += movieFilePaths.size();
    emit movieFilesEnqueued(movieFilePaths);
    emit currentFileNameChanged(currentFileName, currentProcessingFile, totalFiles);
    queueMutex.unlock();
}

void QNapiSubtitleDownloader::beginDownload()
{
    currentProcessingFile = 0;
    cancelRequested = false;

    emit downloadingStarted();

    do {
        queueMutex.lock();
        if(movieFileNamesQueue.isEmpty())
            break;
        currentFileName = movieFileNamesQueue.dequeue();
        queueMutex.unlock();
        processCurrentFileName();
    } while(!cancelRequested);

    emit activityChanged("Sprzątanie...");
    cleanupEngines();

    emit downloadingFinished(subtitlesSummary);
}

void QNapiSubtitleDownloader::selectSubtitles(int index)
{
    lastSelectedIndex = index;
    selectionMutex.unlock();
    subtitleSelected.wakeOne();
}

void QNapiSubtitleDownloader::resignSelection()
{
    selectSubtitles(-1);
}

void QNapiSubtitleDownloader::cancelDownlaod()
{
    cancelRequested = true;

    // if cancel is called during selection is active, then...
    selectionMutex.tryLock(); // mutex isn't locked, as it's already locked; otherwise is locked now
    selectionMutex.unlock(); // we can unlock the mutex
    subtitleSelected.wakeOne(); // and eventually wake the other thread (where download is performed)
    emit subtitleSelectionNotLongerImportant(); // and eventual selection is no loger important for us
}

#define ABORT_POINT if(cancelRequested) return

void QNapiSubtitleDownloader::processCurrentFileName()
{
    ++currentProcessingFile;
    emit fileProgressUpdated(0.0);
    emit currentFileNameChanged(currentFileName, currentProcessingFile, totalFiles);

    bool subtitlesFound = searchSubtitlesCurrentFileName();

    ABORT_POINT;
    emit fileProgressUpdated(0.33);

    if(!subtitlesFound)
    {
        addCurrentToSummaryAsFailed();
        return;
    }

    Maybe<SubtitleInfo> maybeSelectedSubtitles = selectSubtitles();

    ABORT_POINT;

    if(!maybeSelectedSubtitles)
    {
        addCurrentToSummaryAsFailed();
        return;
    }

    const SubtitleInfo & selectedSubtitles = maybeSelectedSubtitles.value();

    if(!downloadSubtitles(selectedSubtitles))
    {
        addCurrentToSummaryAsFailed();
        return;
    }

    ABORT_POINT;
    emit fileProgressUpdated(0.66);

    if(!unpackSubtitles(selectedSubtitles))
    {
        addCurrentToSummaryAsFailed();
        return;
    }

    ABORT_POINT;
    emit fileProgressUpdated(0.77);

    postProcessSubtitles(selectedSubtitles);

    ABORT_POINT;
    emit fileProgressUpdated(0.88);

    if(!matchSubtitles(selectedSubtitles))
    {
        addCurrentToSummaryAsFailed();
        return;
    }

    emit fileProgressUpdated(0.99);

    subtitlesSummary << selectedSubtitles;
}

bool QNapiSubtitleDownloader::searchSubtitlesCurrentFileName()
{ // can make a progress from 0 to 0.33
    emit activityChanged("Szukanie napisów...");


}

Maybe<SubtitleInfo> QNapiSubtitleDownloader::selectSubtitles()
{
    emit activityChanged("Wybieranie napisów...");

}

bool QNapiSubtitleDownloader::downloadSubtitles(const SubtitleInfo & subtitleInfo)
{
    emit activityChanged("Pobieranie napisów...");

}

bool QNapiSubtitleDownloader::unpackSubtitles(const SubtitleInfo & subtitleInfo)
{
    emit activityChanged("Rozpakowywanie napisów...");

}

void QNapiSubtitleDownloader::postProcessSubtitles(const SubtitleInfo & subtitleInfo)
{
    if(config.postProcessingConfig().enabled())
    {
        emit activityChanged("Przetwarzanie napisów...");

    }
}

bool QNapiSubtitleDownloader::matchSubtitles(const SubtitleInfo & subtitleInfo)
{
    emit activityChanged("Dopasowywanie napisów...");

}


void QNapiSubtitleDownloader::addCurrentToSummaryAsFailed()
{
    subtitlesSummary << SubtitleInfo::fromFailed(currentFileName);
}

void QNapiSubtitleDownloader::cleanupEngines()
{
    foreach(QSharedPointer<SubtitleDownloadEngine> engine, engines)
    {
        engine->cleanup();
    }
}
