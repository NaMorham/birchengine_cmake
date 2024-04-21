cmake_minimum_required(VERSION 3.29.1)

project(BirchEngine VERSION 1.0.0)

add_executable(${PROJECT_NAME} src/main.cpp)

message("\${CMAKE_SOURCE_DIR} = ${CMAKE_SOURCE_DIR}")
message("\${CMAKE_SOURCE_DIR}/../VS/SDL2/lib/x64 = ${CMAKE_SOURCE_DIR}/../VS/SDL2/lib/x64")
message("\${CMAKE_SOURCE_DIR}/../VS/SDL_image-2.0.1/lib/x64 = ${CMAKE_SOURCE_DIR}/../VS/SDL_image-2.0.1/lib/x64")

target_include_directories(${PROJECT_NAME} 
    PUBLIC "${CMAKE_SOURCE_DIR}/../VS/SDL2/include"
    PUBLIC "${CMAKE_SOURCE_DIR}/../VS/SDL2_image-2.0.1/include"
)
target_link_directories(${PROJECT_NAME} 
    PUBLIC "${CMAKE_SOURCE_DIR}/../VS/SDL2/lib/x64"
    PUBLIC "${CMAKE_SOURCE_DIR}/../VS/SDL2_image-2.0.1/lib/x64"
)
target_link_libraries(${PROJECT_NAME} SDL2 SDL2_image)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/out)

