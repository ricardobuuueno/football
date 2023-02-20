set(CMAKE_CXX_STANDARD 20)

add_library(util
constants.hpp
globals.cpp
toml.hpp
util.cpp
)

# search for clang-tidy
#find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

# setup clang-tidy command from executable + options
#set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*")

# set CXX_CLANG_TIDY property after defining the target
#set_target_properties(util 
#PROPERTIES 
#CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

set_target_properties(util 
PROPERTIES 
LINKER_LANGUAGE CXX)

target_include_directories(util PRIVATE util)
