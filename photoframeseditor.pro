#-------------------------------------------------
#
# Project created by QtCreator 2011-05-11T18:24:15
#
#-------------------------------------------------

QT       += core gui

TARGET = photoframeseditor
TEMPLATE = app


SOURCES += main.cpp\
        plugin/photoframeseditor.cpp \
    widgets/abstract_photo.cpp \
    widgets/abstract_photo_resizer.cpp \
    widgets/photo_context_menu.cpp \
    widgets/polygon_widget.cpp \
    widgets/widgets_factory.cpp \
    events/canvasmouseevent.cpp \
    widgets/CanvasWidget.cpp \
    widgets/canvas/SwitchableIcon.cpp \
    widgets/canvas/LayersTree.cpp \
    widgets/CanvasCreationDialog.cpp \
    models/LayersModel.cpp \
    models/LayersModelItem.cpp \
    widgets/canvas/Scene.cpp \
    widgets/canvas/Canvas.cpp \
    models/LayersSelectionModel.cpp

HEADERS  += plugin/photoframeseditor.h \
    widgets/abstract_photo.h \
    widgets/abstract_photo_resizer.h \
    widgets/photo_context_menu.h \
    widgets/polygon_widget.h \
    widgets/widgets_factory.h \
    events/canvasmouseevent.h \
    plugin/photoframeseditor_p.h \
    utils/global.h \
    widgets/CanvasWidget.h \
    widgets/canvas/SwitchableIcon.h \
    widgets/canvas/LayersTree.h \
    widgets/dialogs/GridConfigDialog.h \
    widgets/CanvasCreationDialog.h \
    models/LayersModel.h \
    models/LayersModelItem.h \
    widgets/canvas/Scene.h \
    widgets/canvas/Canvas.h \
    models/LayersSelectionModel.h

LIBS += \
    -L/usr \
    -lkdeui \
    -lkio

INCLUDEPATH += \
    plugin \
    widgets \
    widgets/canvas \
    events \
    utils \
    models

OTHER_FILES += \
    plugin/photoframeseditorui.rc

RESOURCES += \
    resources/icons.qrc
