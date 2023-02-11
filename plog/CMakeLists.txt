set(CMAKE_CXX_STANDARD 20)

add_library(plog
Init.h
Log.h
Logger.h
Record.h
Severity.h
Util.h
WinApi.h
)

# search for clang-tidy
find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

# setup clang-tidy command from executable + options
set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*")

# set CXX_CLANG_TIDY property after defining the target
set_target_properties(plog 
PROPERTIES 
CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

set_target_properties(plog 
PROPERTIES 
LINKER_LANGUAGE CXX)

target_include_directories(plog PRIVATE ~/football/plog/)
