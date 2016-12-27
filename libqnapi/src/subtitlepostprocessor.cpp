#include "subtitlepostprocessor.h"
#include "qnapiconfigold.h"
#include "libqnapi.h"

#include <QTextCodec>
#include <QTextStream>
#include <QFile>
#include <QFileInfo>

SubtitlePostProcessor::SubtitlePostProcessor()
    : subtitleConverter(LibQNapi::subtitleConverter())
{}


void SubtitlePostProcessor::perform(const QString & movieFilePath, const QString & subtitleFilePath) const
{
    if(OldGlobalConfig().ppRemoveLines())
    {
        ppRemoveLinesContainingWords(subtitleFilePath, OldGlobalConfig().ppRemoveWords());
    }

    switch (OldGlobalConfig().ppEncodingMethod()) {
        case OLD_CEM_REPLACE_DIACRITICS:
            ppReplaceDiacriticsWithASCII(subtitleFilePath);
        break;
        case OLD_CEM_CHANGE:
            if (!OldGlobalConfig().ppAutoDetectEncoding() || !ppChangeSubtitlesEncoding(subtitleFilePath, OldGlobalConfig().ppEncodingTo()))
            {
                ppChangeSubtitlesEncoding(subtitleFilePath,
                                          OldGlobalConfig().ppEncodingFrom(),
                                          OldGlobalConfig().ppEncodingTo());
            }
        break;
        case OLD_CEM_ORIGINAL:
            // Nie ruszaj pobranych napisów!
        break;
    }

    if(!OldGlobalConfig().ppSubFormat().isEmpty())
    {
        QString targetFormat = OldGlobalConfig().ppSubFormat();
        subtitleConverter->convertSubtitles(subtitleFilePath, targetFormat, subtitleFilePath, movieFilePath);
    }
}


bool SubtitlePostProcessor::ppReplaceDiacriticsWithASCII(const QString & subtitleFilePath) const
{
    if(!QFileInfo(subtitleFilePath).exists())
        return false;

    QString from = encodingUtils.detectFileEncoding(subtitleFilePath);

    if(from.isEmpty())
        return false;

    QFile f(subtitleFilePath);
    if(!f.open(QIODevice::ReadOnly))
        return false;

    QByteArray fileContent = f.readAll();

    QString contentStr = QTextCodec::codecForName(qPrintable(from))->toUnicode(fileContent);
    f.close();

    fileContent.clear();
    fileContent.append(encodingUtils.replaceDiacriticsWithASCII(contentStr));

    if(!f.open(QIODevice::WriteOnly))
        return false;

    f.write(fileContent);
    f.close();

    return true;
}

bool SubtitlePostProcessor::ppChangeSubtitlesEncoding(const QString & subtitleFilePath, const QString & from, const QString & to) const
{
    QFile f(subtitleFilePath);
    if(!f.open(QIODevice::ReadOnly))
        return false;

    QByteArray fileContent = f.readAll();

    QString contentStr = QTextCodec::codecForName(qPrintable(from))->toUnicode(fileContent);
    f.close();

    if(to.compare("UTF-8", Qt::CaseInsensitive) != 0)
    {
        fileContent = QTextCodec::codecForName(qPrintable(to))
                        ->fromUnicode(contentStr.constData(), contentStr.size());
    }
    else
    {
        fileContent.clear();
        fileContent.append(contentStr);
    }

    if(!f.open(QIODevice::WriteOnly))
        return false;

    f.write(fileContent);
    f.close();

    return true;
}

bool SubtitlePostProcessor::ppChangeSubtitlesEncoding(const QString & subtitleFilePath, const QString & to) const
{
    if(!QFileInfo(subtitleFilePath).exists())
        return false;

    QString from = encodingUtils.detectFileEncoding(subtitleFilePath);

    if(from.isEmpty())
        return false;

    return ppChangeSubtitlesEncoding(from, to);
}

bool SubtitlePostProcessor::ppRemoveLinesContainingWords(const QString & subtitleFilePath, QStringList wordList) const
{
    if(!QFileInfo(subtitleFilePath).exists())
        return false;

    wordList = wordList.filter(QRegExp("^(.+)$"));

    QString fromCodec = encodingUtils.detectFileEncoding(subtitleFilePath);

    if(fromCodec.isEmpty())
        fromCodec = OldGlobalConfig().ppEncodingFrom();

    QFile f(subtitleFilePath);
    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QList<QByteArray> lines = f.readAll().split('\n');
    QList<QByteArray> out;

    foreach(QByteArray line, lines)
    {
        int i;
        while((i = line.indexOf('\r')) >= 0)
            line.remove(i, 1);

        QTextStream ts(line);
        ts.setCodec(qPrintable(fromCodec));
        QString encLine = ts.readAll();

        if(encLine.isEmpty())
        {
            out << line;
            continue;
        }

        bool found = false;
        foreach(QString word, wordList)
        {
            if(encLine.contains(word, Qt::CaseInsensitive))
            {
                found = true;
                break;
            }
        }
        if(found) continue;

        out << line;
    }

    f.close();

    if(!f.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate))
        return false;

    foreach(QByteArray line, out)
    {
        f.write(line);
        f.write("\n", 1);
    }
    f.close();

    return true;
}
