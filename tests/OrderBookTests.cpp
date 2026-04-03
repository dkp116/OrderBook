#include <gtest/gtest.h>

#include "Order.hpp"
#include "OrderBook.hpp"

#include <map>


TEST(OrderTest, ConstructorStoresTypeCorrectly)
{
    std::map<int,double> qp = {{100, 5.0}};

    Order order(BUY, qp);

    EXPECT_EQ(order.getType(), BUY);
}


TEST(OrderTest, ConstructorStoresMapCorrectly)
{
    std::map<int,double> qp = {
        {100, 5.0},
        {101, 3.0}
    };

    Order order(SELL, qp);

    EXPECT_EQ(order.getQuantityAndPrice().size(), 2);
    EXPECT_EQ(order.getQuantityAndPrice().at(100), 5.0);
    EXPECT_EQ(order.getQuantityAndPrice().at(101), 3.0);
}


TEST(OrderBookTest, BuyOrderAddedToBuySide)
{
    OrderBook ob;

    std::map<int,double> qp = {{100, 5.0}};
    Order order(BUY, qp);

    ob.AddOrder(order);

    EXPECT_EQ(ob.getBuyOrders().size(), 1);
    EXPECT_EQ(ob.getSellOrders().size(), 0);
}


TEST(OrderBookTest, SellOrderAddedToSellSide)
{
    OrderBook ob;

    std::map<int,double> qp = {{105, 2.0}};
    Order order(SELL, qp);

    ob.AddOrder(order);

    EXPECT_EQ(ob.getBuyOrders().size(), 0);
    EXPECT_EQ(ob.getSellOrders().size(), 1);
}


TEST(OrderBookTest, MultipleOrdersAddedCorrectly)       // I would say this is an intergration test could try and mock?
{
    OrderBook ob;

    Order buyOrder(BUY, {{100, 1.0}});
    Order sellOrder(SELL, {{105, 2.0}});

    ob.AddOrder(buyOrder);
    ob.AddOrder(sellOrder);

    EXPECT_EQ(ob.getBuyOrders().size(), 1);
    EXPECT_EQ(ob.getSellOrders().size(), 1);
}