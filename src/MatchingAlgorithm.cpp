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
    Order topBuyOrder = Bids.front();
    Order topAskOrder = Asks.front();
    if (topBuyOrder.getPrice() >= topAskOrder.getPrice())
    {
        return true;
    }
    return false;
}

void MatchingAlgorithm::FillOrder(){
    Order& topBuyOrder = Bids.front();           
    Order& topAskOrder = Asks.front();
    double remainingQuantityBids = topBuyOrder.getQuantity() - topAskOrder.getQuantity();
    double remainingQuanityAsks = -remainingQuantityBids;
    MakeQuantityNonNegative(remainingQuantityBids, remainingQuanityAsks);
    topBuyOrder.newQuantity(remainingQuantityBids);
    topAskOrder.newQuantity(remainingQuanityAsks);
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
