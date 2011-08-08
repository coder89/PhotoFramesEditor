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
    widgets/dialogs/CanvasCreationDialog.cpp \
    models/LayersModel.cpp \
    models/LayersModelItem.cpp \
    models/LayersSelectionModel.cpp \
    widgets/canvas/Scene.cpp \
    widgets/canvas/Canvas.cpp \
    widgets/canvas/QGraphicsRotationItem.cpp \
    widgets/layers/LayersTree.cpp \
    widgets/layers/LayersTreeDelegate.cpp \
    widgets/menus/SceneMenu.cpp \
    widgets/menus/GraphicsEditingWidgetMenu.cpp \
    widgets/menus/LayersTreeMenu.cpp \
    undo/UndoMoveRowsCommand.cpp \
    widgets/tools/ToolsDockWidget.cpp \
    widgets/tools/BorderEditTool.cpp \
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
    widgets/tools/TextEditorTool.cpp \
    listeners/TextColorChangeListener.cpp \
    listeners/TextFontChangeListener.cpp \
    widgets/items/AbstractItemInterface.cpp \
    borders/BordersGroup.cpp \
    widgets/tools/AbstractListTool.cpp \
    borders/BorderDrawersLoader.cpp \
    borders/BorderChangeListener.cpp \
    widgets/dialogs/GridSetupDialog.cpp

HEADERS  += plugin/photoframeseditor.h \
    widgets/abstract_photo_resizer.h \
    widgets/photo_context_menu.h \
    events/canvasmouseevent.h \
    plugin/photoframeseditor_p.h \
    utils/global.h \
    widgets/canvas/SwitchableIcon.h \
    widgets/dialogs/CanvasCreationDialog.h \
    models/LayersModel.h \
    models/LayersModelItem.h \
    models/LayersSelectionModel.h \
    widgets/canvas/Scene.h \
    widgets/canvas/Canvas.h \
    widgets/canvas/QGraphicsRotationItem.h \
    widgets/layers/LayersTree.h \
    widgets/layers/LayersTreeDelegate.h \
    widgets/menus/SceneMenu.h \
    widgets/menus/GraphicsEditingWidgetMenu.h \
    widgets/menus/LayersTreeMenu.h \
    undo/UndoMoveRowsCommand.h \
    widgets/layers/LayersTreeTitleWidget.h \
    widgets/tools/ToolsDockWidget.h \
    widgets/tools/BorderEditTool.h \
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
    widgets/tools/TextEditorTool.h \
    listeners/TextColorChangeListener.h \
    listeners/TextFontChangeListener.h \
    widgets/items/AbstractItemInterface.h \
    borders/BordersGroup.h \
    widgets/tools/AbstractListTool.h \
    widgets/tools/AbstractListTool_p.h \
    models/AbstractMovableModel.h \
    borders/BorderDrawerInterface.h \
    borders/BorderDrawersLoader.h \
    borders/BorderDrawerFactoryInterface.h \
    borders/BorderChangeListener.h \
    widgets/dialogs/GridSetupDialog.h

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
    listeners \
    utils \
    models \
    undo \
    borders

OTHER_FILES += \
    resources/photoframeseditorui.rc

RESOURCES += \
    resources/icons.qrc
