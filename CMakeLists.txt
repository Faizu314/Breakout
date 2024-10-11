cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(BrickBreaker)

set(ENGINE_SOURCES
    Engine/src/main.cpp
)

set(ENGINE_HEADERS
    Engine/include/common/Engine.hpp
)

source_group("Header Files" FILES ${ENGINE_HEADERS})
source_group("Source Files" FILES ${ENGINE_SOURCES})

set(GAME_SOURCES
    Game/src/main.cpp
)

set(GAME_HEADERS
    Game/include/common/Game.hpp
)

source_group("Header Files" FILES ${GAME_HEADERS})
source_group("Source Files" FILES ${GAME_SOURCES})

if (APPLE)
    add_library(Engine STATIC ${ENGINE_SOURCES} ${ENGINE_HEADERS})
    add_executable(Game ${GAME_SOURCES} ${GAME_HEADERS})
    target_link_libraries(Game PRIVATE Engine)
endif()

target_include_directories(Engine PUBLIC Engine/include/common)
target_include_directories(Game PUBLIC Game/include/common)
