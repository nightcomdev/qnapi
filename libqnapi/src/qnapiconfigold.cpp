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

#include "libqnapi.h"
#include "qnapiconfigold.h"
#include <QCoreApplication>
#include <QSet>
#include <QFileInfo>
#include <QDir>
#include <QProcess>
#include <QRegExp>

QNapiConfigOld::QNapiConfigOld(const QString & qnapiVersion,
                         const QSharedPointer<const StaticConfig> & staticConfig)
    : qnapiVersion(qnapiVersion),
      staticConfig(staticConfig)
{
}

QNapiConfigOld::~QNapiConfigOld()
{
}

void QNapiConfigOld::load(QString appDirPath)
{
    if(settings) delete settings;

    QString localQNapiIniPath = appDirPath + QDir::separator() + "qnapi.ini";
    isPortableMode = QFileInfo(localQNapiIniPath).exists();

    if(isPortableMode)
    {
        settings = new QSettings(localQNapiIniPath, QSettings::IniFormat);
    }
    else
    {
        settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "qnapi");
    }

    performMigrations();

}

void QNapiConfigOld::save()
{
    settings->setValue("qnapi/version", qnapiVersion);
    settings->setValue("qnapi/firstrun", false);
    settings->sync();
}

void QNapiConfigOld::performMigrations()
{
    if(!version().isEmpty() && version() <= "0.2.0") {
        // between 0.2.0 and 0.2.1 there was semantic change within search_policy
        // config item; SP_SEARCH_ALL and SP_BREAK_IF_FOUND were swapped

        if(searchPolicy() == OLD_SP_SEARCH_ALL)
            setSearchPolicy(OLD_SP_BREAK_IF_FOUND);
        else if(searchPolicy() == OLD_SP_BREAK_IF_FOUND)
            setSearchPolicy(OLD_SP_SEARCH_ALL);

        save();
    }
}


bool QNapiConfigOld::firstRun()
{
    return settings->value("qnapi/firstrun", true).toBool();
}

QString QNapiConfigOld::version()
{
    return settings->value("qnapi/version", "").toString();
}

QString QNapiConfigOld::p7zipPath()
{
    // Odczytujemy z konfiguracji
    QString p7z_path = settings->value("qnapi/7z_path", "").toString();
    if(!p7z_path.isEmpty() && QFileInfo(p7z_path).isExecutable())
        return p7z_path;

#ifdef Q_OS_WIN
    QString separator = ";";
#else
    QString separator = ":";
#endif

    // Przeszukiwanie sciezek systemowych i biezacego katalogu
    QString path = QProcess::systemEnvironment().filter(QRegExp("^PATH=(.*)$")).value(0);
    QStringList paths = path.mid(5).split(separator);
    paths.removeAll("");
    paths.append(".");

    if(paths.size() == 0)
        paths << "/bin" << "/usr/bin" << "/usr/local/bin";

#ifdef Q_OS_MAC
    // Pod MacOS X 7zip jest w zasobach aplikacji
    paths << QDir(QCoreApplication::applicationDirPath() + "/../Resources").canonicalPath();
#endif

    QStringList binaries;
    binaries << "7z" << "7za";

    for(QStringList::iterator i = paths.begin(); i != paths.end(); i++)
    {
        for(QStringList::iterator j = binaries.begin(); j != binaries.end(); j++)
        {
            p7z_path = (*i) + "/" + (*j);
            if(QFileInfo(p7z_path).isExecutable())
                return QFileInfo(p7z_path).absoluteFilePath();
        }
    }


#ifdef Q_OS_WIN
    // Pod systemem Windows program 7za.exe musi byc w tym samym katalogu, co qnapi.exe
    p7z_path = QFileInfo(QCoreApplication::applicationDirPath() + "/7za.exe").absoluteFilePath();
    if(QFileInfo(p7z_path).isExecutable())
        return p7z_path;
#endif

    // Jesli wszystko inne zawiodlo...;)
    return "7z";
}

void QNapiConfigOld::setP7zipPath(const QString & path)
{
    settings->setValue("qnapi/7z_path", path);
}

QString QNapiConfigOld::tmpPath()
{
    QString tmpPath = settings->value("qnapi/tmp_path", QDir::tempPath()).toString();
    if(!QFileInfo(tmpPath).exists() || !QFileInfo(tmpPath).isWritable()) {
        if(isPortableMode) {
            tmpPath = QDir::tempPath();
        }
    }
    return tmpPath;
}

void QNapiConfigOld::setTmpPath(const QString & path)
{
    settings->setValue("qnapi/tmp_path", path);
}

