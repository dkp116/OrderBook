#ifndef ORDER_HPP
#define ORDER_HPP

#include <atomic>
#include <chrono>

enum Type
{
    BUY,
    SELL
};

class Order
{
private:
    static std::atomic<uint64_t> next_id;

    Type typeofOrder;
    int quantity_;
    double price_;
    std::chrono::high_resolution_clock::time_point time_;
    uint64_t orderID;

public:
    Order(Type typeofOrder, int quantity, double price)
        : typeofOrder(typeofOrder),
          quantity_(quantity),
          price_(price),
          time_(std::chrono::high_resolution_clock::now()),
          orderID(++next_id) {}

    Type getType() const { return typeofOrder; }
    double getPrice() const { return price_; }
    int getQuantity() const { return quantity_; }
    uint64_t getID() const { return orderID; }

    void newQuantity(int remainingQuantity) { quantity_ = remainingQuantity; }
};

#endif