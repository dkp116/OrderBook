cmake_minimum_required(VERSION 3.28)
project(OrderBook)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

# -----------------------
# GoogleTest
# -----------------------
add_subdirectory(googletest)

# -----------------------
# spdlog
# -----------------------
include(FetchContent)

FetchContent_Declare(
    spdlog
    URL https://github.com/gabime/spdlog/archive/refs/heads/v1.x.zip
)

FetchContent_MakeAvailable(spdlog)

# -----------------------
# Core libraries
# -----------------------
add_library(OrderBook src/OrderBook.cpp)
target_include_directories(OrderBook PUBLIC include)

add_library(MatchingAlgorithm src/MatchingAlgorithm.cpp)
target_include_directories(MatchingAlgorithm PUBLIC include)

add_library(Logger src/SpdlogLogger.cpp)
target_include_directories(Logger PUBLIC include)

# spdlog linked where it is used
target_link_libraries(Logger PUBLIC spdlog::spdlog)

# -----------------------
# App
# -----------------------
add_executable(OrderBookApp src/main.cpp)
target_link_libraries(OrderBookApp PRIVATE OrderBook MatchingAlgorithm Logger)

# -----------------------
# Tests
# -----------------------
add_subdirectory(tests)