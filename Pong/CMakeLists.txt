cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 20)

add_executable(${CMAKE_PROJECT_NAME}
        src/Pong.cpp
        src/Renderer.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME} walnut)