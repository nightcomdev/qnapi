QT += network xml
QT -= gui

TARGET = qnapi
TEMPLATE = lib

CONFIG += staticlib \
          warn_on \
          silent \
          c++11

DESTDIR = $$PWD

INCLUDEPATH += $$PWD/src

include(../deps/libmaia/maia.pri)
include(../deps/qt-maybe/qt-maybe.pri)

UI_DIR = tmp
MOC_DIR = tmp
RCC_DIR = tmp
OBJECTS_DIR = tmp

SOURCES += src/libqnapi.cpp \
    src/movieinfo/libmediainfomovieinfoprovider.cpp \
    src/subconvert/subtitleformatsregistry.cpp \
    src/subconvert/subtitleconverter.cpp \
    src/subconvert/formats/microdvd.cpp \
    src/subconvert/formats/tmplayer.cpp \
    src/subconvert/formats/subrip.cpp \
    src/subconvert/formats/mpl2.cpp \
    src/subconvert/subtitleformat.cpp \
    src/utils/encodingutils.cpp \
    src/utils/synchttp.cpp \
    src/utils/syncxmlrpc.cpp \
    src/config/staticconfig.cpp \
    src/config/configreader.cpp \
    src/config/configwriter.cpp \
    src/config/generalconfig.cpp \
    src/config/scanconfig.cpp \
    src/config/postprocessingconfig.cpp \
    src/engines/subtitledownloadengine.cpp \
    src/engines/napiprojektdownloadengine.cpp \
    src/engines/napisy24downloadengine.cpp \
    src/engines/opensubtitlesdownloadengine.cpp \
    src/engines/subtitledownloadenginesregistry.cpp \
    src/utils/p7zipdecoder.cpp \
    src/qnapiconfigold.cpp \
    src/config/qnapiconfig.cpp \
    src/subtitlematcher.cpp \
    src/subtitlepostprocessor.cpp \
    src/subtitlelanguage.cpp \
    src/subtitleinfo.cpp \
    src/qnapisubtitledownloader.cpp


HEADERS += src/libqnapi.h \
    src/version.h \
    src/movieinfo/movieinfo.h \
    src/movieinfo/movieinfoprovider.h \
    src/movieinfo/libmediainfomovieinfoprovider.h \
    src/subconvert/subfile.h \
    src/subconvert/subtitleformat.h \
    src/subconvert/formats/microdvd.h \
    src/subconvert/subtitleformatsregistry.h \
    src/subconvert/subtitleconverter.h \
    src/subconvert/formats/tmplayer.h \
    src/subconvert/formats/subrip.h \
    src/subconvert/formats/mpl2.h \
    src/utils/encodingutils.h \
    src/utils/synchttp.h \
    src/utils/syncxmlrpc.h \
    src/config/staticconfig.h \
    src/config/qnapiconfig.h \
    src/config/generalconfig.h \
    src/config/postprocessingconfig.h \
    src/config/scanconfig.h \
    src/config/engineconfig.h \
    src/config/configreader.h \
    src/config/configwriter.h \
    src/engines/subtitledownloadengine.h \
    src/engines/napiprojektdownloadengine.h \
    src/engines/napisy24downloadengine.h \
    src/engines/opensubtitlesdownloadengine.h \
    src/engines/subtitledownloadenginesregistry.h \
    src/utils/p7zipdecoder.h \
    src/engines/subtitledownloadenginemetadata.h \
    src/qnapiconfigold.h \
    src/subtitlematcher.h \
    src/subtitlepostprocessor.h \
    src/subtitlelanguage.h \
    src/subtitleinfo.h \
    src/qnapisubtitledownloader.h

unix:!macx {
    PKGCONFIG += libmediainfo
}

macx {
    INCLUDEPATH += ../deps/libmediainfo/include
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.7
    QMAKE_CXXFLAGS_X86_64 = -mmacosx-version-min=10.7
}

win32 {
    INCLUDEPATH += ../deps/libmediainfo/include
}
