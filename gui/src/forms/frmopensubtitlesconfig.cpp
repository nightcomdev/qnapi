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

#include "frmopensubtitlesconfig.h"
#include "qnapiconfig.h"
#include "qnapiapp.h"
#include "libqnapi.h"
#include "engines/opensubtitlesdownloadengine.h"

frmOpenSubtitlesConfig::frmOpenSubtitlesConfig(QWidget *parent, Qt::WindowFlags f)
    : QDialog(parent, f)
{
    ui.setupUi(this);

    QIcon openSubtitlesIcon = QIcon(QPixmap(
      LibQNapi::subtitleDownloadEngineRegistry()->enginePixmapData(OpenSubtitlesDownloadEngine::metadata.name())
    ));
    setWindowIcon(openSubtitlesIcon);

    load();

    connect(ui.pbRegister, SIGNAL(clicked()), this, SLOT(pbRegisterClicked()));

    QRect position = frameGeometry();
    position.moveCenter(QDesktopWidget().availableGeometry().center());
    move(position.topLeft());
}

void frmOpenSubtitlesConfig::accept()
{
    GlobalConfig().setNick(OpenSubtitlesDownloadEngine::metadata.name(), ui.leNick->text());
    GlobalConfig().setPass(OpenSubtitlesDownloadEngine::metadata.name(), ui.lePass->text());
    QDialog::accept();
}

void frmOpenSubtitlesConfig::pbRegisterClicked()
{
    ((QNapiApp*)qApp)->showCreateAccount(OpenSubtitlesDownloadEngine::metadata.name());
}

void frmOpenSubtitlesConfig::load()
{
    ui.leNick->setText(GlobalConfig().nick(OpenSubtitlesDownloadEngine::metadata.name()));
    ui.lePass->setText(GlobalConfig().pass(OpenSubtitlesDownloadEngine::metadata.name()));
}
