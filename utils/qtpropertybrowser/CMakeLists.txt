PROJECT( qtpropertybrowser )

INCLUDE_DIRECTORIES(${QT_INCLUDE})

SET( SOURCES
    ./qtpropertybrowser.cpp
    ./qtpropertymanager.cpp
    ./qteditorfactory.cpp
    ./qtvariantproperty.cpp
    ./qttreepropertybrowser.cpp
    ./qtbuttonpropertybrowser.cpp
    ./qtgroupboxpropertybrowser.cpp
    ./qtpropertybrowserutils.cpp
)

SET( MOC_HEADERS
    ./qtpropertybrowser.h
    ./qtpropertymanager.h
    ./qteditorfactory.h
    ./qtvariantproperty.h
    ./qttreepropertybrowser.h
    ./qtbuttonpropertybrowser.h
    ./qtgroupboxpropertybrowser.h
    ./qtpropertybrowserutils_p.h
)

SET( RESOURCES
    ./qtpropertybrowser.qrc
)

# enable warnings
ADD_DEFINITIONS( -Wall )

# Qt library
FIND_PACKAGE( Qt4 REQUIRED )

# add some useful macros and variables
# (QT_USE_FILE is a variable defined by FIND_PACKAGE( Qt4 ) that contains a path to CMake script)
INCLUDE( ${QT_USE_FILE} )

# this command will generate rules that will run rcc on all files from SAMPLE_RCS
# in result SAMPLE_RC_SRCS variable will contain paths to files produced by rcc
QT4_ADD_RESOURCES( RESOURCES_SRCS ${RESOURCES} )

# and finally this will run moc:
QT4_WRAP_CPP( MOC_SRCS ${MOC_HEADERS} )

ADD_LIBRARY( qtpropertybrowser ${SOURCES} ${RESOURCES_SRCS} ${MOC_SRCS} )

TARGET_LINK_LIBRARIES( sample ${QT_LIBRARIES} )
