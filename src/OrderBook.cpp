#include <algorithm>
#include "OrderBook.hpp"

void OrderBook::AddOrder(const Order &newOrderPlaced)
{
    if (newOrderPlaced.getType() == Type::BUY)
    {
        auto it = Bids.insert({newOrderPlaced.getPrice(), newOrderPlaced});
        bidIterators[newOrderPlaced.getID()] = it;
    }
    else
    {
        auto it = Asks.insert({newOrderPlaced.getPrice(), newOrderPlaced});
        askIterators[newOrderPlaced.getID()] = it;
    }
}

void OrderBook::MatchOrders()
{
    MatchingAlgorithm matcher(Bids, Asks, bidIterators, askIterators, logger);
    matcher.matchTopOfBook();
}

void OrderBook::addOrderAndMatch(const Order &newOrderPlaced){
    AddOrder(newOrderPlaced);
    MatchOrders();
}

bool OrderBook::CancelOrder(uint64_t orderID)
{
    auto bidIt = bidIterators.find(orderID);
    if (bidIt != bidIterators.end())
    {
        Bids.erase(bidIt->second);
        bidIterators.erase(bidIt);
        return true;
    }
    auto askIt = askIterators.find(orderID);
    if (askIt != askIterators.end())
    {
        Asks.erase(askIt->second);
        askIterators.erase(askIt);
        return true;
    }
    return false;
}