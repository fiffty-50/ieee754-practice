cmake_minimum_required(VERSION 3.5)

project(qcs VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        src/main.cpp

        src/nibble.h
        src/RandomBits.h
        src/number.h
        src/number.cpp
        src/floatrepresentation.h
        src/floatrepresentation.cpp
        src/exercisegenerator.h
        src/exercisegenerator.cpp
        src/bitband.h
        src/bitband.cpp
        
        # GUI
        src/gui/mainwindow.cpp
        src/gui/mainwindow.h
        src/gui/mainwindow.ui
        src/gui/dectoieee.h
        src/gui/dectoieee.cpp
        src/gui/dectoieee.ui
        src/gui/ieeetodec.h
        src/gui/ieeetodec.cpp
        src/gui/ieeetodec.ui
        src/gui/bitbandwidget.h
        src/gui/bitbandwidget.cpp
        src/gui/bitbandwidget.ui
        src/gui/intconverterwidget.h
        src/gui/intconverterwidget.cpp
        src/gui/intconverterwidget.ui
        src/gui/floatconverterwidget.h
        src/gui/floatconverterwidget.cpp
        src/gui/floatconverterwidget.ui

        src/debug.h

)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(qcs
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET qcs APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(qcs SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(qcs
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(qcs PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(qcs PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(qcs)
endif()
