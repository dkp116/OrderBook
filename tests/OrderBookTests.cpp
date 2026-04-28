#include <gtest/gtest.h>

#include "Order.hpp"
#include "OrderBook.hpp"

// --------------------
// ORDER TESTS
// --------------------

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

TEST(OrderTest, GetOrderIDIsMonotonic)
{
    Order order1(BUY, 7, 99.0);
    Order order2(BUY, 7, 100.0);

    EXPECT_LT(order1.getID(), order2.getID());
}

// --------------------
// ORDERBOOK TESTS
// --------------------

TEST(OrderBookTest, BuyOrderAddedToBuySide)
{
    OrderBook ob;

    ob.AddOrder(Order(BUY, 5, 100.0));

    EXPECT_EQ(ob.getBidsMap().size(), 1);
    EXPECT_EQ(ob.getAsksMap().size(), 0);
}

TEST(OrderBookTest, SellOrderAddedToSellSide)
{
    OrderBook ob;

    ob.AddOrder(Order(SELL, 2, 105.0));

    EXPECT_EQ(ob.getBidsMap().size(), 0);
    EXPECT_EQ(ob.getAsksMap().size(), 1);
}

TEST(OrderBookTest, MultipleOrdersAddedCorrectly)
{
    OrderBook ob;

    ob.AddOrder(Order(BUY, 1, 100.0));
    ob.AddOrder(Order(SELL, 2, 105.0));

    EXPECT_EQ(ob.getBidsMap().size(), 1);
    EXPECT_EQ(ob.getAsksMap().size(), 1);
}

TEST(OrderBookTest, ReorderWhenMultipleBuyOrders)
{
    OrderBook ob;

    ob.AddOrder(Order(BUY, 1, 100.0));
    ob.AddOrder(Order(BUY, 1, 105.0));
    ob.AddOrder(Order(BUY, 1, 102.0));

    auto it = ob.getBidsMap().begin();
    EXPECT_DOUBLE_EQ(it->second.getPrice(), 105.0);
    ++it;
    EXPECT_DOUBLE_EQ(it->second.getPrice(), 102.0);
    ++it;
    EXPECT_DOUBLE_EQ(it->second.getPrice(), 100.0);
}

TEST(OrderBookTest, ReorderWhenMultipleSellOrders)
{
    OrderBook ob;

    ob.AddOrder(Order(SELL, 1, 100.0));
    ob.AddOrder(Order(SELL, 1, 105.0));
    ob.AddOrder(Order(SELL, 1, 102.0));

    auto it = ob.getAsksMap().begin();
    EXPECT_DOUBLE_EQ(it->second.getPrice(), 100.0);
    ++it;
    EXPECT_DOUBLE_EQ(it->second.getPrice(), 102.0);
    ++it;
    EXPECT_DOUBLE_EQ(it->second.getPrice(), 105.0);
}

TEST(OrderBookTest, AddOrderAndMatchTriggersMatching)
{
    OrderBook ob;

    ob.addOrderAndMatch(Order(BUY, 1, 120.0));
    ob.addOrderAndMatch(Order(SELL, 1, 110.0));

    EXPECT_TRUE(ob.getBidsMap().empty());
    EXPECT_TRUE(ob.getAsksMap().empty());
}

TEST(OrderBookTest, NoMatchWhenPricesDoNotCross)
{
    OrderBook ob;

    ob.addOrderAndMatch(Order(BUY, 1, 100.0));
    ob.addOrderAndMatch(Order(SELL, 1, 110.0));

    EXPECT_EQ(ob.getBidsMap().size(), 1);
    EXPECT_EQ(ob.getAsksMap().size(), 1);
}

TEST(OrderBookTest, PartialFillKeepsRemainingQuantity)
{
    OrderBook ob;

    ob.AddOrder(Order(BUY, 5, 120.0));
    ob.addOrderAndMatch(Order(SELL, 2, 110.0));

    EXPECT_EQ(ob.getBidsMap().begin()->second.getQuantity(), 3);
    EXPECT_TRUE(ob.getAsksMap().empty());
}

TEST(OrderBookTest, CancelOrderRemovesBuyOrder)
{
    OrderBook ob;

    Order buyOrder(BUY, 5, 100.0);
    ob.AddOrder(buyOrder);

    bool canceled = ob.CancelOrder(buyOrder.getID());

    EXPECT_TRUE(canceled);
    EXPECT_TRUE(ob.getBidsMap().empty());
}

TEST(OrderBookTest, CancelOrderRemovesSellOrder)
{
    OrderBook ob;

    Order sellOrder(SELL, 3, 105.0);
    ob.AddOrder(sellOrder);

    bool canceled = ob.CancelOrder(sellOrder.getID());

    EXPECT_TRUE(canceled);
    EXPECT_TRUE(ob.getAsksMap().empty());
}

TEST(OrderBookTest, CancelNonExistentOrderReturnsFalse)
{
    OrderBook ob;

    bool canceled = ob.CancelOrder(99999);

    EXPECT_FALSE(canceled);
}

TEST(OrderBookTest, CancelOrderPreventsMatching)
{
    OrderBook ob;

    Order buyOrder(BUY, 1, 120.0);
    ob.AddOrder(buyOrder);
    Order sellOrder(SELL, 1, 110.0);
    ob.AddOrder(sellOrder);

    // Orders are added but not matched yet
    EXPECT_EQ(ob.getBidsMap().size(), 1);
    EXPECT_EQ(ob.getAsksMap().size(), 1);

    // Cancel the buy order
    bool canceled = ob.CancelOrder(buyOrder.getID());
    EXPECT_TRUE(canceled);

    // Now match - should not match since buy is gone
    ob.MatchOrders();

    EXPECT_TRUE(ob.getBidsMap().empty());
    EXPECT_EQ(ob.getAsksMap().size(), 1);  // Sell order still there
}