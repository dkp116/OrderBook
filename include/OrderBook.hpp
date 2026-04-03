#ifndef ORDERBOOK_HPP
#define ORDERBOOK_HPP
#include "Order.hpp"
#include <deque>

class OrderBook{

    private:

    std::deque<int> buyOrders;
    std::deque<int> SellOrders;

    public:
    
    OrderBook();


    


};


#endif 