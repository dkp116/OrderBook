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

    const Order &topBuyOrder = Bids.front();
    const Order &topAskOrder = Asks.front();

    return topBuyOrder.getPrice() >= topAskOrder.getPrice();
}

void MatchingAlgorithm::FillOrder()
{
    if (Bids.empty() || Asks.empty())
        return;

    Order &topBuyOrder = Bids.front();
    Order &topAskOrder = Asks.front();

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
    while (!Bids.empty() && Bids.front().getQuantity() <= 0)
    {
        Bids.pop_front();
    }

    while (!Asks.empty() && Asks.front().getQuantity() <= 0)
    {
        Asks.pop_front();
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
