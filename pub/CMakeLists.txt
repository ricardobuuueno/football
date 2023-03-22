set(CMAKE_CXX_STANDARD 20)

######### GUMBO ######################

set(GUMBO_SOURCES
~/gumbo-parser/src/attribute.h
~/gumbo-parser/src/attribute.c
~/gumbo-parser/src/char_ref.h
~/gumbo-parser/src/char_ref.c
~/gumbo-parser/src/error.h
~/gumbo-parser/src/error.c
~/gumbo-parser/src/gumbo.h
~/gumbo-parser/src/insertion_mode.h
~/gumbo-parser/src/parser.h
~/gumbo-parser/src/parser.c
~/gumbo-parser/src/string_buffer.h
~/gumbo-parser/src/string_buffer.c
~/gumbo-parser/src/string_piece.h
~/gumbo-parser/src/string_piece.c
~/gumbo-parser/src/tag_enum.h
~/gumbo-parser/src/tag_gperf.h
~/gumbo-parser/src/tag_sizes.h
~/gumbo-parser/src/tag_strings.h
~/gumbo-parser/src/tag.c
~/gumbo-parser/src/token_type.h
~/gumbo-parser/src/tokenizer.h
~/gumbo-parser/src/tokenizer.c
~/gumbo-parser/src/tokenizer_states.h
~/gumbo-parser/src/utf8.h
~/gumbo-parser/src/utf8.c
~/gumbo-parser/src/util.h
~/gumbo-parser/src/util.c
~/gumbo-parser/src/vector.h
~/gumbo-parser/src/vector.c)

add_library(gumbo STATIC ${GUMBO_SOURCES})
include_directories(~/gumbo-parser/src)

add_library(pub
publisher_base.hpp
gazeta_esportiva.hpp
runner.cpp
)

set_target_properties(pub 
PROPERTIES 
LINKER_LANGUAGE CXX)

include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 0817715923c9705e68994eb52ef9df3f6845beba) # The commit hash for 1.10.x. Replace with the latest from: https://github.com/libcpr/cpr/releases
FetchContent_MakeAvailable(cpr)

target_link_libraries(pub cpr::cpr)