QString QNapiConfigOld::nick(const QString & engine)
{
    QString nick = settings->value(engine + "/nick", "").toString();

    // Konwersja z konfigow <= 0.1.5
    if(nick.isEmpty() && (engine == "NapiProjekt"))
    {
        nick = settings->value("qnapi/nick", "").toString();

        if(!nick.isEmpty())
        {
            settings->remove("qnapi/nick");
            setNick("NapiProjekt", nick);
        }
    }

    return nick;
}

void QNapiConfigOld::setNick(const QString & engine, const QString & nick)
{
    settings->setValue(engine + "/nick", nick);
}

QString QNapiConfigOld::pass(const QString & engine)
{
    QString pass = settings->value(engine + "/pass", "").toString();

    if(pass.isEmpty() && (engine == "NapiProjekt"))
    {
        pass = settings->value("qnapi/pass", "").toString();

        if(!pass.isEmpty())
        {
            settings->remove("qnapi/pass");
            setPass("NapiProjekt", pass);
        }
    }

    return pass;
}

void QNapiConfigOld::setPass(const QString & engine, const QString & pass)
{
    settings->setValue(engine + "/pass", pass);
}

QString QNapiConfigOld::language()
{
    return settings->value("qnapi/language", "pl").toString();
}

void QNapiConfigOld::setLanguage(const QString & language)
{
    settings->setValue("qnapi/language", language);
}

QString QNapiConfigOld::languageBackup()
{
    return settings->value("qnapi/language_backup", "en").toString();
}

void QNapiConfigOld::setLanguageBackup(const QString & language)
{
    settings->setValue("qnapi/language_backup", language);
}

bool QNapiConfigOld::noBackup()
{
    return settings->value("qnapi/no_backup", false).toBool();
}

void QNapiConfigOld::setNoBackup(bool noBackup)
{
    settings->setValue("qnapi/no_backup", noBackup);
}

bool QNapiConfigOld::quietBatch()
{
#ifdef Q_OS_MAC
    return false;
#else
    return settings->value("qnapi/quiet_batch", false).toBool();
#endif
}

void QNapiConfigOld::setQuietBatch(bool quietBatch)
{
#ifndef Q_OS_MAC
    settings->setValue("qnapi/quiet_batch", quietBatch);
#else
    Q_UNUSED(quietBatch)
#endif
}

#ifdef Q_OS_MAC
bool QNapiConfigOld::showDockIcon()
{
    const bool show_default = true;
    
    QString infoPlistPath = QFileInfo(QCoreApplication::applicationDirPath() + "/../Info.plist").canonicalFilePath();

    QFile plistFile(infoPlistPath);
    
    QDomDocument doc;
    if(!doc.setContent(&plistFile))
        return show_default;

    if(!doc.hasChildNodes())
        return show_default;

    QDomNodeList nodes = doc.childNodes();
    
    QDomNode node;
    int i;
    for(i = 0; i < nodes.size(); ++i)
    {
        node = nodes.at(i);
        if(node.nodeName() == "plist")
            break;
    }
    
    if(i == nodes.size())
        return show_default;

    if(!node.hasChildNodes())
        return show_default;

    nodes = node.childNodes();
    
    for(i = 0; i < nodes.size(); ++i)
    {
        node = nodes.at(i);
        if(node.nodeName() == "dict")
            break;
    }

    if(i == nodes.size())
        return show_default;
    
    if(!node.hasChildNodes())
        return show_default;
    
    nodes = node.childNodes();

    for(i = 0; i < nodes.size(); ++i)
    {
        node = nodes.at(i);

        QString subText;

        if(node.hasChildNodes())
        {
            subText = node.childNodes().at(0).toText().data();
        }

        if(subText == "LSUIElement")
            break;
    }

    if(i < nodes.size())
    {
        node = node.nextSibling();
        return (node.nodeName() != "true");
    }

    return show_default;
    
}

