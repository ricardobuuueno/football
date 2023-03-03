set(CMAKE_CXX_STANDARD 20)

add_library(net
../db/mysql.cpp
client.hpp
common.hpp
connection.hpp
event.hpp
event_action.cpp
football_client.hpp
football_server.hpp
message.hpp
server.hpp
tsqueue.hpp
)

# search for clang-tidy
#find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

# setup clang-tidy command from executable + options
#set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*;-header-filter=.*")

# set CXX_CLANG_TIDY property after defining the target
#set_target_properties(classes 
#PROPERTIES 
#CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

set_target_properties(net 
PROPERTIES 
LINKER_LANGUAGE CXX)

#target_include_directories(net PRIVATE net)
