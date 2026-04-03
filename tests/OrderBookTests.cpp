#include <gtest/gtest.h>

#include "Order.hpp"
#include "OrderBook.hpp"

TEST(OrderTest, ConstructorStoresTypeCorrectly)
{
    Order order(BUY, 5, 100.0);

    EXPECT_EQ(order.getType(), BUY);
}

TEST(OrderTest, ConstructorStoresPriceCorrectly)
{
    Order order(SELL, 3, 101.5);

    EXPECT_DOUBLE_EQ(order.getPrice(), 101.5);
}

TEST(OrderTest, ConstructorStoresQuantityCorrectly)
{
    Order order(BUY, 7, 99.0);

    EXPECT_EQ(order.getQuantity(), 7);
}

TEST(OrderBookTest, BuyOrderAddedToBuySide)
{
    OrderBook ob;

    Order order(BUY, 5, 100.0);

    ob.AddOrder(order);

    EXPECT_EQ(ob.getBids().size(), 1);
    EXPECT_EQ(ob.getAsks().size(), 0);
}

TEST(OrderBookTest, SellOrderAddedToSellSide)
{
    OrderBook ob;

    Order order(SELL, 2, 105.0);

    ob.AddOrder(order);

    EXPECT_EQ(ob.getBids().size(), 0);
    EXPECT_EQ(ob.getAsks().size(), 1);
}

TEST(OrderBookTest, MultipleOrdersAddedCorrectly)
{
    OrderBook ob;

    Order buyOrder(BUY, 1, 100.0);
    Order sellOrder(SELL, 2, 105.0);

    ob.AddOrder(buyOrder);
    ob.AddOrder(sellOrder);

    EXPECT_EQ(ob.getBids().size(), 1);
    EXPECT_EQ(ob.getAsks().size(), 1);
}

TEST(OrderBookTest, ReorderWhenMultipleBuyOrders)
{
    OrderBook ob;

    ob.AddOrder(Order(BUY, 1, 100.0));
    ob.AddOrder(Order(BUY, 1, 105.0));
    ob.AddOrder(Order(BUY, 1, 102.0));

    EXPECT_EQ(ob.getBids().size(), 3);

    EXPECT_DOUBLE_EQ(ob.getBids()[0].getPrice(), 105.0);
    EXPECT_DOUBLE_EQ(ob.getBids()[1].getPrice(), 102.0);
    EXPECT_DOUBLE_EQ(ob.getBids()[2].getPrice(), 100.0);
}

TEST(OrderBookTest, ReorderWhenMultipleSellOrders)
{
    OrderBook ob;

    ob.AddOrder(Order(SELL, 1, 100.0));
    ob.AddOrder(Order(SELL, 1, 105.0));
    ob.AddOrder(Order(SELL, 1, 102.0));

    EXPECT_EQ(ob.getAsks().size(), 3);

    EXPECT_DOUBLE_EQ(ob.getAsks()[0].getPrice(), 100.0);
    EXPECT_DOUBLE_EQ(ob.getAsks()[1].getPrice(), 102.0);
    EXPECT_DOUBLE_EQ(ob.getAsks()[2].getPrice(), 105.0);
}