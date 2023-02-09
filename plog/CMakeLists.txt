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

set_target_properties(plog PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(plog PRIVATE ~/football/plog/)
