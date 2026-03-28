set(This tests)

set(Sources tests.cpp)

add_executable(${This} ${Sources})

target_link_libraries(${This}
    PUBLIC
    gtest_main
    OrderBook
)

add_test(NAME ${This} COMMAND ${This})