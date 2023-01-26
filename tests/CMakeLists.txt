project (passmg_test)

set(PASSMG_TEST_SRCS 
    ./utils.test.cpp
    # ./crypt.test.cpp
)

FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/GoogleTest
    GIT_TAG v1.12.0
)

FetchContent_MakeAvailable(googletest)

include(GoogleTest)

add_executable(
    passmg_test 
    ${PASSMG_TEST_SRCS}
)

target_link_libraries(
    passmg_test
    GTest::gtest_main
    libpassmg
)

gtest_discover_tests(passmg_test)
