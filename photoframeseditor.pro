#-------------------------------------------------
#
# Project created by QtCreator 2011-05-11T18:24:15
#
#-------------------------------------------------

include(utils/qtpropertybrowser/src/qtpropertybrowser.pri)

QT       += core gui console xml

TARGET = photoframeseditor
TEMPLATE = app


SOURCES += main.cpp\
        plugin/photoframeseditor.cpp \
    widgets/abstract_photo_resizer.cpp \
    widgets/photo_context_menu.cpp \
    events/canvasmouseevent.cpp \
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
    undo/UndoRemoveItem.cpp \
    widgets/tools/ToolsDockWidget.cpp \
    widgets/tools/BorderEditTool.cpp \
    undo/UndoBorderChangeCommand.cpp \
    widgets/items/AbstractPhoto.cpp \
    widgets/items/PhotoItem.cpp \
    widgets/tools/EffectsEditorTool.cpp \
    widgets/tools/AbstractTool.cpp \
    effects/PhotoEffectsGroup.cpp \
    utils/KEditFactory.cpp \
    events/UndoCommandEvent.cpp \
    events/UndoCommandEventFilter.cpp \
    effects/PhotoEffectsLoader.cpp \
    utils/global.cpp \
    widgets/items/TextItem.cpp \
    widgets/dialogs/ImageFileDialog.cpp \
    utils/PFESettings.cpp \
    widgets/tools/TextEditorTool.cpp

HEADERS  += plugin/photoframeseditor.h \
    widgets/abstract_photo_resizer.h \
    widgets/photo_context_menu.h \
    events/canvasmouseevent.h \
    plugin/photoframeseditor_p.h \
    utils/global.h \
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
    widgets/layers/LayersTreeTitleWidget.h \
    widgets/tools/ToolsDockWidget.h \
    widgets/tools/BorderEditTool.h \
    undo/UndoBorderChangeCommand.h \
    widgets/items/AbstractPhoto.h \
    widgets/items/PhotoItem.h \
    widgets/tools/EffectsEditorTool.h \
    widgets/tools/AbstractTool.h \
    effects/PhotoEffectsGroup.h \
    utils/KEditFactory.h \
    events/UndoCommandEvent.h \
    events/UndoCommandEventFilter.h \
    effects/AbstractPhotoEffectFactory.h \
    effects/AbstractPhotoEffectInterface.h \
    effects/PhotoEffectsLoader.h \
    widgets/items/TextItem.h \
    widgets/dialogs/ImageFileDialog.h \
    utils/PFESettings.h \
    widgets/tools/TextEditorTool.h

LIBS += \
    -L/usr \
    -lkdeui \
    -lkio

INCLUDEPATH += \
    plugin \
    widgets \
    widgets/canvas \
    widgets/dialogs \
    widgets/items \
    widgets/layers \
    widgets/menus \
    widgets/tools \
    effects \
    events \
    utils \
    models \
    undo

OTHER_FILES += \
    resources/photoframeseditorui.rc

RESOURCES += \
    resources/icons.qrc
