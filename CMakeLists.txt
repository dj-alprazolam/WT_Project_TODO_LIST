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
    src/todo.cpp
    includes/operation.hpp
    includes/LoadData.hpp
    src/LoadData.cpp
    src/row_worker.cpp
    includes/row_worker.hpp
    includes/add_task.hpp
    src/add_task.cpp
    includes/delete_task.hpp
    src/delete_task.cpp
    includes/update_task.hpp
    src/update_task.cpp
    includes/completed_task.hpp
    src/completed_task.cpp
)

target_include_directories(todo_wt PRIVATE ${CMAKE_SOURCE_DIR}/includes)

target_link_libraries(todo_wt PRIVATE 
    Wt::Wt 
    Wt::HTTP 
    Wt::Dbo 
    Wt::DboSqlite3
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_SOURCE_DIR}/todo.css $<TARGET_FILE_DIR:${PROJECT_NAME}>/todo.css)