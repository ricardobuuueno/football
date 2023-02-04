set(CMAKE_CXX_STANDARD 20)

add_library(db
mysql.cpp
)

set_target_properties(db PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(db PUBLIC include)

