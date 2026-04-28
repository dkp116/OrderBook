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

add_library(Order src/Order.cpp)
target_include_directories(Order PUBLIC include)

add_library(MatchingAlgorithm src/MatchingAlgorithm.cpp)
target_include_directories(MatchingAlgorithm PUBLIC include)

add_library(Logger src/SpdlogLogger.cpp)
target_include_directories(Logger PUBLIC include)

# spdlog linked where it is used
target_link_libraries(Logger PUBLIC spdlog::spdlog)

add_library(Server src/Server.cpp)
# target_include_directories(Server PUBLIC include)
target_link_libraries(Server
    PUBLIC
        OrderBook
        Order
        MatchingAlgorithm
        Logger
)

# -----------------------
# App
# -----------------------
add_executable(OrderBookApp src/main.cpp)
target_link_libraries(OrderBookApp PRIVATE Order OrderBook MatchingAlgorithm Logger Server)

# -----------------------
# Tests
# -----------------------
add_subdirectory(tests)