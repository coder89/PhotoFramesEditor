#-------------------------------------------------
#
# Project created by QtCreator 2011-07-17T21:31:06
#
#-------------------------------------------------

QT       += gui xml

TARGET = BlurPhotoEffect
TEMPLATE = lib

DEFINES += BLURPHOTOEFFECT_LIBRARY

SOURCES += BlurPhotoEffect.cpp \
    BlurPhotoEffect_p.cpp

HEADERS += BlurPhotoEffect.h\
        BlurPhotoEffect_global.h \
    BlurPhotoEffect_p.h

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEC418737
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = BlurPhotoEffect.dll
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
