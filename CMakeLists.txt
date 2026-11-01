cmake_minimum_required(VERSION 3.21...4.4)

project(BirchEngine VERSION 1.0.1.0)

configure_file(BirchEngineVer.h.in include/BirchEngineVer.h)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (SRCS
    src/AssetManager.cpp
    src/Collision.cpp
    src/Game.cpp
    src/main.cpp
    src/Map.cpp
    src/TextureManager.cpp
    src/Vector2D.cpp
)

set (ECS_SRCS
    src/ECS/ECS.cpp
)

set (HDRS
    include/AssetManager.h
    include/BirchEngineVer.h
    include/Collision.h
    include/Game.h
    include/Map.h
    include/TextureManager.h
    include/Vector2D.h
)

set (ECS_HDRS
    include/ECS/Animation.h
    include/ECS/ColliderComponent.h
    include/ECS/Components.h
    include/ECS/ECS.h
    include/ECS/KeyboardController.h
    include/ECS/ProjectileComponent.h
    include/ECS/SpriteComponent.h
    include/ECS/TileComponent.h
    include/ECS/TransformComponent.h
    include/ECS/UILabel.h
)

set (ASSETS
    Resources/assets/collisionmap.txt
    Resources/assets/ColTex.png
    Resources/assets/map.map
    Resources/assets/player_anims.png
    Resources/assets/proj.png
    Resources/assets/terrain_ss.png
)

set (FONTS
    Resources/fonts/LiberationSans-Regular.ttf
    Resources/fonts/LICENSE            # OFL: licence must accompany the fonts
)

set (EXTRAS
    BirchEngineVer.h.in
    README.md
    TODO.md
    LICENSE
)

source_group("Header Files" FILES ${HDRS})
source_group("Header Files/ECS" FILES ${ECS_HDRS})
source_group("Resource Files/assets" FILES ${ASSETS})
source_group("Resource Files/fonts" FILES ${FONTS})
source_group("Source Files" FILES ${SRCS})
source_group("Source Files/ECS" FILES ${ECS_SRCS})
source_group("Extra Files" FILES ${EXTRAS})

add_executable(${PROJECT_NAME} ${SRCS} ${ECS_SRCS} ${HDRS} ${ECS_HDRS} ${ASSETS} ${FONTS} ${EXTRAS})

set_target_properties(${PROJECT_NAME} PROPERTIES
   VS_STARTUP_PROJECT ${PROJECT_NAME}
   CXX_STANDARD 20
   C_STANDARD 17
)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)
target_compile_options(${PROJECT_NAME} PRIVATE
    $<$<C_COMPILER_ID:MSVC>:/W4>
    $<$<CXX_COMPILER_ID:MSVC>:/W4>
    $<$<NOT:$<C_COMPILER_ID:MSVC>>:-Wall>
    $<$<NOT:$<CXX_COMPILER_ID:MSVC>>:-Wall>
)

# Runtime resource folder, relative to the executable. Used by the code
# (Game::resourcePath), the build copy and the install, so they always agree.
# A macOS bundle will set this to "" (resources live in Contents/Resources/).
set(BIRCH_RESOURCE_DIR "Resources/")
target_compile_definitions(${PROJECT_NAME} PRIVATE
    BIRCH_RESOURCE_DIR="${BIRCH_RESOURCE_DIR}"
)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_image::SDL2_image>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_ttf::SDL2_ttf>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    COMMENT "Copy SDL libs...\n"
)
# Runs on every build; copy_if_different only touches changed files.
add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E make_directory
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/${BIRCH_RESOURCE_DIR}assets
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/${BIRCH_RESOURCE_DIR}fonts
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${ASSETS} $<TARGET_FILE_DIR:${PROJECT_NAME}>/${BIRCH_RESOURCE_DIR}assets
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        ${FONTS} $<TARGET_FILE_DIR:${PROJECT_NAME}>/${BIRCH_RESOURCE_DIR}fonts
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMENT "Copy runtime resources"
    VERBATIM
)
add_dependencies(${PROJECT_NAME} copy_resources)

# Portable layout: executable, runtime libraries and Resources/ side by side,
# matching the build directory. Use -DBIRCH_INSTALL_DIR=. to install directly
# into the prefix root (e.g. a dedicated folder on Windows).
set(BIRCH_INSTALL_DIR bin CACHE STRING "Install directory (relative to prefix) for the executable and its resources")

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION ${BIRCH_INSTALL_DIR}
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/archive
)
install(IMPORTED_RUNTIME_ARTIFACTS
    SDL2::SDL2 DESTINATION ${BIRCH_INSTALL_DIR}
)
install(IMPORTED_RUNTIME_ARTIFACTS
    SDL2_image::SDL2_image DESTINATION ${BIRCH_INSTALL_DIR}
)
install(IMPORTED_RUNTIME_ARTIFACTS
    SDL2_ttf::SDL2_ttf DESTINATION ${BIRCH_INSTALL_DIR}
)
install(FILES ${ASSETS} DESTINATION ${BIRCH_INSTALL_DIR}/${BIRCH_RESOURCE_DIR}assets)
install(FILES ${FONTS} DESTINATION ${BIRCH_INSTALL_DIR}/${BIRCH_RESOURCE_DIR}fonts)

