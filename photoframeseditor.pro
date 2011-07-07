#-------------------------------------------------
#
# Project created by QtCreator 2011-05-11T18:24:15
#
#-------------------------------------------------

QT       += core gui console

TARGET = photoframeseditor
TEMPLATE = app


SOURCES += main.cpp\
        plugin/photoframeseditor.cpp \
    widgets/abstract_photo.cpp \
    widgets/abstract_photo_resizer.cpp \
    widgets/photo_context_menu.cpp \
    widgets/polygon_widget.cpp \
    events/canvasmouseevent.cpp \
    widgets/CanvasWidget.cpp \
    widgets/canvas/SwitchableIcon.cpp \
    widgets/CanvasCreationDialog.cpp \
    models/LayersModel.cpp \
    models/LayersModelItem.cpp \
    models/LayersSelectionModel.cpp \
    widgets/canvas/Scene.cpp \
    widgets/canvas/Canvas.cpp \
    widgets/canvas/QGraphicsSelectionItem.cpp \
    widgets/canvas/QGraphicsRotationItem.cpp \
    widgets/canvas/QGraphicsEditionWidget.cpp \
    widgets/layers/LayersTree.cpp \
    widgets/layers/LayersTreeDelegate.cpp \
    widgets/menus/SceneMenu.cpp \
    widgets/menus/GraphicsEditingWidgetMenu.cpp \
    widgets/menus/LayersTreeMenu.cpp \
    undo/UndoMoveRowsCommand.cpp \
    undo/UndoRemoveItem.cpp

HEADERS  += plugin/photoframeseditor.h \
    widgets/abstract_photo.h \
    widgets/abstract_photo_resizer.h \
    widgets/photo_context_menu.h \
    widgets/polygon_widget.h \
    events/canvasmouseevent.h \
    plugin/photoframeseditor_p.h \
    utils/global.h \
    widgets/CanvasWidget.h \
    widgets/canvas/SwitchableIcon.h \
    widgets/dialogs/GridConfigDialog.h \
    widgets/CanvasCreationDialog.h \
    models/LayersModel.h \
    models/LayersModelItem.h \
    models/LayersSelectionModel.h \
    widgets/canvas/Scene.h \
    widgets/canvas/Canvas.h \
    widgets/canvas/QGraphicsSelectionItem.h \
    widgets/canvas/QGraphicsRotationItem.h \
    widgets/canvas/QGraphicsEditionWidget.h \
    widgets/layers/LayersTree.h \
    widgets/layers/LayersTreeDelegate.h \
    widgets/menus/SceneMenu.h \
    widgets/menus/GraphicsEditingWidgetMenu.h \
    widgets/menus/LayersTreeMenu.h \
    undo/UndoMoveRowsCommand.h \
    undo/UndoRemoveItem.h \
    widgets/layers/LayersTreeTitleWidget.h

LIBS += \
    -L/usr \
    -lkdeui \
    -lkio

INCLUDEPATH += \
    plugin \
    widgets \
    widgets/canvas \
    widgets/layers \
    widgets/menus \
    events \
    utils \
    models \
    undo

OTHER_FILES += \
    resources/photoframeseditorui.rc

RESOURCES += \
    resources/icons.qrc
