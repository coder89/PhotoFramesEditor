#-------------------------------------------------
#
# Project created by QtCreator 2011-07-17T21:31:06
#
#-------------------------------------------------

QT       += gui xml

TARGET = SolidBorderDrawer
TEMPLATE = lib

DEFINES += SOLIDBORDERDRAWER_LIBRARY

SOURCES += SolidBorderDrawer.cpp \
    SolidBorderDrawer_p.cpp

HEADERS += SolidBorderDrawer.h\
        SolidBorderDrawer_global.h \
    SolidBorderDrawer_p.h

symbian {
    #Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEC418737
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = SolidBorderDrawer.dll
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
