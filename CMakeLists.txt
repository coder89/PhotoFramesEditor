  # set project's name
  PROJECT( photoframeseditor )

  FIND_PACKAGE( KDE4 REQUIRED )

  INCLUDE(KDE4Defaults)

  SET( INCLUDE_DIRS
      ./
      ./plugin
      ./widgets
      ./widgets/extra
      ./widgets/canvas
      ./widgets/dialogs
      ./widgets/items
      ./widgets/layers
      ./widgets/menus
      ./widgets/tools
      ./effects
      ./events
      ./listeners
      ./utils
      ./models
      ./undo
      ./borders
  )

  INCLUDE_DIRECTORIES(${QT_INCLUDE} ${KDE4_INCLUDES} ${INCLUDE_DIRS})

  # sources
  SET( SAMPLE_SRCS
      ./main.cpp
      ./plugin/photoframeseditor.cpp
      ./widgets/photo_context_menu.cpp
      ./widgets/dialogs/CanvasCreationDialog.cpp
      ./models/LayersModel.cpp
      ./models/LayersModelItem.cpp
      ./models/LayersSelectionModel.cpp
      ./widgets/canvas/Scene.cpp
      ./widgets/canvas/Canvas.cpp
      ./widgets/layers/LayersTree.cpp
      ./widgets/layers/LayersTreeDelegate.cpp
      ./widgets/menus/SceneMenu.cpp
      ./widgets/menus/GraphicsEditingWidgetMenu.cpp
      ./widgets/menus/LayersTreeMenu.cpp
      ./undo/UndoMoveRowsCommand.cpp
      ./widgets/tools/ToolsDockWidget.cpp
      ./widgets/tools/BorderEditTool.cpp
      ./widgets/items/AbstractPhoto.cpp
      ./widgets/items/PhotoItem.cpp
      ./widgets/tools/EffectsEditorTool.cpp
      ./effects/PhotoEffectsGroup.cpp
      ./utils/KEditFactory.cpp
      ./events/UndoCommandEvent.cpp
      ./events/UndoCommandEventFilter.cpp
      ./effects/PhotoEffectsLoader.cpp
      ./utils/global.cpp
      ./widgets/items/TextItem.cpp
      ./widgets/dialogs/ImageFileDialog.cpp
      ./utils/PFEConfig.cpp
      ./widgets/tools/TextEditorTool.cpp
      ./listeners/TextColorChangeListener.cpp
      ./listeners/TextFontChangeListener.cpp
      ./widgets/items/AbstractItemInterface.cpp
      ./borders/BordersGroup.cpp
      ./borders/BorderDrawersLoader.cpp
      ./borders/BorderChangeListener.cpp
      ./widgets/dialogs/GridSetupDialog.cpp
      ./plugin/plugin_photoframeseditor.cpp
      ./widgets/canvas/SceneBackground.cpp
      ./widgets/tools/CanvasEditTool.cpp
      ./widgets/tools/AbstractItemsTool.cpp
      ./widgets/tools/AbstractItemsListViewTool.cpp
      ./widgets/extra/PatternsComboBox.cpp
      ./widgets/canvas/RotationWidgetItem.cpp
      ./widgets/canvas/ScalingWidgetItem.cpp
      ./widgets/canvas/CropWidgetItem.cpp
  )

  # moc headers
  SET( SAMPLE_MOC_HDRS
      ./plugin/photoframeseditor.h
      ./widgets/dialogs/CanvasCreationDialog.h
      ./models/LayersModel.h
      ./models/LayersModelItem.h
      ./models/LayersSelectionModel.h
      ./widgets/canvas/Scene.h
      ./widgets/canvas/Canvas.h
      ./widgets/layers/LayersTree.h
      ./widgets/layers/LayersTreeDelegate.h
      ./widgets/tools/ToolsDockWidget.h
      ./widgets/tools/BorderEditTool.h
      ./widgets/items/AbstractPhoto.h
      ./widgets/items/PhotoItem.h
      ./widgets/tools/EffectsEditorTool.h
      ./effects/PhotoEffectsGroup.h
      ./utils/KEditFactory.h
      ./events/UndoCommandEventFilter.h
      ./effects/PhotoEffectsLoader.h
      ./widgets/items/TextItem.h
      ./widgets/dialogs/ImageFileDialog.h
      ./widgets/tools/TextEditorTool.h
      ./listeners/TextColorChangeListener.h
      ./listeners/TextFontChangeListener.h
      ./borders/BordersGroup.h
      ./borders/BorderDrawersLoader.h
      ./borders/BorderChangeListener.h
      ./widgets/dialogs/GridSetupDialog.h
      ./plugin/plugin_photoframeseditor.h
      ./widgets/canvas/SceneBackground.h
      ./widgets/tools/CanvasEditTool.h
      ./widgets/tools/AbstractItemsTool.h
      ./widgets/tools/AbstractItemsListViewTool.h
      ./widgets/tools/AbstractItemsListViewTool_p.h
      ./widgets/tools/AbstractTool.h
      ./widgets/canvas/MousePressListener.h
      ./widgets/extra/PatternsComboBox.h
      ./widgets/canvas/RotationWidgetItem.h
      ./widgets/canvas/ScalingWidgetItem.h
      ./widgets/canvas/CropWidgetItem.h
  )

  # resource file
  SET( SAMPLE_RCS
       ./resources/icons.qrc
  )

  # enable warnings
  ADD_DEFINITIONS( -Wall )

  # Qt4 additional modules
  SET( QT_USE_QTXML TRUE )
  SET( QT_USE_QTCONSOLE TRUE )

  # Qt library
  FIND_PACKAGE( Qt4 REQUIRED )

  # add some useful macros and variables
  # (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
  INCLUDE( ${QT_USE_FILE} )

  # this command will generate rules that will run rcc on all files from SAMPLE_RCS
  # in result SAMPLE_RC_SRCS variable will contain paths to files produced by rcc
  QT4_ADD_RESOURCES( SAMPLE_RC_SRCS ${SAMPLE_RCS} )

  # and finally this will run moc:
  QT4_WRAP_CPP( SAMPLE_MOC_SRCS ${SAMPLE_MOC_HDRS} )

  # here we instruct CMake to build "sample" executable from all of the source files
  KDE4_ADD_EXECUTABLE( photoframeseditor ${SAMPLE_SRCS} ${SAMPLE_MOC_SRCS} ${SAMPLE_RC_SRCS} )

  # last thing we have to do is to tell CMake what libraries our executable needs,
  # luckily FIND_PACKAGE prepared QT_LIBRARIES variable for us:
  TARGET_LINK_LIBRARIES( photoframeseditor ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} ./utils/qtpropertybrowser/src/qtpropertybrowser.pri)

  KDE4_ADD_KCFG_FILES(photoframeseditor_SRCS ./resources/PFEConfigSkeleton.kcfgc)
  INSTALL( FILES ./resources/PFEConfigSkeleton.kcfgc DESTINATION ${KCFG_INSTALL_DIR} )

  INSTALL( TARGETS photoframeseditor  ${INSTALL_TARGETS_DEFAULT_ARGS} )
