set(CMAKE_CXX_STANDARD 20)

add_library(scanner
../db/mysql.hpp
../pub/publisher_base.hpp
../util/json.hpp
scheduler.hpp
task_base.hpp
task_new_championship.hpp
task_result.hpp
)

set_target_properties(scanner 
PROPERTIES 
LINKER_LANGUAGE CXX)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

target_link_libraries(scanner cpr::cpr)
