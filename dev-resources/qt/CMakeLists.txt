cmake_minimum_required(VERSION 3.14)

project(anatomyqt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set (HEADERS anqaboutdialog.h  anqcsv.h  anqjson.h  anqta1tree.h  anqta2tree.h  anqtermstree.h  anqtools.h
    anqinfolabel.h anqfilewidget.h  anqtreewidget.h anatomyqt.h)
set (SOURCES anqaboutdialog.cpp anqcsv.cpp  anqjson.cpp  anqta1tree.cpp  anqta2tree.cpp  anqtermstree.cpp  anqtools.cpp
     anqinfolabel.cpp anqfilewidget.cpp  anqtreewidget.cpp)

add_library(anatomyqt SHARED ${HEADERS} ${SOURCES})


#target_link_libraries(anatomyqt PRIVATE Qt${QT_VERSION_MAJOR}::Core)

target_include_directories(anatomyqt PUBLIC ../cpp)
target_link_libraries(anatomyqt PUBLIC anatomycpp PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

install(TARGETS anatomyqt DESTINATION lib)
