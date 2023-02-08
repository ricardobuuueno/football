set(CMAKE_CXX_STANDARD 20)

add_library(db
mysql.cpp
)

set_target_properties(db PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(db PUBLIC /usr/include/cppconn/)

link_directories(/usr/lib)
target_link_libraries(db mysqlcppconn)

#fmt
find_package(fmt REQUIRED)
include_directories(${fmt_INCLUDE_DIRS})
target_link_libraries(db fmt::fmt)
