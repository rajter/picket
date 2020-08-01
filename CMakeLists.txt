cmake_minimum_required(VERSION 3.17)
project(picket)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(src/cfg)
include_directories(src/models)
include_directories(src/utils)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})

add_executable(picket
        src/cfg/config.cpp
        src/cfg/config.h
        src/cfg/ini.h
        src/models/color.cpp
        src/models/color.h
        src/models/colorformatmanager.cpp
        src/models/colorformatmanager.h
        src/utils/colorspace.cpp
        src/utils/datautils.h
        src/utils/dirutils.h
        src/utils/drawinghelpers.cpp
        src/utils/enums.h
        src/utils/stringutils.cpp
        src/colorpickerwindow.cpp
        src/colorpickerwindow.h
        src/formateditwindow.cpp
        src/formateditwindow.h
        src/main.cpp
        src/mainwindow.cpp
        src/mainwindow.h
        src/settingswindow.cpp
        src/settingswindow.h)

target_link_libraries(picket
        ${GTKMM_LIBRARIES})
