#ifndef ORDER_HPP
#define ORDER_HPP

#include <map>

enum Type
{
    BUY,
    SELL
};

class Order
{
private:
    std::map<int, double> quantityAndPrice;
    Type typeofOrder;

public:

    Order(Type typeofOrder, std::map<int, double> quantityAndPrice)
        : typeofOrder(typeofOrder), quantityAndPrice(quantityAndPrice) {}


    Type getType() const { return typeofOrder; }
    const std::map<int, double>& getQuantityAndPrice() const { return quantityAndPrice; }
};

#endif