set(This OrderBookTests)

set(Sources OrderBookTests.cpp MatchingAlgorithmTests.cpp)

add_executable(${This} ${Sources})

target_link_libraries(${This}
    PUBLIC
    gtest_main
    OrderBook
    MatchingAlgorithm
    
)

include(GoogleTest)

gtest_discover_tests(${This})