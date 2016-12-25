#ifndef SUBTITLEDOWNLOADENGINESREGISTRY_H
#define SUBTITLEDOWNLOADENGINESREGISTRY_H

#include "engines/subtitledownloadengine.h"

#include <QSharedPointer>
#include <QString>
#include <QStringList>

class SubtitleDownloadEnginesRegistry
{
public:
    SubtitleDownloadEnginesRegistry(const QString & qnapiDisplayableVersion);

    QStringList listEngineNames() const;
    QSharedPointer<SubtitleDownloadEngine> createEngine(const QString & engineName) const;
    QList<QSharedPointer<SubtitleDownloadEngine>> createEngines(const QStringList & engineNames) const;
    const char * const * enginePixmapData(const QString & engineName) const;

private:
    const QString qnapiDisplayableVersion;
};

#endif // SUBTITLEDOWNLOADENGINESREGISTRY_H
