# Install script for directory: /home/coder89/KDE/PFE/photoframeseditor/effectplugins/colorize

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "/usr/")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "RelWithDebInfo")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

# Install shared libraries without execute permission?
IF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)
  SET(CMAKE_INSTALL_SO_NO_EXE "1")
ENDIF(NOT DEFINED CMAKE_INSTALL_SO_NO_EXE)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF(EXISTS "$ENV{DESTDIR}/usr/lib/kde4/photolayoutseditoreffectplugin_colorize.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib/kde4/photolayoutseditoreffectplugin_colorize.so")
    FILE(RPATH_CHECK
         FILE "$ENV{DESTDIR}/usr/lib/kde4/photolayoutseditoreffectplugin_colorize.so"
         RPATH "/usr//lib")
  ENDIF()
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/lib/kde4/photolayoutseditoreffectplugin_colorize.so")
FILE(INSTALL DESTINATION "/usr/lib/kde4" TYPE MODULE FILES "/home/coder89/KDE/PFE/photoframeseditor/build/lib/photolayoutseditoreffectplugin_colorize.so")
  IF(EXISTS "$ENV{DESTDIR}/usr/lib/kde4/photolayoutseditoreffectplugin_colorize.so" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}/usr/lib/kde4/photolayoutseditoreffectplugin_colorize.so")
    FILE(RPATH_CHANGE
         FILE "$ENV{DESTDIR}/usr/lib/kde4/photolayoutseditoreffectplugin_colorize.so"
         OLD_RPATH "/home/coder89/KDE/PFE/photoframeseditor/build/utils/qtpropertybrowser:"
         NEW_RPATH "/usr//lib")
    IF(CMAKE_INSTALL_DO_STRIP)
      EXECUTE_PROCESS(COMMAND "/usr/bin/strip" "$ENV{DESTDIR}/usr/lib/kde4/photolayoutseditoreffectplugin_colorize.so")
    ENDIF(CMAKE_INSTALL_DO_STRIP)
  ENDIF()
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  list(APPEND CPACK_ABSOLUTE_DESTINATION_FILES
   "/usr/share/kde4/services/photolayoutseditoreffectplugin_colorize.desktop")
FILE(INSTALL DESTINATION "/usr/share/kde4/services" TYPE FILE FILES "/home/coder89/KDE/PFE/photoframeseditor/effectplugins/colorize/photolayoutseditoreffectplugin_colorize.desktop")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

