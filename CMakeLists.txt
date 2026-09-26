cmake_minimum_required(VERSION 3.30.0)
project(WT_project VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(WT_project main.cpp todo.cpp)

target_link_libraries(WT_project PRIVATE
    wt
    wthttp
)