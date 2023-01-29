set(PASSMG_CLI_SRCS
    ./utils.cpp
    ./utils.hpp
    ./main.cpp
)

FetchContent_Declare(
    cxxopts
    GIT_REPOSITORY https://github.com/jarro2783/cxxopts
    GIT_TAG v3.0.0
)

FetchContent_MakeAvailable(cxxopts)

add_executable(passmg_cli ${PASSMG_CLI_SRCS})
set_target_properties(passmg_cli PROPERTIES OUTPUT_NAME passmg)
target_link_libraries(passmg_cli 
    PUBLIC libpassmg 
    PUBLIC cxxopts
)
