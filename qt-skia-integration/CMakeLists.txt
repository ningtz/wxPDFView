cmake_minimum_required(VERSION 3.16)
project(QtSkiaIntegration VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 查找Qt6或Qt5
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

# 设置Skia路径（根据实际安装路径修改）
set(SKIA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party/skia" CACHE PATH "Skia source directory")
set(SKIA_BUILD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/skia" CACHE PATH "Skia build directory")

# 查找Skia库
# 方式1: 如果Skia已经编译为静态库
if(EXISTS "${SKIA_BUILD_DIR}/libskia.a" OR EXISTS "${SKIA_BUILD_DIR}/libskia.lib")
    set(SKIA_LIBRARY "${SKIA_BUILD_DIR}/libskia.a")
    if(WIN32)
        set(SKIA_LIBRARY "${SKIA_BUILD_DIR}/libskia.lib")
    endif()
    message(STATUS "Found Skia library: ${SKIA_LIBRARY}")
else()
    # 方式2: 使用find_library查找
    find_library(SKIA_LIBRARY
        NAMES skia libskia
        PATHS
            ${SKIA_BUILD_DIR}
            ${SKIA_DIR}/out
            /usr/local/lib
            ${CMAKE_PREFIX_PATH}/lib
    )
endif()

# 设置Skia包含目录
if(EXISTS "${SKIA_DIR}/include")
    set(SKIA_INCLUDE_DIR "${SKIA_DIR}/include")
elseif(EXISTS "${SKIA_BUILD_DIR}/include")
    set(SKIA_INCLUDE_DIR "${SKIA_BUILD_DIR}/include")
else()
    # 默认Skia头文件位置
    set(SKIA_INCLUDE_DIR "${SKIA_DIR}")
endif()

include_directories(${SKIA_INCLUDE_DIR})
include_directories(${SKIA_DIR}/include/core)
include_directories(${SKIA_DIR}/include/gpu)
include_directories(${SKIA_DIR}/include/utils)

# 添加可执行文件
set(SOURCES
    main.cpp
    MainWindow.cpp
    SkiaWidget.cpp
)

set(HEADERS
    MainWindow.h
    SkiaWidget.h
)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# 链接Qt库
target_link_libraries(${PROJECT_NAME}
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

# 链接Skia库
if(SKIA_LIBRARY)
    target_link_libraries(${PROJECT_NAME} ${SKIA_LIBRARY})
else()
    message(WARNING "Skia library not found. Please set SKIA_LIBRARY manually.")
endif()

# 链接Skia依赖的系统库
if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} 
        pthread
        dl
        fontconfig
        freetype
    )
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(IOKIT_LIBRARY IOKit)
    find_library(COREGRAPHICS_LIBRARY CoreGraphics)
    target_link_libraries(${PROJECT_NAME}
        ${COCOA_LIBRARY}
        ${IOKIT_LIBRARY}
        ${COREGRAPHICS_LIBRARY}
    )
elseif(WIN32)
    # Windows特定库
endif()

# 设置Qt MOC、UIC、RCC自动处理
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
