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
    Type typeofOrder;
    int quantity_;
    double price_;
    double time_;

public:
    Order(Type typeofOrder, int quantity, double price)
        : typeofOrder(typeofOrder), quantity_(quantity), price_(price) {}

    Type getType() const { return typeofOrder; }
    double getPrice() const { return price_; }
    double getQuantity() const { return quantity_; }
    void newQuantity(double remainingQuantity){quantity_ = remainingQuantity;}
};

#endif