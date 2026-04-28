#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP

#include "Order.hpp"
#include "MatchingAlgorithm.hpp"
#include "SpdlogLogger.hpp"
#include <map>
#include <unordered_map>
#include <functional>
#include <memory>

class OrderBook
{
private:
    std::multimap<double, Order, std::greater<double>> Bids;
    std::multimap<double, Order, std::less<double>> Asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator> bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator> askIterators;
    std::shared_ptr<ILogger> logger;

public:
    OrderBook()
        : logger(std::make_shared<SpdlogLogger>()) {}

    void AddOrder(const Order &newOrderPlaced);
    void MatchOrders();
    void addOrderAndMatch(const Order &newOrderPlaced);
    bool CancelOrder(uint64_t orderID);
    const std::multimap<double, Order, std::greater<double>>& getBidsMap() const { return Bids; }
    const std::multimap<double, Order, std::less<double>>& getAsksMap() const { return Asks; }
};

#endif