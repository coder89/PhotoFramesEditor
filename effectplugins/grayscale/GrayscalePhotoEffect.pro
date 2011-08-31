#-------------------------------------------------
#
# Project created by QtCreator 2011-07-17T11:46:38
#
#-------------------------------------------------

QT       += gui xml

TARGET = GrayscalePhotoEffectFactory
TEMPLATE = lib

DEFINES += GRAYSCALEPHOTOEFFECT_LIBRARY

SOURCES += GrayscalePhotoEffect.cpp \
    GrayscalePhotoEffect_p.cpp

HEADERS += GrayscalePhotoEffect.h\
        GrayscalePhotoEffect_global.h \
    GrayscalePhotoEffect_p.h

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE61F3A89
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = GrayscalePhotoEffect.dll
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
