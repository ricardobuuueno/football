set(CMAKE_CXX_STANDARD 20)

add_library(util
toml.hpp
)

set_target_properties(util PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(util PRIVATE util)