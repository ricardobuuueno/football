cmake_minimum_required(VERSION 3.23.0)

project(football VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX d)
set(Boost_NO_WARN_NEW_VERSIONS 1)

add_subdirectory(classes)
add_subdirectory(db)
add_subdirectory(net)
add_subdirectory(util)
add_subdirectory(plog)

# search for clang-tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

# setup clang-tidy command from executable + options
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*;-header-filter=.*")

add_executable(${PROJECT_NAME} 
football.cpp
)

# set CXX_CLANG_TIDY property after defining the target
set_target_properties(${PROJECT_NAME} 
PROPERTIES 
CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

set_target_properties(${PROJECT_NAME} 
PROPERTIES 
RUNTIME_OUTPUT_DIRECTORY "~/football/bin"
)

set_target_properties(football 
PROPERTIES 
DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

#include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#conan_basic_setup()

target_link_libraries(${PROJECT_NAME} ${CONAN_LIBS})
target_link_libraries(${PROJECT_NAME} classes db net util)

target_include_directories(classes PRIVATE ~/football/classes/)
target_include_directories(db PRIVATE ~/football/db/)
target_include_directories(net PRIVATE ~/football/net/)
target_include_directories(util PRIVATE ~/football/util/)
target_include_directories(plog PRIVATE ~/football/plog/)


######### BOOST ###############

set(BOOST_ROOT /usr/local)
set(Boost_INCLUDE_DIR  /usr/local/include)
set(Boost_LIBRARY_DIR /usr/local/lib)

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.81.0 COMPONENTS system thread regex) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS}) 
    target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
endif()

######### RADAR ######################

add_executable(
  radar
  radar.cpp
)

set_target_properties(radar 
PROPERTIES 
RUNTIME_OUTPUT_DIRECTORY "~/football/bin"
)

target_link_libraries(radar classes db net util)


######### GOOGLE TESTS ###############

include(FetchContent)
FetchContent_Declare( 
  googletest
  URL https://github.com/google/googletest/archive/b796f7d44681514f58a683a3a71ff17c94edb0c1.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


#### TEST APPLICATION ###

enable_testing()

add_executable(
  football-test
  football-test/football_server.cpp
  football-test/football.cpp
  football-test/pch.cpp
  football-test/objects.cpp
)

set_target_properties(football-test 
PROPERTIES 
RUNTIME_OUTPUT_DIRECTORY "~/football/bin"
)

target_link_libraries(football-test GTest::gtest_main classes db net util)

include(GoogleTest)
gtest_discover_tests(football-test)
