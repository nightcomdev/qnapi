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

#ifndef QNAPICONFIG_OLD_H
#define QNAPICONFIG_OLD_H

#include "config/staticconfig.h"

#include <QSettings>
#include <QString>
#include <QStringList>
#include <QList>
#include <QPair>
#include <QSharedPointer>

#ifdef Q_OS_MAC
#include <QtXml>
#endif

enum OldSearchPolicy
{
    OLD_SP_BREAK_IF_FOUND = 0,
    OLD_SP_SEARCH_ALL = 1,
    OLD_SP_SEARCH_ALL_WITH_BACKUP_LANG = 2
};

enum OldDownloadPolicy
{
    OLD_DP_ALWAYS_SHOW_LIST = 0,
    OLD_DP_SHOW_LIST_IF_NEEDED = 1,
    OLD_DP_NEVER_SHOW_LIST  = 2
};

enum OldChangeEncodingMethod
{
    OLD_CEM_ORIGINAL = 0,
    OLD_CEM_CHANGE = 1,
    OLD_CEM_REPLACE_DIACRITICS = 2
};

class QNapiConfigOld
{
    public:
        void load(QString appDirPath);
        void save();

        void performMigrations();

        bool firstRun();
        QString version();

        QString p7zipPath();
        void setP7zipPath(const QString & path);

        QString tmpPath();
        void setTmpPath(const QString & path);

        QString nick(const QString & engine);
        void setNick(const QString & engine, const QString & nick);

        QString pass(const QString & engine);
        void setPass(const QString & engine, const QString & pass);

        QString language();
        void setLanguage(const QString & language);

        QString languageBackup();
        void setLanguageBackup(const QString & language);

        bool noBackup();
        void setNoBackup(bool noBackup);

        bool quietBatch();
        void setQuietBatch(bool quietBatch);

#ifdef Q_OS_MAC
        bool showDockIcon();
        void setShowDockIcon(bool show);
#endif

        QList<QPair<QString, bool> > engines();
        QStringList enginesList();
        void setEngines(QList<QPair<QString, bool> > engines);

        OldSearchPolicy searchPolicy();
        void setSearchPolicy(OldSearchPolicy policy);
        
        OldDownloadPolicy downloadPolicy();
        void setDownloadPolicy(OldDownloadPolicy policy);

        bool ppEnabled();
        void setPpEnabled(bool enable);

        OldChangeEncodingMethod ppEncodingMethod();
        void setPpEncodingMethod(OldChangeEncodingMethod method);
        
        bool ppAutoDetectEncoding();
        void setPpAutoDetectEncoding(bool change);

        QString ppEncodingFrom();
        void setPpEncodingFrom(const QString & encoding);

        QString ppEncodingTo();
        void setPpEncodingTo(const QString & encoding);

        bool ppShowAllEncodings();
        void setPpShowAllEncodings(bool show);

        bool ppRemoveLines();
        void setPpRemoveLines(bool remove);

        QString ppSubFormat();
        void setPpSubFormat(const QString & subFormat);

        QString ppSubExtension();
        void setPpSubExtension(const QString & subExtension);

        bool ppSkipConvertAds();
        void setPpSkipConvertAds(bool skip);

        QStringList ppRemoveWords();
        void setPpRemoveWords(const QStringList & words);

        bool changePermissions();
        void setChangePermissions(bool change);

        QString changePermissionsTo();
        void setChangePermissionsTo(const QString & permissions);

        QString previousDialogPath();
        void setPreviousDialogPath(const QString & path);

        QStringList scanFilters();
        void setScanFilters(const QStringList & filters);

        QString scanSkipFilters();
        void setScanSkipFilters(const QString & filters);

        bool scanSkipIfSubtitlesExists();
        void setScanSkipIfSubtitlesExists(bool skip);

        QString lastScanDir();
        void setLastScanDir(const QString & dir);

    private:
        QNapiConfigOld(const QString & qnapiVersion,
                    const QSharedPointer<const StaticConfig> & staticConfig);
        QNapiConfigOld(const QNapiConfigOld &);
        ~QNapiConfigOld();

        QSettings *settings;
        bool isPortableMode;
        QString qnapiVersion;
        QSharedPointer<const StaticConfig> staticConfig;

    friend QNapiConfigOld& OldGlobalConfig();
};

QNapiConfigOld & OldGlobalConfig();

#endif
