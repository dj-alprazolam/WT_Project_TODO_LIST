cmake_minimum_required(VERSION 3.30)
project(todo_wt CXX)

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Wt REQUIRED COMPONENTS 
    Wt 
    HTTP 
    Dbo              
    DboSqlite3       
)

add_executable(todo_wt 
    main.cpp 
    todo.cpp
    data.hpp
)

target_link_libraries(todo_wt PRIVATE 
    Wt::Wt 
    Wt::HTTP 
    Wt::Dbo 
    Wt::DboSqlite3
)