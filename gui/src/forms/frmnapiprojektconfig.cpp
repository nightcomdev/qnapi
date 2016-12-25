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

#include "frmnapiprojektconfig.h"
#include "libqnapi.h"
#include "engines/napiprojektdownloadengine.h"

frmNapiProjektConfig::frmNapiProjektConfig(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    ui.setupUi(this);

    QIcon napiProjektIcon = QIcon(QPixmap(
      LibQNapi::subtitleDownloadEngineRegistry()->enginePixmapData(NapiProjektDownloadEngine::name)
    ));
    setWindowIcon(napiProjektIcon);

    load();

    connect(ui.pbRegister, SIGNAL(clicked()), this, SLOT(pbRegisterClicked()));

    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
}

void frmNapiProjektConfig::accept()
{
    GlobalConfig().setNick(NapiProjektDownloadEngine::name, ui.leNick->text());
    GlobalConfig().setPass(NapiProjektDownloadEngine::name, ui.lePass->text());
    QDialog::accept();
}

void frmNapiProjektConfig::pbRegisterClicked()
{
    ((QNapiApp*)qApp)->showCreateAccount(NapiProjektDownloadEngine::name);
}

void frmNapiProjektConfig::load()
{
    ui.leNick->setText(GlobalConfig().nick(NapiProjektDownloadEngine::name));
    ui.lePass->setText(GlobalConfig().pass(NapiProjektDownloadEngine::name));
}
