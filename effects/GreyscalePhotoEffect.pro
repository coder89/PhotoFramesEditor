#-------------------------------------------------
#
# Project created by QtCreator 2011-07-17T11:46:38
#
#-------------------------------------------------

QT       += gui

TARGET = GreyscalePhotoEffectFactory
TEMPLATE = lib

DEFINES += GREYSCALEPHOTOEFFECT_LIBRARY

SOURCES += GreyscalePhotoEffect.cpp \
    GreyscalePhotoEffect_p.cpp

HEADERS += GreyscalePhotoEffect.h\
        GreyscalePhotoEffect_global.h \
    GreyscalePhotoEffect_p.h

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE61F3A89
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = GreyscalePhotoEffect.dll
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