void QNapiConfigOld::setShowDockIcon(bool show)
{
    QString infoPlistPath = QFileInfo(QCoreApplication::applicationDirPath() + "/../Info.plist").canonicalFilePath();
    
    QFile plistFile(infoPlistPath);
    
    QDomDocument doc;
    if(!doc.setContent(&plistFile) || !doc.hasChildNodes())
        return;
    
    QDomNodeList nodes = doc.childNodes();
    
    QDomNode node;
    int i;
    for(i = 0; i < nodes.size(); ++i)
    {
        node = nodes.at(i);
        if(node.nodeName() == "plist")
            break;
    }
    
    if((i == nodes.size()) || !node.hasChildNodes())
        return;
    
    nodes = node.childNodes();
    
    for(i = 0; i < nodes.size(); ++i)
    {
        node = nodes.at(i);
        if(node.nodeName() == "dict")
            break;
    }
    
    if((i == nodes.size()) || !node.hasChildNodes())
        return;
    
    nodes = node.childNodes();
    
    for(i = 0; i < nodes.size(); ++i)
    {
        node = nodes.at(i);
        
        QString subText;
        
        if(node.hasChildNodes())
        {
            subText = node.childNodes().at(0).toText().data();
        }
        
        if(subText == "LSUIElement")
            break;
    }
    
    if(i >= nodes.size())
        return;
    
    node = node.nextSibling();
    
    node.toElement().setTagName(show ? "false" : "true");

    QString modifiedContent = doc.toString(4);

    plistFile.close();
    
    if(!plistFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return;

    QTextStream plistStream(&plistFile);
    
    plistStream << modifiedContent;
    
    plistFile.close();
    
}

#endif

QList<QPair<QString, bool> > QNapiConfigOld::engines()
{
    QList<QVariant> inList = settings->value("qnapi/engines").toList();
    QList<QPair<QString, bool> > map;
    QSet<QString> names;

    foreach(QVariant v, inList)
    {
        QStringList sl = v.toStringList();
        if(sl.size() != 2) continue;

        QString key = sl.at(0);
        bool value = (sl.at(1) == "1");
        map << qMakePair(key, value);
        names << key;
    }

    QStringList allNames;
    allNames << "NapiProjekt" << "Napisy24" << "OpenSubtitles";

    QString name;
    foreach(name, allNames) {
        if(!names.contains(name)) {
          map << qMakePair(name, true);
        }
    }

    return map;
}

QStringList QNapiConfigOld::enginesList()
{
    QList<QPair<QString, bool> > map = engines();
    QStringList list;

    for(int i = 0; i < map.size(); ++i)
    {
        QPair<QString,bool> e = map.at(i);
        if(e.second) list << e.first;
    }

    if(list.isEmpty())
    {
        list << "NapiProjekt" << "OpenSubtitles" << "Napisy24";
    }

    return list;
}

void QNapiConfigOld::setEngines(QList<QPair<QString, bool> > engines)
{
    QList<QVariant> outList;
    for(int i = 0; i < engines.size(); ++i)
    {
        QPair<QString, bool> e = engines.at(i);

        QStringList sl;
        sl << e.first << (e.second ? "1" : "0");
        outList << sl;
    }

    settings->setValue("qnapi/engines", outList);
}

OldSearchPolicy QNapiConfigOld::searchPolicy()
{
    return (OldSearchPolicy) settings->value("qnapi/search_policy", 0).toInt();
}

void QNapiConfigOld::setSearchPolicy(OldSearchPolicy policy)
{
    settings->setValue("qnapi/search_policy", policy);
}

OldDownloadPolicy QNapiConfigOld::downloadPolicy()
{
    return (OldDownloadPolicy) settings->value("qnapi/download_policy", 1).toInt();
}

void QNapiConfigOld::setDownloadPolicy(OldDownloadPolicy policy)
{
    settings->setValue("qnapi/download_policy", policy);
}

bool QNapiConfigOld::ppEnabled()
{
    return settings->value("qnapi/post_processing", false).toBool();
}

void QNapiConfigOld::setPpEnabled(bool enable)
{
    settings->setValue("qnapi/post_processing", enable);
}

OldChangeEncodingMethod QNapiConfigOld::ppEncodingMethod() {
    if(settings->contains("qnapi/change_encoding")) {
        bool encodingEnabled = settings->value("qnapi/change_encoding", false).toBool();
        settings->remove("qnapi/change_encoding");

        OldChangeEncodingMethod method = encodingEnabled ? OLD_CEM_CHANGE : OLD_CEM_ORIGINAL;

        setPpEncodingMethod(method);
        return method;
    }

    return (OldChangeEncodingMethod) settings->value("qnapi/encoding_method", 0).toInt();
}


void QNapiConfigOld::setPpEncodingMethod(OldChangeEncodingMethod method) {
    settings->setValue("qnapi/encoding_method", method);
}

bool QNapiConfigOld::ppAutoDetectEncoding()
{
    return settings->value("qnapi/auto_detect_encoding", true).toBool();
}

void QNapiConfigOld::setPpAutoDetectEncoding(bool change)
{
    settings->setValue("qnapi/auto_detect_encoding", change);
}

QString QNapiConfigOld::ppEncodingFrom()
{
    return settings->value("qnapi/enc_from", "windows-1250").toString();
}

void QNapiConfigOld::setPpEncodingFrom(const QString & encoding)
{
    settings->setValue("qnapi/enc_from", encoding);
}

QString QNapiConfigOld::ppEncodingTo()
{
    return settings->value("qnapi/enc_to", "UTF-8").toString();
}

void QNapiConfigOld::setPpEncodingTo(const QString & encoding)
{
    settings->setValue("qnapi/enc_to", encoding);
}

bool QNapiConfigOld::ppShowAllEncodings()
{
    return settings->value("qnapi/show_all_encodings", false).toBool();
}

void QNapiConfigOld::setPpShowAllEncodings(bool show)
{
    settings->setValue("qnapi/show_all_encodings", show);
}

bool QNapiConfigOld::ppRemoveLines()
{
    return settings->value("qnapi/remove_lines", false).toBool();
}

void QNapiConfigOld::setPpRemoveLines(bool remove)
{
    settings->setValue("qnapi/remove_lines", remove);
}

QString QNapiConfigOld::ppSubFormat()
{
    return settings->value("qnapi/sub_format", "").toString();
}

void QNapiConfigOld::setPpSubFormat(const QString & subFormat)
{
    settings->setValue("qnapi/sub_format", subFormat);
}

QString QNapiConfigOld::ppSubExtension()
{
    return settings->value("qnapi/sub_ext", "").toString();
}

void QNapiConfigOld::setPpSubExtension(const QString & subExtension)
{
    settings->setValue("qnapi/sub_ext", subExtension);
}

bool QNapiConfigOld::ppSkipConvertAds()
{
    return settings->value("qnapi/skip_convert_ads", false).toBool();
}

void QNapiConfigOld::setPpSkipConvertAds(bool skip)
{
    settings->setValue("qnapi/skip_convert_ads", skip);
}

QStringList QNapiConfigOld::ppRemoveWords()
{
    QStringList defaultRemoveWords;
    defaultRemoveWords << "movie info" << "synchro ";

    return settings->value("qnapi/remove_words", defaultRemoveWords).toStringList();
}

void QNapiConfigOld::setPpRemoveWords(const QStringList & words)
{
    settings->setValue("qnapi/remove_words", words);
}

bool QNapiConfigOld::changePermissions()
{
    return settings->value("qnapi/change_permissions", false).toBool();
}

void QNapiConfigOld::setChangePermissions(bool change)
{
    settings->setValue("qnapi/change_permissions", change);
}

QString QNapiConfigOld::changePermissionsTo()
{
    bool ok;
    int perm = settings->value("qnapi/permissions", 644).toInt(&ok);
    if(!ok || perm > 777)
        perm = 644;
    QString str = QString("%1").arg(perm);
    while(str.size() < 3)
        str = QString("0") + str;
    return str;
}

void QNapiConfigOld::setChangePermissionsTo(const QString & permissions)
{
    bool ok;
    int perm = permissions.toInt(&ok);
    if(ok && perm <= 777)
    {
        QString str = QString("%1").arg(perm);
        while(str.size() < 3)
            str = QString("0") + str;
        settings->setValue("qnapi/permissions", str);
    }
}

QString QNapiConfigOld::previousDialogPath()
{
    QString path = settings->value("qnapi/prev_dialog_path", "").toString();
    return (QDir(path).exists() ? path : QDir::homePath());
}

void QNapiConfigOld::setPreviousDialogPath(const QString & path)
{
    settings->setValue("qnapi/prev_dialog_path", path);
}

QStringList QNapiConfigOld::scanFilters()
{
    QStringList defaultScanFilters;

    defaultScanFilters << staticConfig->movieExtensionsFilter() << "*.*";

    return settings->value("scan/filters", defaultScanFilters).toStringList();
}

void QNapiConfigOld::setScanFilters(const QStringList & filters)
{
    settings->setValue("scan/filters", filters);
}

QString QNapiConfigOld::scanSkipFilters()
{
    return settings->value("scan/skip_filters", "PL dubbing").toString();
}

void QNapiConfigOld::setScanSkipFilters(const QString & filters)
{
    settings->setValue("scan/skip_filters", filters);
}

bool QNapiConfigOld::scanSkipIfSubtitlesExists()
{
    return settings->value("scan/skip_if_subtitles_exists", false).toBool();
}

void QNapiConfigOld::setScanSkipIfSubtitlesExists(bool skip)
{
    settings->setValue("scan/skip_if_subtitles_exists", skip);
}

QString QNapiConfigOld::lastScanDir()
{
    return settings->value("scan/last_scan_dir", "").toString();
}

void QNapiConfigOld::setLastScanDir(const QString & dir)
{
    settings->setValue("scan/last_scan_dir", dir);
}

QNapiConfigOld & OldGlobalConfig()
{
    static QNapiConfigOld cfg(LibQNapi::version(),
                           LibQNapi::staticConfig());
    return cfg;
}
