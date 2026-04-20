#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include "Order.hpp"
#include "MatchingAlgorithm.hpp"
#include "SpdlogLogger.hpp"
#include <deque>
#include <memory>

class OrderBook
{
private:
    std::deque<Order> Bids;
    std::deque<Order> Asks;
    std::shared_ptr<ILogger> logger;
    MatchingAlgorithm Matcher;
    

    public : OrderBook() :   logger(std::make_shared<SpdlogLogger>()),   Matcher(Bids, Asks,logger) {};

    void AddOrder(const Order &newOrderPlaced);
    void ReOrderQueue(std::deque<Order> &orders);

    std::deque<Order> &getBids() { return Bids; }
    std::deque<Order> &getAsks() { return Asks; }

    const std::deque<Order> &getBids() const { return Bids; }
    const std::deque<Order> &getAsks() const { return Asks; }

    void MatchOrders();
    void addOrderAndMatch(const Order &newOrderPlaced);
};

#endif