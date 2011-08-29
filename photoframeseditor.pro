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
    widgets/photo_context_menu.cpp \
    models/LayersModel.cpp \
    models/LayersModelItem.cpp \
    models/LayersSelectionModel.cpp \
    widgets/canvas/Scene.cpp \
    widgets/canvas/Canvas.cpp \
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
    effects/PhotoEffectsGroup.cpp \
    utils/KEditFactory.cpp \
    events/UndoCommandEvent.cpp \
    events/UndoCommandEventFilter.cpp \
    effects/PhotoEffectsLoader.cpp \
    utils/global.cpp \
    widgets/items/TextItem.cpp \
    widgets/dialogs/ImageFileDialog.cpp \
    widgets/tools/TextEditorTool.cpp \
    listeners/TextColorChangeListener.cpp \
    listeners/TextFontChangeListener.cpp \
    widgets/items/AbstractItemInterface.cpp \
    borders/BordersGroup.cpp \
    borders/BorderDrawersLoader.cpp \
    borders/BorderChangeListener.cpp \
    widgets/dialogs/GridSetupDialog.cpp \
    plugin/plugin_photoframeseditor.cpp \
    widgets/canvas/SceneBackground.cpp \
    widgets/tools/CanvasEditTool.cpp \
    widgets/tools/AbstractItemsTool.cpp \
    widgets/tools/AbstractItemsListViewTool.cpp \
    widgets/extra/PatternsComboBox.cpp \
    widgets/canvas/RotationWidgetItem.cpp \
    widgets/canvas/ScalingWidgetItem.cpp \
    widgets/canvas/CropWidgetItem.cpp \
    settings/PFEConfig.cpp \
    settings/PFEConfigSkeleton.cpp \
    settings/PFEConfigViewWidget.cpp \
    widgets/tools/AbstractTool.cpp \
    widgets/canvas/MousePressListener.cpp \
    settings/PFEConfigDialog.cpp \
    widgets/canvas/CanvasSize.cpp \
    widgets/dialogs/CanvasSizeDialog.cpp

HEADERS  += plugin/photoframeseditor.h \
    widgets/photo_context_menu.h \
    plugin/photoframeseditor_p.h \
    utils/global.h \
    models/LayersModel.h \
    models/LayersModelItem.h \
    models/LayersSelectionModel.h \
    widgets/canvas/Scene.h \
    widgets/canvas/Canvas.h \
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
    effects/PhotoEffectsGroup.h \
    utils/KEditFactory.h \
    events/UndoCommandEvent.h \
    events/UndoCommandEventFilter.h \
    effects/AbstractPhotoEffectFactory.h \
    effects/AbstractPhotoEffectInterface.h \
    effects/PhotoEffectsLoader.h \
    widgets/items/TextItem.h \
    widgets/dialogs/ImageFileDialog.h \
    widgets/tools/TextEditorTool.h \
    listeners/TextColorChangeListener.h \
    listeners/TextFontChangeListener.h \
    widgets/items/AbstractItemInterface.h \
    borders/BordersGroup.h \
    models/AbstractMovableModel.h \
    borders/BorderDrawerInterface.h \
    borders/BorderDrawersLoader.h \
    borders/BorderDrawerFactoryInterface.h \
    borders/BorderChangeListener.h \
    widgets/dialogs/GridSetupDialog.h \
    plugin/plugin_photoframeseditor.h \
    widgets/canvas/SceneBackground.h \
    widgets/tools/CanvasEditTool.h \
    widgets/tools/AbstractItemsTool.h \
    widgets/tools/AbstractItemsListViewTool.h \
    widgets/tools/AbstractItemsListViewTool_p.h \
    widgets/tools/AbstractTool.h \
    widgets/canvas/MousePressListener.h \
    widgets/extra/PatternsComboBox.h \
    widgets/canvas/RotationWidgetItem.h \
    widgets/canvas/ScalingWidgetItem.h \
    widgets/canvas/CropWidgetItem.h \
    settings/PFEConfig.h \
    settings/PFEConfigSkeleton.h \
    settings/PFEConfigViewWidget.h \
    settings/PFEConfigDialog.h \
    widgets/canvas/CanvasSize.h \
    widgets/dialogs/CanvasSizeDialog.h

LIBS += \
    -L/usr \
    -lkdeui \
    -lkio \
    -lkipi \
    -lkdecore

INCLUDEPATH += \
    plugin \
    widgets \
    widgets/extra \
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
    borders \
    settings

OTHER_FILES += \
    plugin/photoframeseditorui.rc \
    settings/photoframeseditor.kcfg \
    CMakeLists.txt \
    settings/PFEConfigSkeleton.kcfgc \
    resources/photoframeseditor.desktop \
    plugin/photoframeseditor.desktop \
    resources/app/CMakeLists.txt \
    effectplugins/CMakeLists.txt \
    effectplugins/blur/CMakeLists.txt \
    effectplugins/colorize/CMakeLists.txt \
    effectplugins/greyscale/CMakeLists.txt \
    effectplugins/sepia/CMakeLists.txt \
    effectplugins/blur/photolayoutseditoreffectplugin_blur.desktop \
    effects/photolayoutseditoreffectplugin.desktop \
    effectplugins/colorize/photolayoutseditoreffectplugin_colorize.desktop \
    effectplugins/greyscale/photolayoutseditoreffectplugin_grayscale.desktop \
    effectplugins/sepia/photolayoutseditoreffectplugin_sepia.desktop

RESOURCES += \
    resources/icons.qrc
