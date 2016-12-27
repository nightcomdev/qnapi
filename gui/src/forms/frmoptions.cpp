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

#include "frmoptions.h"
#include "qnapiapp.h"

#include "engines/napiprojektdownloadengine.h"
#include "engines/opensubtitlesdownloadengine.h"
#include "engines/napisy24downloadengine.h"

#include "forms/frmnapiprojektconfig.h"
#include "forms/frmopensubtitlesconfig.h"
#include "forms/frmnapisy24config.h"

#include "libqnapi.h"

frmOptions::frmOptions(QWidget * parent, Qt::WindowFlags f)
    : QDialog(parent, f),
      subtitleFormatsRegistry(LibQNapi::subtitleFormatsRegistry()),
      enginesRegistry(LibQNapi::subtitleDownloadEngineRegistry())
{
    ui.setupUi(this);

#ifdef Q_OS_MAC
    ui.cbQuietBatch->hide();
#endif

#ifndef Q_OS_MAC
    ui.cbShowDockIcon->hide();
#endif


#ifdef Q_OS_WIN
    // Pod Windowsami chowamy kontrolki odpowiadajace za zmiane uprawnien - i tak sie nie przydadza
    ui.cbChangePermissions->hide();
    ui.sbOPerm->hide();
    ui.sbGPerm->hide();
    ui.sbUPerm->hide();
#endif

    QString tlcode;
    ui.cbLangBackup->addItem("Brak", QVariant(""));
    foreach(QString lang, SubtitleLanguage("").listLanguages())
    {
        tlcode = SubtitleLanguage(lang).toTwoLetter();
        ui.cbLang->addItem(QIcon(QString(":/languages/%1.png").arg(tlcode)),lang,QVariant(tlcode));
        ui.cbLangBackup->addItem(QIcon(QString(":/languages/%1.png").arg(tlcode)),lang,QVariant(tlcode));
    }

    setAttribute(Qt::WA_QuitOnClose, false);

    connect(ui.le7zPath, SIGNAL(textChanged(const QString &)), this, SLOT(le7zPathChanged()));
    connect(ui.pb7zPathSelect, SIGNAL(clicked()), this, SLOT(select7zPath()));
    connect(ui.leTmpPath, SIGNAL(textChanged(const QString &)), this, SLOT(leTmpPathChanged()));
    connect(ui.pbTmpPathSelect, SIGNAL(clicked()), this, SLOT(selectTmpPath()));

    connect(ui.twEngines, SIGNAL(itemSelectionChanged()), this, SLOT(twEnginesSelectionChanged()));
    connect(ui.twEngines, SIGNAL(itemChanged(QTableWidgetItem *)), this, SLOT(twEnginesItemChanged(QTableWidgetItem *)));

    connect(ui.pbMoveUp, SIGNAL(clicked()), this, SLOT(pbMoveUpClicked()));
    connect(ui.pbMoveDown, SIGNAL(clicked()), this, SLOT(pbMoveDownClicked()));
    connect(ui.pbEngineConf, SIGNAL(clicked()), this, SLOT(pbEngineConfClicked()));
    connect(ui.pbEngineInfo, SIGNAL(clicked()), this, SLOT(pbEngineInfoClicked()));

    connect(ui.cbSubFormat, SIGNAL(currentIndexChanged(int)), this, SLOT(subFormatChanged(int)));
    connect(ui.cbEncodingMethod, SIGNAL(currentIndexChanged(int)), this, SLOT(encodingMethodChanged(int)));
    connect(ui.cbAutoDetectEncoding, SIGNAL(clicked()), this, SLOT(autoDetectEncodingClicked()));
    connect(ui.cbShowAllEncodings, SIGNAL(clicked()), this, SLOT(showAllEncodingsClicked()));

    connect(ui.pbRestoreDefaults, SIGNAL(clicked()), this, SLOT(restoreDefaults()));

    showAllEncodings();

    foreach(QString format, subtitleFormatsRegistry->listFormatNames())
    {
        ui.cbSubFormat->addItem(format);
    }

    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
}

frmOptions::~frmOptions()
{
}

void frmOptions::le7zPathChanged()
{
    QFileInfo f(ui.le7zPath->text());
    ui.le7zPath->setStyleSheet(
        f.isFile() && f.isExecutable()
            ? ""
            : "color:red;"
        );
}

