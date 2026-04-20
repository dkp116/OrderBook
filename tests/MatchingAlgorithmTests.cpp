#include <gtest/gtest.h>

#include "Order.hpp"
#include "MatchingAlgorithm.hpp"
#include "SpdlogLogger.hpp"

TEST(MatchingAlgorithmTest, ReturnsFalseWhenBothSidesEmpty)
{
    std::deque<Order> bids;
    std::deque<Order> asks;
    std::shared_ptr<ILogger> log;

    MatchingAlgorithm algo(bids, asks,log);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, ReturnsFalseWhenAsksEmpty)
{
    std::deque<Order> bids = {Order(BUY, 1, 100.0)};
    std::deque<Order> asks;
    std::shared_ptr<ILogger> log;

    MatchingAlgorithm algo(bids, asks, log);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, ReturnsFalseWhenBidsEmpty)
{
    std::deque<Order> bids;
    std::deque<Order> asks = {Order(SELL, 1, 100.0)};
    std::shared_ptr<ILogger> log;

    MatchingAlgorithm algo(bids, asks, log);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, OrdersCanBeFilled)
{
    std::deque<Order> bids = {Order(BUY, 1, 100.0)};
    std::deque<Order> asks = {Order(SELL, 1, 100.0)};
    std::shared_ptr<ILogger> log;
    MatchingAlgorithm algo(bids, asks,log);

    EXPECT_TRUE(algo.validateOrdersToMatch());
}

TEST(MatchingAlgorithmTest, OrdersCannotBeFilled)
{
    std::deque<Order> bids = {Order(BUY, 1, 100.0)};
    std::deque<Order> asks = {Order(SELL, 1, 110.0)};
    std::shared_ptr<ILogger> log;
    MatchingAlgorithm algo(bids, asks,log);

    EXPECT_FALSE(algo.validateOrdersToMatch());
}

TEST(MatchingAlgorithmTest, FillOrder)
{
    std::deque<Order> bids = {Order(BUY, 1, 120.0)};
    std::deque<Order> asks = {Order(SELL, 3, 110.0)};
    std::shared_ptr<ILogger> log;
    MatchingAlgorithm algo(bids, asks,log);

    algo.FillOrder();
    ASSERT_EQ(asks.front().getQuantity(), 2);
    ASSERT_EQ(bids.front().getQuantity(), 0);
}


TEST(MatchingAlgorithmTest, CleanOrders)
{
    std::deque<Order> bids = {Order(BUY, 1, 120.0)};
    std::deque<Order> asks = {Order(SELL, 0, 110.0)};
    std::shared_ptr<ILogger> log;
    MatchingAlgorithm algo(bids, asks,log);

    algo.CleanOrders();
    ASSERT_TRUE(asks.empty());
}


TEST(MatchingAlgorithmTest, MatchExecutesTradeAndCleansOrders)
{
    std::deque<Order> bids = {Order(BUY, 1, 120.0)};
    std::deque<Order> asks = {Order(SELL, 1, 110.0)};
    std::shared_ptr<ILogger> log;
    MatchingAlgorithm algo(bids, asks,log);

    algo.matchTopOfBook(); 

    ASSERT_TRUE(bids.empty());
    ASSERT_TRUE(asks.empty());
}

TEST(MatchingAlgorithmTest, MatchExecutesTradeAndCleansOrdersLeaveRemaining)
{
    std::deque<Order> bids = {Order(BUY, 1, 120.0)};
    std::deque<Order> asks = {Order(SELL, 2, 110.0)};
    std::shared_ptr<ILogger> log;
    MatchingAlgorithm algo(bids, asks,log);

    algo.matchTopOfBook(); 

    ASSERT_TRUE(bids.empty());
    ASSERT_TRUE(!asks.empty());
}