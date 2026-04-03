cmake_minimum_required(VERSION 3.28)
project(OrderBook)

set(CMAKE_CXX_STANDARD 17)
enable_testing()

add_subdirectory(googletest)


add_library(OrderBook   src/OrderBook.cpp)
target_include_directories(OrderBook PUBLIC include)


add_executable(OrderBookApp src/main.cpp)
target_link_libraries(OrderBookApp PRIVATE OrderBook)


add_subdirectory(tests)