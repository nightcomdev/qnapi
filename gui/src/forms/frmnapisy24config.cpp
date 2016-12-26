
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

#include "frmnapisy24config.h"
#include "libqnapi.h"
#include "engines/napisy24downloadengine.h"

frmNapisy24Config::frmNapisy24Config(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    ui.setupUi(this);

    QIcon napisy24Icon = QIcon(QPixmap(
      LibQNapi::subtitleDownloadEngineRegistry()->enginePixmapData(Napisy24DownloadEngine::metadata.name())
    ));
    setWindowIcon(napisy24Icon);


    load();

    connect(ui.pbRegister, SIGNAL(clicked()), this, SLOT(pbRegisterClicked()));

    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
}

void frmNapisy24Config::accept()
{
    GlobalConfig().setNick(Napisy24DownloadEngine::metadata.name(), ui.leNick->text());
    GlobalConfig().setPass(Napisy24DownloadEngine::metadata.name(), ui.lePass->text());
    QDialog::accept();
}

void frmNapisy24Config::pbRegisterClicked()
{
    ((QNapiApp*)qApp)->showCreateAccount(Napisy24DownloadEngine::metadata.name());
}

void frmNapisy24Config::load()
{
    ui.leNick->setText(GlobalConfig().nick(Napisy24DownloadEngine::metadata.name()));
    ui.lePass->setText(GlobalConfig().pass(Napisy24DownloadEngine::metadata.name()));
}
