# OrderBook Trading System

A high-performance C++ implementation of an electronic order book for trading systems. This project demonstrates core trading engine concepts including order management, price-time priority matching, and order cancellation.

## Features

- **Order Management**: Add buy and sell orders with automatic price-based sorting
- **Fast Order Cancellation**: O(1) order cancellation by order ID using hash maps
- **Price-Time Priority Matching**: Orders match when buy price ≥ sell price
- **Partial Order Filling**: Supports partial fills with remaining quantity tracking
- **Comprehensive Logging**: Trade execution logging with spdlog
- **Thread-Safe Design**: Uses atomic counters for order ID generation
- **Unit Testing**: Full test coverage with GoogleTest

## Architecture

### Core Components

- **`Order`**: Represents individual buy/sell orders with ID, price, quantity, and timestamp
- **`OrderBook`**: Main order book managing bids (buy orders) and asks (sell orders)
- **`MatchingAlgorithm`**: Implements order matching logic with price crossing detection
- **`ILogger`**: Interface for logging trade executions
- **`SpdlogLogger`**: Concrete logger implementation using spdlog library

### Data Structures

- **Bids**: `std::multimap<double, Order, std::greater<double>>` - Highest price first
- **Asks**: `std::multimap<double, Order, std::less<double>>` - Lowest price first
- **Order Lookup**: `std::unordered_map<uint64_t, iterator>` - O(1) order cancellation

## Implementation Details

### Order Book Operations

#### Adding Orders
```cpp
OrderBook orderBook;
Order buyOrder(BUY, 100, 50.0);  // Buy 100 shares at $50.00
orderBook.AddOrder(buyOrder);
```

Orders are automatically sorted by price:
- Buy orders: Highest price at front (best bid)
- Sell orders: Lowest price at front (best ask)

#### Order Matching
```cpp
orderBook.addOrderAndMatch(sellOrder);  // Add and attempt matching
```

Matching occurs when:
- There are orders on both sides
- Best bid price ≥ Best ask price

Trade execution:
- Quantity = min(buy_quantity, sell_quantity)
- Price = ask_price (price priority to passive side)
- Partial fills update remaining quantities
- Fully filled orders are removed

#### Order Cancellation
```cpp
bool success = orderBook.CancelOrder(orderID);
```

- O(1) lookup using hash map
- Removes order from price levels
- Updates internal iterators
- Returns true if order existed and was canceled

### Example Usage

```cpp
#include "OrderBook.hpp"

int main() {
    OrderBook orderBook;

    // Add buy order
    Order buyOrder(BUY, 100, 50.0);
    orderBook.AddOrder(buyOrder);

    // Add sell order that crosses
    Order sellOrder(SELL, 50, 49.0);
    orderBook.addOrderAndMatch(sellOrder);
    // Trade executes: 50 shares at $49.00
    // Buy order has 50 shares remaining

    // Cancel remaining buy order
    orderBook.CancelOrder(buyOrder.getID());

    return 0;
}
```

## Building and Running

### Prerequisites

- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10+
- spdlog library (automatically downloaded via CMake)

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/yourusername/orderbook.git
cd orderbook

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
make

# Run tests
make test
# or
ctest
```

### Running the Application

```bash
# Show help
./OrderBookApp --help

# Run as TCP server on default port 8080
./OrderBookApp --server

# Run as TCP server on custom port
./OrderBookApp --server --port 9090

# Run specific tests
ctest -R OrderBookTest
```

### TCP Server Mode

The OrderBook can run as a TCP server to accept order submissions over the network:

#### Starting the Server

```bash
# Start server on default port 8080
./OrderBookApp --server

# Start server on custom port
./OrderBookApp --server --port 9090
```

#### Sending Orders via TCP

Connect to the server using any TCP client (telnet, netcat, or custom client) and send orders in CSV format:

```bash
# Using netcat
echo "BUY,100.50,50" | nc localhost 8080

# Using telnet
telnet localhost 8080
BUY,100.50,50
```

**Message Format:**
```
TYPE,PRICE,QUANTITY
```

- **TYPE**: "BUY" or "SELL"
- **PRICE**: Double precision price (e.g., 100.50)
- **QUANTITY**: Integer quantity (e.g., 50)

**Examples:**
```bash
# Buy 100 shares at $50.25
echo "BUY,50.25,100" | nc localhost 8080

# Sell 75 shares at $50.50
echo "SELL,50.50,75" | nc localhost 8080
```

**Server Response:**
- `OK` - Order accepted and processed
- `ERROR` - Invalid message format or processing error

#### Server Implementation Details

- **Threading**: Each client connection handled in separate thread
- **Thread Safety**: OrderBook operations protected by mutex
- **Shared State**: Single OrderBook instance shared across all connections
- **Matching**: Automatic order matching on each new order submission
- **Logging**: All trades logged via spdlog

#### Testing the TCP Server

```bash
# Terminal 1: Start the server
./OrderBookApp --server

# Terminal 2: Submit orders
echo "BUY,100.50,50" | nc localhost 8080
echo "SELL,100.25,25" | nc localhost 8080

# Check server logs for trade execution
# Press Enter in Terminal 1 to stop the server
```

### Running Tests

The project includes comprehensive unit tests covering:
- Order creation and validation
- Order book operations (add, match, cancel)
- Matching algorithm logic
- Edge cases (partial fills, no matches, cancellations)

```bash
# Run all tests
ctest

# Run specific test suite
ctest -R OrderBookTest

# Run with verbose output
ctest -V
```

## Performance Characteristics

- **Order Addition**: O(log n) - multimap insertion
- **Order Cancellation**: O(1) - hash map lookup
- **Order Matching**: O(1) amortized - checks top of books
- **Memory Usage**: O(n) for n orders

## Design Decisions

### Why Multimaps for Price Levels?
- Automatic sorting by price
- Support for multiple orders at same price
- Logarithmic insertion/removal complexity

### Why Separate Hash Maps for Cancellation?
- Enables O(1) order lookup by ID
- Critical for high-frequency trading systems
- Maintains iterator validity during operations

### Why Price Priority Matching?
- Industry standard for most exchanges
- Passive side (resting order) sets execution price
- Encourages liquidity provision

## Future Enhancements

- Time priority for same-price orders
- Market orders (no price limit)
- Stop orders
- Order modification (change quantity/price)
- Multi-threaded matching engine
- Network protocol for order submission
- Persistence layer for order state

## Dependencies

- **spdlog**: High-performance logging library
- **GoogleTest**: Unit testing framework
- **CMake**: Build system

## Contributing

1. Fork the repository
2. Create a feature branch
3. Add tests for new functionality
4. Ensure all tests pass
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.
