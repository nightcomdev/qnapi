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

#ifndef QNAPISUBTITLEDOWNLOADER_H
#define QNAPISUBTITLEDOWNLOADER_H

#include "config/qnapiconfig.h"
#include "engines/subtitledownloadengine.h"
#include "subtitleinfo.h"

#include <Maybe.h>

#include <QList>
#include <QMutex>
#include <QObject>
#include <QQueue>
#include <QSharedPointer>
#include <QWaitCondition>

class QNapiSubtitleDownloader : public QObject
{
    Q_OBJECT
public:
    explicit QNapiSubtitleDownloader(const QNapiConfig & config, QObject *parent = 0);

    void beginDownload();

public slots:

    void enqueueMovieFiles(const QStringList & movieFilePaths);

    void selectSubtitles(int index);
    void resignSelection();
    void cancelDownlaod();

signals:

    void movieFilesEnqueued(const QStringList & movieFilePaths);

    void downloadingStarted();

    void currentFileNameChanged(const QString & fileName, int current, int total);
    void activityChanged(const QString & activityName);
    void fileProgressUpdated(double percentage);

    void subtitleSelectionNeeded(const QString & fileName, const QList<SubtitleInfo> & availableSubtitles);
    void subtitleSelectionNotLongerImportant();

    void downloadingFinished(const QList<SubtitleInfo> & subtitlesSummary);

private:

    void processCurrentFileName();
    bool searchSubtitlesCurrentFileName();
    Maybe<SubtitleInfo> selectSubtitles();
    bool downloadSubtitles(const SubtitleInfo & subtitleInfo);
    bool unpackSubtitles(const SubtitleInfo & subtitleInfo);
    void postProcessSubtitles(const SubtitleInfo & subtitleInfo);
    bool matchSubtitles(const SubtitleInfo & subtitleInfo);

    void addCurrentToSummaryAsFailed();
    void cleanupEngines();

    const QNapiConfig & config;

    QList<QSharedPointer<SubtitleDownloadEngine>> engines;

    QQueue<QString> movieFileNamesQueue;
    QList<SubtitleInfo> subtitlesSummary;

    QString currentFileName;
    int currentProcessingFile, totalFiles;
    QList<SubtitleInfo> currentFoundSubtitles;

    bool cancelRequested;
    int lastSelectedIndex;

    QMutex queueMutex, selectionMutex;
    QWaitCondition subtitleSelected;

};

#endif // QNAPISUBTITLEDOWNLOADER_H
