set(CMAKE_CXX_STANDARD 20)

add_library(scanner
scheduler.hpp
task_base.hpp
task_new_championship.hpp
task_result.hpp
)

set_target_properties(scanner 
PROPERTIES 
LINKER_LANGUAGE CXX)