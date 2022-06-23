cmake_minimum_required(VERSION 3.17)
project(picket)

# GTKMM
find_package(PkgConfig REQUIRED)
pkg_search_module(gtkmm-3.0 REQUIRED IMPORTED_TARGET gtkmm-3.0)

# X11
find_package(X11 REQUIRED)


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

target_compile_features(picket PRIVATE cxx_std_14)

target_include_directories(picket PRIVATE
        src
        src/cfg
        src/models
        src/utils)

target_link_libraries(picket PRIVATE
        PkgConfig::gtkmm-3.0
        X11::X11)

# install target
SET(CMAKE_INSTALL_PREFIX "/usr/")
install(TARGETS picket)
install(FILES "ui/MainWindow.glade" DESTINATION share/${PROJECT_NAME}/)
install(FILES "ui/SettingsWindow.glade" DESTINATION share/${PROJECT_NAME}/)
install(FILES "ui/FormatEditWindow.glade" DESTINATION share/${PROJECT_NAME}/)
install(FILES "ui/ColorPickerWindow.glade" DESTINATION share/${PROJECT_NAME}/)
install(FILES "resources/picket.desktop" DESTINATION share/applications/)
install(FILES "resources/picket.svg" DESTINATION share/pixmaps/)
install(FILES "LICENSE" DESTINATION share/licenses/${PROJECT_NAME})

# uninstall target
if(NOT TARGET uninstall)
    configure_file(
            "cmake_uninstall.cmake.in"
            "cmake_uninstall.cmake"
            IMMEDIATE @ONLY)

    add_custom_target(uninstall
            COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake)
endif()
