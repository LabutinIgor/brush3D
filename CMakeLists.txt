cmake_minimum_required(VERSION 2.8.8)

set(CMAKE_PREFIX_PATH "/Users/igorl/Qt/5.7/clang_64")

project(ObjViewer)

set(CMAKE_CXX_COMPILER=/usr/bin/clang++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

file(GLOB ROOT_SOURCE ./src/*.cpp)
file(GLOB ROOT_HEADER ./src/*.h)

file(GLOB_RECURSE UI_SOURCE ./src/ui/*.cpp)
file(GLOB_RECURSE UI_HEADER ./src/ui/*.h)

find_package(Qt5Widgets REQUIRED)

include_directories(
        src/ui
        external/glm
        external/tinyobjloader
)

add_subdirectory(src/brush)

QT5_WRAP_UI(UIS_HDRS
        src/ui/mainwindow.ui
        )

add_executable(objviewer
        ${ROOT_SOURCE} ${ROOT_HEADER}
        ${UI_SOURCE} ${UI_HEADER}
        )

qt5_use_modules(objviewer Widgets)

target_link_libraries(objviewer Brush)
