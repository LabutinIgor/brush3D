cmake_minimum_required(VERSION 2.8.8)

project(ObjViewer)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB SOURCE brush_ui/src/*.cpp)
file(GLOB HEADER brush_ui/src/*.hpp)

file(GLOB_RECURSE UI_SOURCE brush_ui/src/ui/*.cpp)
file(GLOB_RECURSE UI_HEADER brush_ui/src/ui/*.h)

find_package(Qt5Widgets REQUIRED)

add_subdirectory(brush)

include_directories(
        brush/include
        brush_ui/src/ui
        brush_ui/external/glm
        brush_ui/external/tinyobjloader)

QT5_WRAP_UI(UIS_HDRS brush_ui/src/ui/mainwindow.ui)

add_executable(objviewer
        ${SOURCE} ${HEADER}
        ${UI_SOURCE} ${UI_HEADER} ui_mainwindow.h)

qt5_use_modules(objviewer Widgets)

target_link_libraries(objviewer Brush)
