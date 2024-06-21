cmake_minimum_required(VERSION 3.5)

project(labCC VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        cipher.cpp
        lfsr.cpp
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(labCC
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET labCC APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(labCC SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(labCC
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(labCC PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(labCC PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(labCC)
endif()


set(Boost_USE_STATIC_LIBS ON)
set(Boost_USE_MULTITHREADED ON)
find_package(Boost REQUIRED COMPONENTS
    program_options
    )

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

add_executable(
    task-generation
    task.cpp
    cipher.cpp
    lfsr.cpp
)

target_link_libraries(task-generation ${Boost_LIBRARIES})

#
#
#cmake_minimum_required(VERSION 3.5)
#
#project(labCC)
#
## Настройка кросс-компилятора
#set(CMAKE_SYSTEM_NAME Windows)
#set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
#set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
#set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
#
## Настройка сборки
#set(CMAKE_CXX_STANDARD 11)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_INCLUDE_CURRENT_DIR ON)
#set(CMAKE_AUTOUIC ON)
#set(CMAKE_AUTOMOC ON)
#set(CMAKE_AUTORCC ON)
#
#find_package(Qt5 COMPONENTS Widgets REQUIRED)
#
#set(SOURCES
#        main.cpp
#        mainwindow.cpp
#        mainwindow.h
#        mainwindow.ui
#        cipher.cpp
#        lfsr.cpp
#)
#
#add_executable(labCC WIN32 ${SOURCES})
#
#target_link_libraries(labCC Qt5::Widgets)
#
## Настройка установки
#install(TARGETS labCC DESTINATION bin)
