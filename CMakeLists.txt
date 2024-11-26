cmake_minimum_required(VERSION 3.16)

project(GetColor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    src/Main.cpp
        src/MainWindow.cpp
        src/MainWindow.h
        src/MainWindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(GetColor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/PixelPreview.h src/PixelPreview.cpp
        src/ColorPicker.h src/ColorPicker.cpp
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET GetColor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(GetColor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(GetColor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(GetColor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.GetColor)
endif()
set_target_properties(GetColor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS GetColor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(GetColor)
endif()

# Windows 平台下的部署命令
if(WIN32 AND CMAKE_BUILD_TYPE STREQUAL "Release")
    # 获取 windeployqt.exe 的路径
    get_target_property(_qmake_executable Qt${QT_VERSION_MAJOR}::qmake IMPORTED_LOCATION)
    get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
    
    add_custom_command(TARGET GetColor POST_BUILD
        COMMAND "${_qt_bin_dir}/windeployqt.exe"
                --no-translations
                --no-system-d3d-compiler
                --no-opengl-sw
                $<TARGET_FILE:GetColor>
        COMMENT "Deploying Qt dependencies..."
    )
endif()

# 复制 cursor.png 文件的命令
add_custom_command(TARGET GetColor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/src/cursor.png
        $<TARGET_FILE_DIR:GetColor>/cursor.png
)
