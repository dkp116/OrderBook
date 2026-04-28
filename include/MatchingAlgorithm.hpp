#ifndef MATCHINGALGORITHM_HPP
#define MATCHINGALGORITHM_HPP

#include "Order.hpp"
#include "SpdlogLogger.hpp"
#include <map>
#include <unordered_map>
#include <functional>
#include <memory>
#include <cstdint>

class MatchingAlgorithm
{
private:
    std::multimap<double, Order, std::greater<double>>& Bids;
    std::multimap<double, Order, std::less<double>>& Asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator>& bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator>& askIterators;
    std::shared_ptr<ILogger> logger;

public:
    MatchingAlgorithm(std::multimap<double, Order, std::greater<double>>& Bids_, std::multimap<double, Order, std::less<double>>& Asks_ , std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator>& bidIterators_, std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator>& askIterators_, std::shared_ptr<ILogger> logger_) : Bids(Bids_), Asks(Asks_), bidIterators(bidIterators_), askIterators(askIterators_), logger(logger_) {};
    bool anyOrdersToMatch();
    bool validateOrdersToMatch();
    void FillOrder();
    void MakeQuantityNonNegative(double &remainingQuantityBids, double &remainingQuanityAsks);
    void CleanOrders();
    void matchTopOfBook();
    void LogTrade(uint64_t bidID, uint64_t askID, double priceExectued, int quantity);
};

#endif