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

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/todo.css $<TARGET_FILE_DIR:${PROJECT_NAME}>/todo.css)