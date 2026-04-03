#include "OrderBook.hpp"

void OrderBook::AddOrder(Order newOrderPlaced){
    Type TypeofOrder = newOrderPlaced.getType();
    if(TypeofOrder == Type::BUY){
        buyOrders.push_back(newOrderPlaced);
    }

    else{
        sellOrders.push_back(newOrderPlaced);

    }

    return;
}