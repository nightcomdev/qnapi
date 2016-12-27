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

#include "subtitlelanguage.h"

SubtitleLanguage::SubtitleLanguage(QString source)
{
    fill_tables();

    setLanguage(source);
}

void SubtitleLanguage::setLanguage(QString source)
{
    idx = -1;

    if(source.length() == 2)
    {
        idx = codes2l.indexOf(source.toLower());
    }
    else if(source.length() == 3)
    {
        idx = codes3l.indexOf(source.toLower());
    }
    else
    {
        idx = names.indexOf(source);
    }   
}

QString SubtitleLanguage::toTwoLetter()
{
    if(idx > -1)
    {
        return codes2l.at(idx);
    }
    return "";
}

QString SubtitleLanguage::toTriLetter()
{
    if(idx > -1)
    {
        return codes3l.at(idx);
    }
    return "";
}

QString SubtitleLanguage::toFullName()
{
    if(idx > -1)
    {
        return names.at(idx);
    }
    return "";
}
    
QStringList SubtitleLanguage::listLanguages()
{
    return names;
}

void SubtitleLanguage::fill_tables()
{
    codes2l << "sq";
    codes3l << "alb";
    names << "Albański";

    codes2l << "en";
    codes3l << "eng";
    names << "Angielski";

    codes2l << "ar";
    codes3l << "ara";
    names << "Arabski";

    codes2l << "bg";
    codes3l << "bul";
    names << "Bułgarski";

    codes2l << "zh";
    codes3l << "chi";
    names << "Chiński";

    codes2l << "hr";
    codes3l << "hrv";
    names << "Chorwacki";

    codes2l << "cs";
    codes3l << "cze";
    names << "Czeski";

    codes2l << "da";
    codes3l << "dan";
    names << "Duński";

    codes2l << "et";
    codes3l << "est";
    names << "Estoński";

    codes2l << "fi";
    codes3l << "fin";
    names << "Fiński";

    codes2l << "fr";
    codes3l << "fre";
    names << "Francuski";

    codes2l << "gl";
    codes3l << "glg";
    names << "Galicyjski";

    codes2l << "el";
    codes3l << "ell";
    names << "Grecki";

    codes2l << "he";
    codes3l << "heb";
    names << "Hebrajski";

    codes2l << "es";
    codes3l << "spa";
    names << "Hiszpanski";

    codes2l << "nl";
    codes3l << "dut";
    names << "Holenderski";

    codes2l << "id";
    codes3l << "ind";
    names << "Indonezyjski";

    codes2l << "ja";
    codes3l << "jpn";
    names << "Japoński";

    codes2l << "ko";
    codes3l << "kor";
    names << "Koreański";

    codes2l << "mk";
    codes3l << "mac";
    names << "Macedoński";

    codes2l << "de";
    codes3l << "ger";
    names << "Niemiecki";

    codes2l << "no";
    codes3l << "nor";
    names << "Norweski";

    codes2l << "oc";
    codes3l << "oci";
    names << "Oksytański";

    codes2l << "fa";
    codes3l << "per";
    names << "Perski (farsi)";

    codes2l << "pl";
    codes3l << "pol";
    names << "Polski";

    codes2l << "pt";
    codes3l << "por";
    names << "Portugalski";

    codes2l << "pb";
    codes3l << "pob";
    names << "Portugalski-BR";

    codes2l << "ru";
    codes3l << "rus";
    names << "Rosyjski";

    codes2l << "ro";
    codes3l << "rum";
    names << "Rumuński";

    codes2l << "sr";
    codes3l << "scc";
    names << "Serbski";

    codes2l << "sl";
    codes3l << "slv";
    names << "Słoweński";

    codes2l << "sv";
    codes3l << "swe";
    names << "Szwedzki";

    codes2l << "sk";
    codes3l << "slo";
    names << "Słowacki";

    codes2l << "tr";
    codes3l << "tur";
    names << "Turecki";

    codes2l << "vi";
    codes3l << "vie";
    names << "Wietnamski";

    codes2l << "hu";
    codes3l << "hun";
    names << "Węgierski";

    codes2l << "it";
    codes3l << "ita";
    names << "Włoski";
}
