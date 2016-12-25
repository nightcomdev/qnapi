#ifndef QSUBPOSTPROCESS_H
#define QSUBPOSTPROCESS_H


#include "subconvert/subtitleconverter.h"
#include "utils/encodingutils.h"

#include <QString>
#include <QSharedPointer>

class QSubPostProcess
{
public:

    QSubPostProcess(QString _movieFilePath,
                    QString _subtitleFilePath);

    void perform() const;

private:

    bool ppReplaceDiacriticsWithASCII() const;
    bool ppChangeSubtitlesEncoding(const QString & from, const QString & to) const;
    bool ppChangeSubtitlesEncoding(const QString & to) const;
    bool ppRemoveLinesContainingWords(QStringList wordList) const;

    QString movieFilePath;
    QString subtitleFilePath;

    QSharedPointer<const SubtitleConverter> subtitleConverter;
    EncodingUtils encodingUtils;
};

#endif // QSUBPOSTPROCESS_H
