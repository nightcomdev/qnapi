#ifndef SUBTITLEDOWNLOADENGINESREGISTRY_H
#define SUBTITLEDOWNLOADENGINESREGISTRY_H

#include "config/qnapiconfig.h"

#include "engines/subtitledownloadengine.h"

#include "utils/p7zipdecoder.h"

#include <functional>
#include <QSharedPointer>
#include <QString>
#include <QStringList>

class SubtitleDownloadEnginesRegistry
{
public:
    SubtitleDownloadEnginesRegistry(const QString & qnapiDisplayableVersion,
                                    const QStringList & subtitleExtensions,
                                    std::function<QSharedPointer<const P7ZipDecoder> (const QString &)> p7zipDecoderFactory);

    QStringList listEngineNames() const;
    const char * const * enginePixmapData(const QString & engineName) const;

    QSharedPointer<SubtitleDownloadEngine> createEngine(const QString & engineName, const QNapiConfig2 & config) const;
    QList<QSharedPointer<SubtitleDownloadEngine>> createEngines(const QStringList & engineNames, const QNapiConfig2 & config) const;
    QList<QSharedPointer<SubtitleDownloadEngine>> createEnabledEngines(const QNapiConfig2 & config) const;

private:
    const QString qnapiDisplayableVersion;
    const QStringList subtitleExtensions;
    std::function<QSharedPointer<const P7ZipDecoder> (const QString &)> p7zipDecoderFactory;
};

#endif // SUBTITLEDOWNLOADENGINESREGISTRY_H
