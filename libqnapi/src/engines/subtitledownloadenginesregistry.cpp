#include "subtitledownloadenginesregistry.h"

#include "engines/napiprojektdownloadengine.h"
#include "engines/opensubtitlesdownloadengine.h"
#include "engines/napisy24downloadengine.h"

SubtitleDownloadEnginesRegistry::SubtitleDownloadEnginesRegistry(const QString & qnapiDisplayableVersion)
    : qnapiDisplayableVersion(qnapiDisplayableVersion)
{}

QStringList SubtitleDownloadEnginesRegistry::listEngineNames() const
{
    static QStringList engineNames = {
        NapiProjektDownloadEngine::name,
        OpenSubtitlesDownloadEngine::name,
        Napisy24DownloadEngine::name
    };
    return engineNames;
}

QSharedPointer<SubtitleDownloadEngine>
SubtitleDownloadEnginesRegistry::createEngine(const QString & engineName) const
{
    if(engineName == NapiProjektDownloadEngine::name)
        return QSharedPointer<SubtitleDownloadEngine>(
            new NapiProjektDownloadEngine
        );

    if(engineName == OpenSubtitlesDownloadEngine::name)
        return QSharedPointer<SubtitleDownloadEngine>(
            new OpenSubtitlesDownloadEngine(qnapiDisplayableVersion)
        );

    if(engineName == Napisy24DownloadEngine::name)
        return QSharedPointer<SubtitleDownloadEngine>(
            new Napisy24DownloadEngine
        );

    return QSharedPointer<SubtitleDownloadEngine>();
}

QList<QSharedPointer<SubtitleDownloadEngine>>
SubtitleDownloadEnginesRegistry::createEngines(const QStringList & engineNames) const
{
    QList<QSharedPointer<SubtitleDownloadEngine>> engines;
    foreach(QString engineName, engineNames)
    {
        engines << createEngine(engineName);
    }
    return engines;
}

const char * const * SubtitleDownloadEnginesRegistry::enginePixmapData(const QString & engineName) const
{
    return createEngine(engineName)->enginePixmapData();
}
