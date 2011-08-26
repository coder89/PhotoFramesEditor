  PROJECT( photoframeseditor )

  CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

  ADD_SUBDIRECTORY( ./utils/qtpropertybrowser )

  FIND_PACKAGE(Qt4 REQUIRED)
  FIND_PACKAGE(KDE4 REQUIRED)
  FIND_PACKAGE(Kipi REQUIRED)

  ADD_DEFINITIONS( -Wall )
  ADD_DEFINITIONS(${QT_DEFINITIONS})

  SET( CMAKE_PACKAGE_QTGUI TRUE )
  SET( QT_USE_QTXML TRUE )

  INCLUDE( ${QT_USE_FILE} )
  INCLUDE( KDE4Defaults )

  SET( INCLUDE_DIRS
      plugin
      widgets
      widgets/extra
      widgets/canvas
      widgets/dialogs
      widgets/items
      widgets/layers
      widgets/menus
      widgets/tools
      effects
      events
      listeners
      utils
      utils/qtpropertybrowser/src
      models
      undo
      borders
      settings
  )

  SET( PhotoFramesEditor_SRCS
      plugin/photoframeseditor.cpp
      widgets/photo_context_menu.cpp
      widgets/dialogs/CanvasCreationDialog.cpp
      models/LayersModel.cpp
      models/LayersModelItem.cpp
      models/LayersSelectionModel.cpp
      widgets/canvas/Scene.cpp
      widgets/canvas/Canvas.cpp
      widgets/layers/LayersTree.cpp
      widgets/layers/LayersTreeDelegate.cpp
      widgets/menus/SceneMenu.cpp
      widgets/menus/GraphicsEditingWidgetMenu.cpp
      widgets/menus/LayersTreeMenu.cpp
      undo/UndoMoveRowsCommand.cpp
      widgets/tools/ToolsDockWidget.cpp
      widgets/tools/BorderEditTool.cpp
      widgets/items/AbstractPhoto.cpp
      widgets/items/PhotoItem.cpp
      widgets/tools/EffectsEditorTool.cpp
      effects/PhotoEffectsGroup.cpp
      utils/KEditFactory.cpp
      events/UndoCommandEvent.cpp
      events/UndoCommandEventFilter.cpp
      effects/PhotoEffectsLoader.cpp
      utils/global.cpp
      widgets/items/TextItem.cpp
      widgets/dialogs/ImageFileDialog.cpp
      settings/PFEConfig.cpp
      settings/PFEConfigViewWidget.cpp
      widgets/tools/TextEditorTool.cpp
      listeners/TextColorChangeListener.cpp
      listeners/TextFontChangeListener.cpp
      widgets/items/AbstractItemInterface.cpp
      borders/BordersGroup.cpp
      borders/BorderDrawersLoader.cpp
      borders/BorderChangeListener.cpp
      widgets/dialogs/GridSetupDialog.cpp
      widgets/canvas/SceneBackground.cpp
      widgets/tools/CanvasEditTool.cpp
      widgets/tools/AbstractTool.cpp
      widgets/tools/AbstractItemsTool.cpp
      widgets/tools/AbstractItemsListViewTool.cpp
      widgets/extra/PatternsComboBox.cpp
      widgets/canvas/RotationWidgetItem.cpp
      widgets/canvas/ScalingWidgetItem.cpp
      widgets/canvas/CropWidgetItem.cpp
      widgets/canvas/MousePressListener.cpp
  )

  SET(PhotoFramesEditor_RCS
      resources/icons.qrc
  )

  INCLUDE_DIRECTORIES(${QT_INCLUDE}
                      ${KDE4_INCLUDES}
                      ${INCLUDE_DIRS}
                      ${KIPI_INCLUDE_DIR})

  QT4_ADD_RESOURCES( PhotoFramesEditor_RC_SRCS ${PhotoFramesEditor_RCS} )

  KDE4_ADD_KCFG_FILES(PhotoFramesEditor_SRCS GENERATE_MOC ./settings/PFEConfigSkeleton.kcfgc)
  INSTALL( FILES ./settings/PFEConfigSkeleton.kcfgc DESTINATION ${KCFG_INSTALL_DIR} )

  # Adjust flag for static lib and 64 bits computers using -fPIC for GCC compiler (B.K.O: #269903)
  FOREACH(_currentfile ${expoblending_common_SRCS})
      IF(NOT MSVC)
          SET_SOURCE_FILES_PROPERTIES(${_currentfile} PROPERTIES COMPILE_FLAGS "-fPIC")
      ENDIF(NOT MSVC)
  ENDFOREACH(_currentfile ${expoblending_common_SRCS})

  KDE4_ADD_LIBRARY( libphotoframeseditor STATIC ${PhotoFramesEditor_SRCS} ${PhotoFramesEditor_RC_SRCS} )

  TARGET_LINK_LIBRARIES( libphotoframeseditor ${QT_LIBRARIES} ${KDE4_KDEUI_LIBS} ${KDE4_KPARTS_LIBS} ${KIPI_LIBRARIES} PropertyBrowser )


# =======================================================
# PhotoFramesEditor plugin

  #SET(kipiplugin_PhotoFramesEditor_PART_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/plugin/plugin_photoframeseditor.cpp)

  #KDE4_ADD_PLUGIN(kipiplugin_photoframeseditor ${kipiplugin_PhotoFramesEditor_PART_SRCS})

  #TARGET_LINK_LIBRARIES(kipiplugin_photoframeseditor libphotoframeseditor)

  #INSTALL(TARGETS kipiplugin_photoframeseditor DESTINATION ${PLUGIN_INSTALL_DIR})
  #INSTALL(FILES plugin/kipiplugin_photoframeseditor.desktop DESTINATION ${SERVICES_INSTALL_DIR})

# =======================================================
# Stand-alone version of PhotoFramesEditor

  SET(PhotoFramesEditor_SRCS main.cpp)

  KDE4_ADD_APP_ICON( PhotoFramesEditor_SRCS "resources/app/hi*-app-photoframeseditor.png" )

  KDE4_ADD_EXECUTABLE( photoframeseditor ${PhotoFramesEditor_SRCS} )

  TARGET_LINK_LIBRARIES( photoframeseditor libphotoframeseditor )
  INSTALL(TARGETS photoframeseditor  ${INSTALL_TARGETS_DEFAULT_ARGS})
  INSTALL(PROGRAMS plugin/photoframeseditor.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
  INSTALL(FILES plugin/photoframeseditorui.rc DESTINATION ${DATA_INSTALL_DIR}/photoframeseditor)
  #INSTALL(FILES main/photoframeseditor.notifyrc DESTINATION ${DATA_INSTALL_DIR}/photoframeseditor)
