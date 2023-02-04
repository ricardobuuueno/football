cmake_minimum_required(VERSION 3.23.0)

project(football VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(classes)
add_subdirectory(db)

add_executable(football 
main.cpp
)

target_link_libraries(football PUBLIC classes db)
