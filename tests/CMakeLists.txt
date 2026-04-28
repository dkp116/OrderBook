set(This OrderBookTests)

set(Sources OrderBookTests.cpp MatchingAlgorithmTests.cpp ServerTest.cpp)

add_executable(${This} ${Sources})

target_link_libraries(${This}
    PUBLIC
    gtest_main
    Order
    OrderBook
    MatchingAlgorithm
    Logger
    Server
    
    
)

include(GoogleTest)

gtest_discover_tests(${This})