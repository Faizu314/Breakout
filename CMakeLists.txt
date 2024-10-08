cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(Engine)

set(ENGINE_SOURCES
    Engine/src/main.cpp
)

set(ENGINE_HEADERS
    Engine/include/common/Engine.hpp
)

source_group("Header Files" FILES ${ENGINE_HEADERS})
source_group("Source Files" FILES ${ENGINE_SOURCES})

include_directories(Engine/include/common)

if (APPLE)
    add_executable(Engine ${ENGINE_SOURCES} ${ENGINE_HEADERS})
endif()
