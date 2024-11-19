cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)

project(Breakout)

set(PHEZU_SOURCES
    Phezu/src/Phezu.cpp
    Phezu/src/Engine.cpp
    Phezu/src/Math.cpp
    Phezu/src/Input.cpp
    Phezu/src/Renderer.cpp
    Phezu/src/Physics.cpp
    Phezu/src/Window.cpp
    Phezu/src/Entity.cpp
    Phezu/src/Scene.cpp
    Phezu/src/ShapeData.cpp
    Phezu/src/PhysicsData.cpp
    Phezu/src/TransformData.cpp
    Phezu/src/SceneManager.cpp
    Phezu/src/PrefabEntity.cpp
    Phezu/src/EntityTemplate.cpp
    Phezu/src/BehaviourComponentPrefab.cpp
)
set(PHEZU_VENDOR_SOURCES
    Phezu/vendor/tinyxml2/tinyxml2.cpp
)
set(PHEZU_HEADERS
    Phezu/include/phezu/Phezu.hpp
    Phezu/include/phezu/Engine.hpp
    Phezu/include/phezu/Renderer.hpp
    Phezu/include/phezu/Physics.hpp
    Phezu/include/phezu/Input.hpp
    Phezu/include/phezu/Window.hpp
    Phezu/include/phezu/scene/Entity.hpp
    Phezu/include/phezu/scene/PrefabEntity.hpp
    Phezu/include/phezu/scene/Prefab.hpp
    Phezu/include/phezu/scene/EntityTemplate.hpp
    Phezu/include/phezu/scene/Scene.hpp
    Phezu/include/phezu/scene/SceneManager.hpp
    Phezu/include/phezu/scene/components/DataComponent.hpp
    Phezu/include/phezu/scene/components/TransformData.hpp
    Phezu/include/phezu/scene/components/ShapeData.hpp
    Phezu/include/phezu/scene/components/RenderData.hpp
    Phezu/include/phezu/scene/components/PhysicsData.hpp
    Phezu/include/phezu/scene/components/BehaviourComponent.hpp
    Phezu/include/phezu/scene/components/BehaviourComponentPrefab.hpp
    Phezu/include/phezu/maths/Math.hpp
)

source_group("Header Files" FILES ${PHEZU_HEADERS})
source_group("Source Files" FILES ${PHEZU_SOURCES})
source_group("Vendor\\Source Files" FILES ${PHEZU_VENDOR_SOURCES})

set(GAME_SOURCES
    Game/src/main.cpp
)

set(GAME_HEADERS
    Game/include/Game.hpp
)

source_group("Header Files" FILES ${GAME_HEADERS})
source_group("Source Files" FILES ${GAME_SOURCES})

add_library(Phezu STATIC ${PHEZU_SOURCES} ${PHEZU_VENDOR_SOURCES} ${PHEZU_HEADERS})
set_target_properties(Phezu PROPERTIES OUTPUT_NAME Phezu PREFIX "")

if (APPLE)
    add_executable(Game ${GAME_SOURCES} ${GAME_HEADERS})

    set(LIB_PATH "${CMAKE_SOURCE_DIR}/Phezu/lib/mac")

    set_target_properties(Game PROPERTIES
        INSTALL_RPATH "@loader_path"
        BUILD_WITH_INSTALL_RPATH TRUE
    )

    target_link_libraries(Phezu PRIVATE
        "-F${LIB_PATH}"
        "-framework SDL2"
        "-framework SDL2_image"
        "-framework SDL2_ttf"
    )

    target_include_directories(Phezu PRIVATE Phezu/include/mac)
elseif (WIN32)
    add_executable(Game WIN32 ${GAME_SOURCES} ${GAME_HEADERS})

    set(LIB_PATH "${CMAKE_SOURCE_DIR}/Phezu/lib/windows")

    target_link_libraries(Phezu PRIVATE
        "${LIB_PATH}/SDL2.lib"
        "${LIB_PATH}/SDL2_image.lib"
        "${LIB_PATH}/SDL2_ttf.lib"
    )

    target_include_directories(Phezu PRIVATE Phezu/include/windows)
endif()

target_include_directories(Phezu PUBLIC Phezu/include/phezu)
target_include_directories(Phezu PUBLIC Phezu/include/common)
target_include_directories(Game PUBLIC Game/include)
target_link_libraries(Game PRIVATE Phezu)

# copy and paste the libraries to the executable directory
add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${LIB_PATH}"
    "$<TARGET_FILE_DIR:Game>"
)

# copy and paste the assets to the executable directory
add_custom_command(TARGET Game POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/Game/assets"
    "$<TARGET_FILE_DIR:Game>"
)
