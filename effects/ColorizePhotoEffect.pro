#-------------------------------------------------
#
# Project created by QtCreator 2011-07-17T11:46:38
#
#-------------------------------------------------

QT       += gui

TARGET = ColorizePhotoEffectFactory
TEMPLATE = lib

DEFINES += COLORIZEPHOTOEFFECT_LIBRARY

SOURCES += ColorizePhotoEffect.cpp \
    ColorizePhotoEffect_p.cpp

HEADERS += ColorizePhotoEffect.h\
        ColorizePhotoEffect_global.h \
    ColorizePhotoEffect_p.h

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE61F3A89
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ColorizePhotoEffect.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
    }
    INSTALLS += target
}