void frmOptions::select7zPath()
{
    QString path7z = QFileDialog::getOpenFileName(this, tr("Wskaż ścieżkę do programu 7z"),
                                                    QFileInfo(ui.le7zPath->text()).path());
    if(!path7z.isEmpty())
    {
        if(!QFileInfo(path7z).isExecutable())
            QMessageBox::warning(this, tr("Niepoprawna ścieżka"),
                tr("Wskazana przez Ciebie ścieżka do programu 7z jest niepoprawna. Jeśli nie możesz "
                    "odnaleźć programu 7z, spróbuj zainstalować pakiet p7zip-full."));
        else
            ui.le7zPath->setText(path7z);
    }
}

void frmOptions::leTmpPathChanged()
{
    QFileInfo f(ui.leTmpPath->text());
    ui.leTmpPath->setStyleSheet(
        f.isDir() && f.isWritable()
            ? ""
            : "color:red;"
        );
}

void frmOptions::selectTmpPath()
{
    QString tmpDir = QFileDialog::getExistingDirectory(this,
                                    tr("Wskaż katalog tymczasowy"),
                                    QFileInfo(ui.leTmpPath->text()).path(),
                                    QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(!tmpDir.isEmpty())
        ui.leTmpPath->setText(QFileInfo(tmpDir).path());
}

void frmOptions::twEnginesSelectionChanged()
{
    if(ui.twEngines->selectedItems().size() < 1)
        return; 

    int currentRow = ui.twEngines->row(ui.twEngines->selectedItems().at(0));

    ui.pbMoveUp->setEnabled(currentRow > 0);
    ui.pbMoveDown->setEnabled(currentRow < ui.twEngines->rowCount() - 1);
    ui.pbEngineConf->setEnabled(true);
    ui.pbEngineInfo->setEnabled(true);
}

void frmOptions::twEnginesItemChanged(QTableWidgetItem * item)
{
    bool foundActive = false;

    for(int i = 0; i < ui.twEngines->rowCount(); ++i)
    {
        if(!ui.twEngines->item(i, 0))
        {
            return;
        }

        if(ui.twEngines->item(i, 0)->checkState() == Qt::Checked)
        {
            foundActive = true;
            break;
        }
    }

    if(!foundActive)
    {
        item->setCheckState(Qt::Checked);
        QMessageBox::warning(this,
                            "Ostrzeżenie",
                            "Przynajmniej jeden moduł pobierania musi pozostać aktywny!");
    }
    
}

void frmOptions::pbMoveUpClicked()
{
    int currentRow = ui.twEngines->row(ui.twEngines->selectedItems().at(0));

    QTableWidgetItem *current, *above;
    current = ui.twEngines->item(currentRow, 0);
    above = ui.twEngines->item(currentRow - 1, 0);

    QTableWidgetItem tmp = *current;
    *current = *above;
    *above = tmp;

    ui.twEngines->selectRow(currentRow - 1);
}

void frmOptions::pbMoveDownClicked()
{
    int currentRow = ui.twEngines->row(ui.twEngines->selectedItems().at(0));

    QTableWidgetItem *current, *below;
    current = ui.twEngines->item(currentRow, 0);
    below = ui.twEngines->item(currentRow + 1, 0);

    QTableWidgetItem tmp = *current;
    *current = *below;
    *below = tmp;

    ui.twEngines->selectRow(currentRow + 1);
}

void frmOptions::pbEngineConfClicked()
{
    QString engineName = ui.twEngines->selectedItems().at(0)->text();

    if(engineName == NapiProjektDownloadEngine::metadata.name()) {
        frmNapiProjektConfig config(this);
        config.exec();
    } else if(engineName == OpenSubtitlesDownloadEngine::metadata.name()) {
        frmOpenSubtitlesConfig config(this);
        config.exec();
    } else if(engineName == Napisy24DownloadEngine::metadata.name()) {
        frmNapisy24Config config(this);
        config.exec();
    }
}

void frmOptions::pbEngineInfoClicked()
{
    QString engineName = ui.twEngines->selectedItems().at(0)->text();
    QString engineDescription = enginesRegistry->engineMetadata(engineName).description();

    QMessageBox::information(this,
                             QString("Informacje o silniku %1").arg(engineName),
                             engineDescription);
}

void frmOptions::subFormatChanged(int format)
{
    if(format == 0)
    {
        ui.cbSubExtension->setItemText(0, tr("Domyślne"));
    }
    else
    {
        QString targetFormatName = ui.cbSubFormat->currentText();
        QSharedPointer<const SubtitleFormat> targetSF = subtitleFormatsRegistry->select(targetFormatName);
        QString targetDefaultExt = targetSF->defaultExtension();
        ui.cbSubExtension->setItemText(0, tr("Domyślne (%1)").arg(targetDefaultExt));
    }
}

void frmOptions::encodingMethodChanged(int method)
{
    OldChangeEncodingMethod cemMethod = (OldChangeEncodingMethod) method;
    bool enableEncodingSettings = cemMethod == OLD_CEM_CHANGE;
    ui.cbEncFrom->setEnabled(enableEncodingSettings);
    ui.lbConvert->setEnabled(enableEncodingSettings);
    ui.lbConvertFrom->setEnabled(enableEncodingSettings);
    ui.cbEncTo->setEnabled(enableEncodingSettings);
    ui.cbAutoDetectEncoding->setEnabled(enableEncodingSettings);
    ui.cbShowAllEncodings->setEnabled(enableEncodingSettings);
    autoDetectEncodingClicked();
}

void frmOptions::autoDetectEncodingClicked()
{
    bool checkedCE = (OldChangeEncodingMethod) ui.cbEncodingMethod->currentIndex() == OLD_CEM_CHANGE;
    bool checkedADE = ui.cbAutoDetectEncoding->isChecked();
    ui.cbEncFrom->setEnabled(checkedCE && !checkedADE);
    ui.lbConvertFrom->setEnabled(checkedCE && !checkedADE);
}

void frmOptions::showAllEncodingsClicked()
{
    QString encFrom = ui.cbEncFrom->currentText();
    QString encTo = ui.cbEncTo->currentText();

    if(ui.cbShowAllEncodings->isChecked())
    {
        showAllEncodings();
    }
    else
    {
        ui.cbEncFrom->clear();
        ui.cbEncTo->clear();

        QStringList codecs;
        codecs << "windows-1250" << "windows-1257" << "ISO-8859-2" << "ISO-8859-13"
                << "ISO-8859-16" << "UTF-8";

        ui.cbEncFrom->addItems(codecs);
        ui.cbEncTo->addItems(codecs);
    }

    ui.cbEncFrom->setCurrentIndex(ui.cbEncFrom->findText(encFrom));
    ui.cbEncTo->setCurrentIndex(ui.cbEncTo->findText(encTo));
}

void frmOptions::showAllEncodings()
{
    ui.cbEncFrom->clear();
    ui.cbEncTo->clear();

    QList<QByteArray> codecs = QTextCodec::availableCodecs();
    qSort(codecs.begin(), codecs.end());
    for(QList<QByteArray>::iterator i = codecs.begin(); i != codecs.end(); i++)
    {
        ui.cbEncFrom->addItem(*i);
        ui.cbEncTo->addItem(*i);
    }
}

void frmOptions::writeConfig()
{
    OldGlobalConfig().setP7zipPath(ui.le7zPath->text());
    OldGlobalConfig().setTmpPath(ui.leTmpPath->text());
    OldGlobalConfig().setLanguage(ui.cbLang->itemData(ui.cbLang->currentIndex()).toString());
    OldGlobalConfig().setLanguageBackup(ui.cbLangBackup->itemData(ui.cbLangBackup->currentIndex()).toString());
    OldGlobalConfig().setNoBackup(ui.cbNoBackup->isChecked());
#ifndef Q_OS_MAC
    GlobalConfig().setQuietBatch(ui.cbQuietBatch->isChecked());
#endif

#ifdef Q_OS_MAC
    OldGlobalConfig().setShowDockIcon(ui.cbShowDockIcon->isChecked());
#endif

    QList<QPair<QString, bool> > engines;
    for(int i = 0; i < ui.twEngines->rowCount(); ++i)
    {
        engines << qMakePair(ui.twEngines->item(i, 0)->text(),
                            (ui.twEngines->item(i, 0)->checkState() == Qt::Checked));
    }

    OldGlobalConfig().setEngines(engines);
    
    OldGlobalConfig().setSearchPolicy((OldSearchPolicy) ui.cbSearchPolicy->currentIndex());
    OldGlobalConfig().setDownloadPolicy((OldDownloadPolicy) ui.cbDownloadPolicy->currentIndex());

    OldGlobalConfig().setPpEnabled(ui.gbPpEnable->isChecked());
    OldGlobalConfig().setPpEncodingMethod((OldChangeEncodingMethod) ui.cbEncodingMethod->currentIndex());
    OldGlobalConfig().setPpAutoDetectEncoding(ui.cbAutoDetectEncoding->isChecked());
    OldGlobalConfig().setPpEncodingFrom(ui.cbEncFrom->currentText());
    OldGlobalConfig().setPpEncodingTo(ui.cbEncTo->currentText());
    OldGlobalConfig().setPpShowAllEncodings(ui.cbShowAllEncodings->isChecked());
    OldGlobalConfig().setPpRemoveLines(ui.cbRemoveLines->isChecked());
    OldGlobalConfig().setPpRemoveWords(ui.teRemoveWords->toPlainText().split("\n"));
    QString targetFormat = ui.cbSubFormat->currentIndex() == 0 ? "" : subtitleFormatsRegistry->listFormatNames().at(ui.cbSubFormat->currentIndex() - 1);
    OldGlobalConfig().setPpSubFormat(targetFormat);
    QString targetExt = ui.cbSubExtension->currentIndex() == 0 ? "" : ui.cbSubExtension->currentText();
    OldGlobalConfig().setPpSubExtension(targetExt);
    OldGlobalConfig().setPpSkipConvertAds(ui.cbSkipConvertAds->isChecked());
    OldGlobalConfig().setChangePermissions(ui.cbChangePermissions->isChecked());

    QString permissions = QString("%1%2%3").arg(ui.sbUPerm->value())
                                           .arg(ui.sbGPerm->value())
                                           .arg(ui.sbOPerm->value());
    OldGlobalConfig().setChangePermissionsTo(permissions);

    OldGlobalConfig().save();
}

void frmOptions::readConfig()
{
    OldGlobalConfig().load(QCoreApplication::applicationDirPath());

    ui.le7zPath->setText(OldGlobalConfig().p7zipPath());
    ui.leTmpPath->setText(OldGlobalConfig().tmpPath());
    ui.cbLang->setCurrentIndex(ui.cbLang->findData(SubtitleLanguage(OldGlobalConfig().language()).toTwoLetter()));
    ui.cbLangBackup->setCurrentIndex(ui.cbLangBackup->findData(SubtitleLanguage(OldGlobalConfig().languageBackup()).toTwoLetter()));
    ui.cbNoBackup->setChecked(OldGlobalConfig().noBackup());
#ifndef Q_OS_MAC
    ui.cbQuietBatch->setChecked(GlobalConfig().quietBatch());
#endif
#ifdef Q_OS_MAC
    ui.cbShowDockIcon->setChecked(OldGlobalConfig().showDockIcon());
#endif

    ui.twEngines->clear();

    QList<QPair<QString,bool> > engines = OldGlobalConfig().engines();
    ui.twEngines->setColumnCount(1);
    ui.twEngines->setRowCount(engines.size());

    for(int i = 0; i < engines.size(); ++i)
    {
        QPair<QString,bool> e = engines.at(i);
        QIcon engineIcon = QIcon(QPixmap(enginesRegistry->enginePixmapData(e.first)));
        QTableWidgetItem *item = new QTableWidgetItem(engineIcon, e.first);
        item->setCheckState(e.second ? Qt::Checked : Qt::Unchecked);
        ui.twEngines->setItem(i, 0, item);
    }

    ui.twEngines->horizontalHeader()->hide();
    ui.twEngines->verticalHeader()->hide();
    ui.twEngines->verticalHeader()->setDefaultSectionSize(20);
    ui.twEngines->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    ui.twEngines->setColumnWidth(0, 300);

    ui.cbSearchPolicy->setCurrentIndex(OldGlobalConfig().searchPolicy());
    ui.cbDownloadPolicy->setCurrentIndex(OldGlobalConfig().downloadPolicy());

    ui.cbEncodingMethod->setCurrentIndex(OldGlobalConfig().ppEncodingMethod());
    ui.cbAutoDetectEncoding->setChecked(OldGlobalConfig().ppAutoDetectEncoding());
    ui.cbEncFrom->setCurrentIndex(ui.cbEncFrom->findText(OldGlobalConfig().ppEncodingFrom()));
    ui.cbEncTo->setCurrentIndex(ui.cbEncTo->findText(OldGlobalConfig().ppEncodingTo()));
    ui.cbShowAllEncodings->setChecked(OldGlobalConfig().ppShowAllEncodings());
    ui.cbRemoveLines->setChecked(OldGlobalConfig().ppRemoveLines());
    ui.teRemoveWords->setText(OldGlobalConfig().ppRemoveWords().join("\n"));

    int formatIdx = 1;
    foreach(QString format, subtitleFormatsRegistry->listFormatNames())
    {
        if(OldGlobalConfig().ppSubFormat() == format)
        {
            ui.cbSubFormat->setCurrentIndex(formatIdx);
        }
        ++formatIdx;
    }

    ui.cbSubExtension->setCurrentText(OldGlobalConfig().ppSubExtension());
    ui.cbSkipConvertAds->setChecked(OldGlobalConfig().ppSkipConvertAds());

    ui.cbChangePermissions->setChecked(OldGlobalConfig().changePermissions());
    QString permissions = OldGlobalConfig().changePermissionsTo();
    unsigned short o, g, u;
    o = permissions.at(0).toLatin1() - '0';
    g = permissions.at(1).toLatin1() - '0';
    u = permissions.at(2).toLatin1() - '0';
    ui.sbUPerm->setValue((o <= 7) ? o : 6);
    ui.sbGPerm->setValue((g <= 7) ? g : 4);
    ui.sbOPerm->setValue((u <= 7) ? u : 4);

    encodingMethodChanged((int) OldGlobalConfig().ppEncodingMethod());
    showAllEncodingsClicked();

    ui.gbPpEnable->setChecked(OldGlobalConfig().ppEnabled());
}

void frmOptions::restoreDefaults()
{
    OldGlobalConfig().setP7zipPath("");
    OldGlobalConfig().setTmpPath(QDir::tempPath());
    OldGlobalConfig().setLanguage("pl");
    OldGlobalConfig().setLanguageBackup("en");
    OldGlobalConfig().setNoBackup(false);
    OldGlobalConfig().setQuietBatch(false);
    OldGlobalConfig().setChangePermissions(false);
    OldGlobalConfig().setChangePermissionsTo("644");

#ifdef Q_OS_MAC
    OldGlobalConfig().setShowDockIcon(true);
#endif

    QList<QPair<QString, bool> > engines;
    engines << QPair<QString, bool>("NapiProjekt", true)
            << QPair<QString, bool>("OpenSubtitles", true)
            << QPair<QString, bool>("Napisy24", true);
    OldGlobalConfig().setEngines(engines);
    OldGlobalConfig().setSearchPolicy(OLD_SP_SEARCH_ALL);
    OldGlobalConfig().setDownloadPolicy(OLD_DP_SHOW_LIST_IF_NEEDED);

    OldGlobalConfig().setPpEnabled(false);
    OldGlobalConfig().setPpEncodingMethod(OLD_CEM_ORIGINAL);
    OldGlobalConfig().setPpAutoDetectEncoding(true);
    OldGlobalConfig().setPpEncodingFrom("windows-1250");
    OldGlobalConfig().setPpEncodingTo("UTF-8");
    OldGlobalConfig().setPpShowAllEncodings(false);
    OldGlobalConfig().setPpRemoveLines(false);
    QStringList words;
    words << "movie info" << "synchro";
    OldGlobalConfig().setPpRemoveWords(words);
    OldGlobalConfig().setPpSubFormat("");
    OldGlobalConfig().setPpSubExtension("");
    OldGlobalConfig().setPpSkipConvertAds(false);

    OldGlobalConfig().save();

    readConfig();
}




