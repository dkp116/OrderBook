#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP
#include "Order.hpp"
#include <deque>

class OrderBook{

    private:

    std::deque<Order> buyOrders;
    std::deque<Order> sellOrders;

    public:
    
    OrderBook();
    void AddOrder(Order newOrderPlaced);


    


};


#endif 