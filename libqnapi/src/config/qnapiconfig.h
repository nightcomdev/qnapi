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

#ifndef QNAPICONFIG_H
#define QNAPICONFIG_H

#include <QString>
#include <QMap>
#include <config/engineconfig.h>
#include <config/generalconfig.h>
#include <config/postprocessingconfig.h>
#include <config/scanconfig.h>

class QNapiConfig {
private:
    bool firstrun_;
    QString version_;
    GeneralConfig generalConfig_;
    QStringList enabledEngines_;
    QMap<QString, EngineConfig> enginesConfig_;
    PostProcessingConfig postProcessingConfig_;
    ScanConfig scanConfig_;

public:
    QNapiConfig(const bool & firstrun,
                const QString & version,
                const GeneralConfig & generalConfig,
                const QStringList & enabledEngines,
                const QMap<QString, EngineConfig> & enginesConfig,
                const PostProcessingConfig & postProcessingConfig,
                const ScanConfig & scanConfig)
        : firstrun_(firstrun),
          version_(version),
          generalConfig_(generalConfig),
          enabledEngines_(enabledEngines),
          enginesConfig_(enginesConfig),
          postProcessingConfig_(postProcessingConfig),
          scanConfig_(scanConfig)
        {}

    bool firstrun() const { return firstrun_; }
    QString version() const { return version_; }
    GeneralConfig generalConfig() const { return generalConfig_; }
    QStringList enabledEngines() const { return enabledEngines_; }
    QMap<QString, EngineConfig> enginesConfig() const { return enginesConfig_; }
    PostProcessingConfig postProcessingConfig() const { return postProcessingConfig_; }
    ScanConfig scanConfig() const { return scanConfig_; }

    const QNapiConfig setFirstrun(const bool & firstrun) const {
        return QNapiConfig(firstrun, version_, generalConfig_, enabledEngines_, enginesConfig_, postProcessingConfig_, scanConfig_);
    }
    const QNapiConfig setVersion(const QString & version) const {
        return QNapiConfig(firstrun_, version, generalConfig_, enabledEngines_, enginesConfig_, postProcessingConfig_, scanConfig_);
    }
    const QNapiConfig setGeneralConfig(const GeneralConfig & generalConfig) const {
        return QNapiConfig(firstrun_, version_, generalConfig, enabledEngines_, enginesConfig_, postProcessingConfig_, scanConfig_);
    }
    const QNapiConfig setEnabledEngines(const QStringList & enabledEngines) const {
        return QNapiConfig(firstrun_, version_, generalConfig_, enabledEngines, enginesConfig_, postProcessingConfig_, scanConfig_);
    }
    const QNapiConfig setEnginesConfig(const QMap<QString, EngineConfig> & enginesConfig) const {
        return QNapiConfig(firstrun_, version_, generalConfig_, enabledEngines_, enginesConfig, postProcessingConfig_, scanConfig_);
    }
    const QNapiConfig setPostProcessingConfig(const PostProcessingConfig & postProcessingConfig) const {
        return QNapiConfig(firstrun_, version_, generalConfig_, enabledEngines_, enginesConfig_, postProcessingConfig, scanConfig_);
    }
    const QNapiConfig setScanConfig(const ScanConfig & scanConfig) const {
        return QNapiConfig(firstrun_, version_, generalConfig_, enabledEngines_, enginesConfig_, postProcessingConfig_, scanConfig);
    }
};

#endif
