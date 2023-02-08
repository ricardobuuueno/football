set(CMAKE_CXX_STANDARD 20)

add_library(classes
championship.hpp
club.hpp
game.hpp
goal.hpp
player.hpp
referee.hpp
score.hpp
season.hpp
stadium.hpp
team.hpp
)

set_target_properties(classes PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(classes PRIVATE classes)
