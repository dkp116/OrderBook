cmake_minimum_required(VERSION 3.28)
project(OrderBook)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

add_subdirectory(googletest)
include(FetchContent)

FetchContent_Declare(
    spdlog
    URL https://github.com/gabime/spdlog/archive/refs/heads/v1.x.zip
)

FetchContent_MakeAvailable(spdlog)


add_library(OrderBook   src/OrderBook.cpp)
target_include_directories(OrderBook PUBLIC include)

add_library(MatchingAlgorithm   src/MatchingAlgorithm.cpp)
target_include_directories(MatchingAlgorithm PUBLIC include)
target_link_libraries(MatchingAlgorithm PUBLIC spdlog::spdlog)

add_executable(OrderBookApp src/main.cpp)
target_link_libraries(OrderBookApp PRIVATE OrderBook MatchingAlgorithm)


add_subdirectory(tests)