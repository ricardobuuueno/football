set(CMAKE_CXX_STANDARD 20)

add_library(db
mysql.hpp
mysql.cpp
../util/json.hpp
)

# search for clang-tidy
#find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

# setup clang-tidy command from executable + options
#set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-checks=-*,modernize-*;-header-filter=.*")

# set CXX_CLANG_TIDY property after defining the target
#set_target_properties(db 
#PROPERTIES 
#CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

set_target_properties(db 
PROPERTIES 
LINKER_LANGUAGE CXX)

target_include_directories(db PUBLIC /usr/include/cppconn/)

link_directories(/usr/lib)
target_link_libraries(db mysqlcppconn)

#fmt
find_package(fmt REQUIRED)
include_directories(${fmt_INCLUDE_DIRS})
target_link_libraries(db fmt::fmt)
