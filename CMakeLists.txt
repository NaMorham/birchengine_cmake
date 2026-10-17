cmake_minimum_required(VERSION 3.21...4.4)

project(BirchEngine VERSION 1.0.0)

configure_file(BirchEngineVer.h.in include/BirchEngineVer.h)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set (SRCS
    src/main.cpp
    src/Game.cpp
)

set (HDRS
    include/BirchEngineVer.h
    include/Game.h    
)

#set (FONTS
#    fonts/cour.ttf
#    fonts/courbd.ttf
#    fonts/courbi.ttf
#    fonts/couri.ttf
#)

set (EXTRAS
    BirchEngineVer.h.in
    README.md
#    TODO.md
    LICENSE
)

source_group("Headers" FILES ${HDRS})
#source_group("Resources/fonts" FILES ${FONTS})
source_group("Sources" FILES ${SRCS})
source_group("Extras" FILES ${EXTRAS})

add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} ${EXTRAS})
#add_executable(${PROJECT_NAME} ${SRCS} ${HDRS} ${FONTS} ${EXTRAS})

set_target_properties(${PROJECT_NAME} PROPERTIES
   VS_STARTUP_PROJECT ${PROJECT_NAME}
   CXX_STANDARD 20
   C_STANDARD 17
)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

target_include_directories(${PROJECT_NAME} PUBLIC
    ${PROJECT_BINARY_DIR}
    ${PROJECT_SOURCE_DIR}/include
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2::SDL2>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:SDL2_ttf::SDL2_ttf>
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
        COMMENT "Copy SDL libs...\n"
)
#add_custom_command(
#    TARGET ${PROJECT_NAME} POST_BUILD
#    COMMAND ${CMAKE_COMMAND} -E copy_directory
#        ${CMAKE_CURRENT_SOURCE_DIR}/fonts
#        $<TARGET_FILE_DIR:${PROJECT_NAME}>/fonts
#        COMMAND_EXPAND_LISTS
#        COMMENT "Copy fonts...\n"
#)

install(TARGETS ${PROJECT_NAME}
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib/archive
)
install(IMPORTED_RUNTIME_ARTIFACTS
    SDL2::SDL2 DESTINATION bin
)
install(IMPORTED_RUNTIME_ARTIFACTS
    SDL2_ttf::SDL2_ttf DESTINATION bin
)
#install(FILES ${FONTS} DESTINATION fonts)
