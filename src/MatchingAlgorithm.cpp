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

    const Order& topBuyOrder = Bids.front();
    const Order& topAskOrder = Asks.front();

    return topBuyOrder.getPrice() >= topAskOrder.getPrice();
}

void MatchingAlgorithm::FillOrder()
{
    if (Bids.empty() || Asks.empty())
        return;

    const Order& topBuyOrder = Bids.front();
    const Order& topAskOrder = Asks.front();

    int tradeQty = std::min(topBuyOrder.getQuantity(), topAskOrder.getQuantity());

    LogTrade(topBuyOrder.getPrice(), topAskOrder.getPrice(), tradeQty);

    double remainingBuy  = topBuyOrder.getQuantity() - tradeQty;
    double remainingSell = topAskOrder.getQuantity() - tradeQty;

    Bids.front().newQuantity(std::max(0.0, remainingBuy));
    Asks.front().newQuantity(std::max(0.0, remainingSell));
}

void MatchingAlgorithm::MakeQuantityNonNegative(double &remainingQuantityBids, double &remainingQuanityAsks)
{
    remainingQuantityBids = std::max(0.0, remainingQuantityBids);
    remainingQuanityAsks  = std::max(0.0, remainingQuanityAsks);
}

void MatchingAlgorithm::CleanOrders()
{
    while (!Bids.empty() && Bids.front().getQuantity() <= 0)
    {
        Bids.pop_front();
    }

    while (!Asks.empty() && Asks.front().getQuantity() <= 0)
    {
        Asks.pop_front();
    }
}

void MatchingAlgorithm::matchTopOfBook(){
    while (anyOrdersToMatch() && validateOrdersToMatch())
    {
        FillOrder();
        CleanOrders();
    }
}

void MatchingAlgorithm::LogTrade(double bidPrice, double askPrice,int quantity){
    logger->logTrade(bidPrice,askPrice,quantity);
}
