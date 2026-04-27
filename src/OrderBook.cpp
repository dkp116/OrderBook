#include <algorithm>
#include "OrderBook.hpp"

void OrderBook::AddOrder(const Order &newOrderPlaced)
{
    if (newOrderPlaced.getType() == Type::BUY)
    {
        Bids.push_back(newOrderPlaced);
        ReOrderQueue(Bids);
    }
    else
    {
        Asks.push_back(newOrderPlaced);
        ReOrderQueue(Asks);
    }
}

void OrderBook::ReOrderQueue(std::deque<Order> &orders)
{
    if (orders.empty())
        return;

    Type type = orders.front().getType();

    if (type == Type::BUY)
    {
        std::sort(orders.begin(), orders.end(),
                  [](const Order &a, const Order &b)
                  {
                      return a.getPrice() > b.getPrice();
                  });
    }
    else
    {
        std::sort(orders.begin(), orders.end(),
                  [](const Order &a, const Order &b)
                  {
                      return a.getPrice() < b.getPrice();
                  });
    }
}

void OrderBook::MatchOrders()
{
    MatchingAlgorithm matcher(Bids, Asks, logger);
    matcher.matchTopOfBook();
}

void OrderBook::addOrderAndMatch(const Order &newOrderPlaced){
    AddOrder(newOrderPlaced);
    MatchOrders();
}