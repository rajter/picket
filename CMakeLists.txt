cmake_minimum_required(VERSION 3.17)
project(picket)

set(CMAKE_CXX_STANDARD 14)

# GTKMM
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0 REQUIRED)
link_directories(${GTKMM_LIBRARY_DIRS})
include_directories(${GTKMM_INCLUDE_DIRS})

# X11
find_package(X11 REQUIRED)
link_libraries(${X11_LIBRARIES})
include_directories(${X11_INCLUDE_DIR})

# SRC
include_directories(src)
include_directories(src/cfg)
include_directories(src/models)
include_directories(src/utils)

add_executable(picket
        src/cfg/config.cpp
        src/cfg/config.h
        src/cfg/ini.h
        src/models/color.cpp
        src/models/color.h
        src/models/colorformatmanager.cpp
        src/models/colorformatmanager.h
        src/models/historymanager.cpp
        src/models/historymanager.h
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

# install target
SET(CMAKE_INSTALL_PREFIX "/usr/")
install(TARGETS picket DESTINATION bin)
install(FILES "${PROJECT_SOURCE_DIR}/ui/MainWindow.glade" DESTINATION share/${PROJECT_NAME}/)
install(FILES "${PROJECT_SOURCE_DIR}/ui/SettingsWindow.glade" DESTINATION share/${PROJECT_NAME}/)
install(FILES "${PROJECT_SOURCE_DIR}/ui/FormatEditWindow.glade" DESTINATION share/${PROJECT_NAME}/)
install(FILES "${PROJECT_SOURCE_DIR}/ui/ColorPickerWindow.glade" DESTINATION share/${PROJECT_NAME}/)
install(FILES "${PROJECT_SOURCE_DIR}/resources/picket.desktop" DESTINATION share/applications/)
install(FILES "${PROJECT_SOURCE_DIR}/resources/picket.svg" DESTINATION share/pixmaps/)
install(FILES "${PROJECT_SOURCE_DIR}/LICENSE" DESTINATION share/licenses/${PROJECT_NAME})

# uninstall target
if(NOT TARGET uninstall)
    configure_file(
            "${CMAKE_CURRENT_SOURCE_DIR}/cmake_uninstall.cmake.in"
            "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
            IMMEDIATE @ONLY)

    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()