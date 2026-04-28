#include <algorithm>
#include "MatchingAlgorithm.hpp"

bool MatchingAlgorithm::anyOrdersToMatch()
{
    if (!Bids.empty() && !Asks.empty())
    {
        return true;
    }
    return false;
}

bool MatchingAlgorithm::validateOrdersToMatch()
{
    if (Bids.empty() || Asks.empty())
        return false;

    const Order &topBuyOrder = Bids.begin()->second;
    const Order &topAskOrder = Asks.begin()->second;

    return topBuyOrder.getPrice() >= topAskOrder.getPrice();
}

void MatchingAlgorithm::FillOrder()
{
    if (Bids.empty() || Asks.empty())
        return;

    Order &topBuyOrder = Bids.begin()->second;
    Order &topAskOrder = Asks.begin()->second;

    int tradeQty = std::min(topBuyOrder.getQuantity(),
                            topAskOrder.getQuantity());

    double executionPrice = topAskOrder.getPrice(); // passive side buy comes to asking price

    LogTrade(topBuyOrder.getID(),
             topAskOrder.getID(),
             executionPrice,
             tradeQty);

    int remainingBuy = topBuyOrder.getQuantity() - tradeQty;
    int remainingSell = topAskOrder.getQuantity() - tradeQty;

    topBuyOrder.newQuantity(std::max(0, remainingBuy));
    topAskOrder.newQuantity(std::max(0, remainingSell));
}
void MatchingAlgorithm::MakeQuantityNonNegative(double &remainingQuantityBids, double &remainingQuanityAsks)
{
    remainingQuantityBids = std::max(0.0, remainingQuantityBids);
    remainingQuanityAsks = std::max(0.0, remainingQuanityAsks);
}

void MatchingAlgorithm::CleanOrders()
{
    while (!Bids.empty() && Bids.begin()->second.getQuantity() <= 0)
    {
        auto it = Bids.begin();
        uint64_t id = it->second.getID();
        Bids.erase(it);
        bidIterators.erase(id);
    }

    while (!Asks.empty() && Asks.begin()->second.getQuantity() <= 0)
    {
        auto it = Asks.begin();
        uint64_t id = it->second.getID();
        Asks.erase(it);
        askIterators.erase(id);
    }
}

void MatchingAlgorithm::matchTopOfBook()
{
    while (anyOrdersToMatch() && validateOrdersToMatch())
    {
        FillOrder();
        CleanOrders();
    }
}

void MatchingAlgorithm::LogTrade(uint64_t bidID,uint64_t askID, double priceExectued,int quantity)
{
    logger->logTrade( bidID , askID, priceExectued, quantity);
}
