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

#include "configreader.h"
#include <QDebug>

ConfigReader::ConfigReader(const QSharedPointer<const StaticConfig> & staticConfig)
    : staticConfig(staticConfig)
{}


const QNapiConfig2 ConfigReader::readUserConfig() const
{
    return readConfig(QSettings(QSettings::IniFormat, QSettings::UserScope, "qnapi"));
}

const QNapiConfig2 ConfigReader::readPortableConfig(const QString & configFilePath) const
{
    return readConfig(QSettings(configFilePath, QSettings::IniFormat));
}

const QNapiConfig2 ConfigReader::readConfig(const QSettings & settings) const
{
    return QNapiConfig2(settings.value("qnapi/firstrun", true).toBool(),
                       settings.value("qnapi/version", "").toString(),
                       readGeneralConfig(settings),
                       readEnabledEngines(settings),
                       readEnginesConfig(settings),
                       readPostProcessingConfig(settings),
                       readScanConfig(settings));
}

const GeneralConfig ConfigReader::readGeneralConfig(const QSettings & settings) const
{
    return GeneralConfig(settings.value("qnapi/7z_path", "").toString(),
                         settings.value("qnapi/tmp_path", "").toString(),
                         settings.value("qnapi/language", "pl").toString(),
                         settings.value("qnapi/language_backup", "en").toString(),
                         settings.value("qnapi/no_backup", false).toBool(),
                         settings.value("qnapi/quiet_batch", false).toBool(),
                         (SearchPolicy2) settings.value("qnapi/search_policy", SP_BREAK_IF_FOUND_2).toInt(),
                         (DownloadPolicy2) settings.value("qnapi/download_policy", DP_SHOW_LIST_IF_NEEDED_2).toInt(),
                         settings.value("qnapi/change_permissions", false).toBool(),
                         settings.value("qnapi/permissions", "644").toString());
}

const QList<QPair<QString, bool>> ConfigReader::readEnabledEngines(const QSettings & settings) const
{
    QList<QPair<QString, bool>> defaultEnabledEngines;
    foreach(QString engineName, staticConfig->subtitleEngineNames())
    {
        defaultEnabledEngines << qMakePair(engineName, true);
    }

    QStringList enabledEnginesStr = settings.value("qnapi/engines", QStringList()).toStringList();

    if(enabledEnginesStr.size() != staticConfig->subtitleEngineNames().size()) {
        return defaultEnabledEngines;
    } else {
        QList<QPair<QString, bool>> enabledEngines;
        foreach(QString engineEnableStr, enabledEnginesStr)
        {
            QStringList engineParts = engineEnableStr.split(":", QString::SkipEmptyParts);
            if(engineParts.size() != 2)
            {
                return defaultEnabledEngines;
            }
            enabledEngines << qMakePair(engineParts[0], engineParts[1] == "on");
        }
        return enabledEngines;
    }
}

const QMap<QString, EngineConfig> ConfigReader::readEnginesConfig(const QSettings & settings) const
{
    QMap<QString, EngineConfig> engineConfigurations;
    foreach(QString engineName, staticConfig->subtitleEngineNames())
    {
        engineConfigurations.insert(engineName, readEngineConfig(engineName, settings));
    }
    return engineConfigurations;
}

const EngineConfig ConfigReader::readEngineConfig(QString engineName, const QSettings & settings) const
{
    return EngineConfig(settings.value(engineName + "/nick", "").toString(),
                        settings.value(engineName + "/password", "").toString());
}

const PostProcessingConfig ConfigReader::readPostProcessingConfig(const QSettings & settings) const
{
    return PostProcessingConfig(settings.value("qnapi/post_processing", true).toBool(),
                                (EncodingChangeMethod) settings.value("qnapi/encoding_method", ECM_CHANGE).toInt(),
                                settings.value("qnapi/enc_from", "windows-1250").toString(),
                                settings.value("qnapi/auto_detect_encoding", true).toBool(),
                                settings.value("qnapi/enc_to", "UTF-8").toString(),
                                settings.value("qnapi/show_all_encodings", false).toBool(),
                                settings.value("qnapi/sub_format", "srt").toString(),
                                settings.value("qnapi/sub_ext", "").toString(),
                                settings.value("qnapi/skip_convert_ads", false).toBool(),
                                settings.value("qnapi/remove_words", QStringList() << "movie info" << "synchro").toStringList());
}

const ScanConfig ConfigReader::readScanConfig(const QSettings & settings) const
{
    return ScanConfig(settings.value("scan/last_scan_dir", "").toString(),
                      settings.value("scan/skip_if_subtitles_exist", false).toBool(),
                      settings.value("scan/filters", QStringList()).toStringList(),
                      settings.value("scan/skip_filters", "PL dubbing").toString());
}
