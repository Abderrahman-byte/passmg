include(FindOpenSSL)

set(LIBPASSMG_SOURCES 
    ./crypt.cpp
    ./utils.cpp
    ./PasswordManager.cpp
)

find_library(SQLITE_LIB sqlite3 REQUIRED)

add_library(libpassmg ${LIBPASSMG_SOURCES})
target_include_directories(libpassmg 
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${PROJECT_BINARY_DIR}/include
)
set_target_properties(libpassmg
    PROPERTIES OUTPUT_NAME passmg
)

target_link_libraries(libpassmg 
    PUBLIC OpenSSL::Crypto
    PUBLIC ${SQLITE_LIB}
)
