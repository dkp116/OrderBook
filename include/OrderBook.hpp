#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include "Order.hpp"
#include <deque>

class OrderBook
{
private:
    std::deque<Order> Bids;
    std::deque<Order> Asks;

public:
    OrderBook() {};

    void AddOrder(const Order &newOrderPlaced);
    void ReOrderQueue(std::deque<Order> &orders);

    std::deque<Order> &getBids() { return Bids; }
    std::deque<Order> &getAsks() { return Asks; }

    const std::deque<Order> &getBids() const { return Bids; }
    const std::deque<Order> &getAsks() const { return Asks; }
};

#endif