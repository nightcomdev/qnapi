#include "subtitledownloadenginesregistry.h"

#include "engines/napiprojektdownloadengine.h"
#include "engines/opensubtitlesdownloadengine.h"
#include "engines/napisy24downloadengine.h"

#include "utils/p7zipdecoder.h"

#include <QPair>

SubtitleDownloadEnginesRegistry::SubtitleDownloadEnginesRegistry(const QString & qnapiDisplayableVersion,
                                                                 const QStringList & subtitleExtensions,
                                                                 std::function<QSharedPointer<const P7ZipDecoder> (const QString &)> p7zipDecoderFactory)
    : qnapiDisplayableVersion(qnapiDisplayableVersion),
      subtitleExtensions(subtitleExtensions),
      p7zipDecoderFactory(p7zipDecoderFactory)
{
}

QStringList SubtitleDownloadEnginesRegistry::listEngineNames() const
{
    static QStringList engineNames = {
        NapiProjektDownloadEngine::name,
        OpenSubtitlesDownloadEngine::name,
        Napisy24DownloadEngine::name
    };
    return engineNames;
}

const char * const * SubtitleDownloadEnginesRegistry::enginePixmapData(const QString & engineName) const
{
    if(engineName == NapiProjektDownloadEngine::name)
        return NapiProjektDownloadEngine::pixmapData;

    if(engineName == OpenSubtitlesDownloadEngine::name)
        return OpenSubtitlesDownloadEngine::pixmapData;

    if(engineName == Napisy24DownloadEngine::name)
        return Napisy24DownloadEngine::pixmapData;

    return {};
}


QSharedPointer<SubtitleDownloadEngine>
SubtitleDownloadEnginesRegistry::createEngine(const QString & engineName, const QNapiConfig2 & config) const
{
    QSharedPointer<const P7ZipDecoder> p7zipDecoder = p7zipDecoderFactory(config.generalConfig().p7zipPath());
    const QString & tmpPath = config.generalConfig().tmpPath();

    if(engineName == NapiProjektDownloadEngine::name)
    {
        const EngineConfig & engineConfig = config.enginesConfig()[NapiProjektDownloadEngine::name];
        return QSharedPointer<SubtitleDownloadEngine>(
            new NapiProjektDownloadEngine(tmpPath, engineConfig, p7zipDecoder)
        );
    }

    if(engineName == OpenSubtitlesDownloadEngine::name)
    {
        const EngineConfig & engineConfig = config.enginesConfig()[OpenSubtitlesDownloadEngine::name];
        const QString & language = config.generalConfig().language();

        return QSharedPointer<SubtitleDownloadEngine>(
            new OpenSubtitlesDownloadEngine(tmpPath, engineConfig, p7zipDecoder, qnapiDisplayableVersion, language)
        );
    }

    if(engineName == Napisy24DownloadEngine::name)
    {
        const EngineConfig & engineConfig = config.enginesConfig()[Napisy24DownloadEngine::name];

        return QSharedPointer<SubtitleDownloadEngine>(
            new Napisy24DownloadEngine(tmpPath, engineConfig, p7zipDecoder, subtitleExtensions)
        );
    }

    return QSharedPointer<SubtitleDownloadEngine>();
}

QList<QSharedPointer<SubtitleDownloadEngine>>
SubtitleDownloadEnginesRegistry::createEngines(const QStringList & engineNames, const QNapiConfig2 & config) const
{
    QList<QSharedPointer<SubtitleDownloadEngine>> engines;
    foreach(QString engineName, engineNames)
    {
        engines << createEngine(engineName, config);
    }
    return engines;
}


QList<QSharedPointer<SubtitleDownloadEngine>>
SubtitleDownloadEnginesRegistry::createEnabledEngines(const QNapiConfig2 & config) const
{
    QStringList engineNames;
    typedef QPair<QString, bool> EngineEnableCfg;

    foreach(EngineEnableCfg engineCfg, config.enabledEngines())
    {
        if(engineCfg.second)
            engineNames << engineCfg.first;
    }

    return createEngines(engineNames, config);
}
