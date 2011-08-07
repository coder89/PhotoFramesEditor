#-------------------------------------------------
#
# Project created by QtCreator 2011-07-17T21:31:06
#
#-------------------------------------------------

QT       += gui xml

TARGET = BlendBorderDrawer
TEMPLATE = lib

DEFINES += BLENDBORDERDRAWER_LIBRARY

SOURCES += BlendBorderDrawer.cpp \
    BlendBorderDrawer_p.cpp

HEADERS += BlendBorderDrawer.h\
        BlendBorderDrawer_global.h \
    BlendBorderDrawer_p.h

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEC418737
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = BlendBorderDrawer.dll
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
