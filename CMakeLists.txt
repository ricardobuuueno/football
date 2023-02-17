cmake_minimum_required(VERSION 3.23.0)

project(football VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_DEBUG_POSTFIX d)

add_subdirectory(classes)
add_subdirectory(db)
add_subdirectory(util)
add_subdirectory(plog)

# search for clang-tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

# setup clang-tidy command from executable + options
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*")

add_executable(football 
football.cpp
globals.cpp
)

# set CXX_CLANG_TIDY property after defining the target
set_target_properties(football 
PROPERTIES 
CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

set_target_properties(football 
PROPERTIES 
RUNTIME_OUTPUT_DIRECTORY "~/football/bin"
)

set_target_properties(football 
PROPERTIES 
DEBUG_POSTFIX ${CMAKE_DEBUG_POSTFIX})

target_link_libraries(football PUBLIC classes db util)

target_include_directories(classes PRIVATE ~/football/classes/)
target_include_directories(db PRIVATE ~/football/db/)
target_include_directories(util PRIVATE ~/football/util/)
target_include_directories(plog PRIVATE ~/football/plog/)


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
  football-test/test.cpp
  football-test/objects.cpp
  globals.cpp
)

set_target_properties(football-test 
PROPERTIES 
RUNTIME_OUTPUT_DIRECTORY "~/football/bin"
)

target_link_libraries(football-test GTest::gtest_main classes db util)

include(GoogleTest)
gtest_discover_tests(football-test)
