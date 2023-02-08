cmake_minimum_required(VERSION 3.23.0)

project(football VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(classes)
add_subdirectory(db)
add_subdirectory(util)

add_executable(football 
main.cpp
)
set_target_properties(football 
PROPERTIES 
RUNTIME_OUTPUT_DIRECTORY "~/football/bin"
)

target_link_libraries(football PUBLIC classes db util)

target_include_directories(classes PRIVATE ~/football/classes/)
target_include_directories(db PRIVATE ~/football/db/)
target_include_directories(util PRIVATE ~/football/util/)
