#include <gtest/gtest.h>

#include "Order.hpp"
#include "MatchingAlgorithm.hpp"

TEST(MatchingAlgorithmTest, ReturnsFalseWhenBothSidesEmpty)
{
    std::deque<Order> bids;
    std::deque<Order> asks;

    MatchingAlgorithm algo(bids, asks);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, ReturnsFalseWhenAsksEmpty)
{
    std::deque<Order> bids = {Order(BUY, 1, 100.0)};
    std::deque<Order> asks;

    MatchingAlgorithm algo(bids, asks);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, ReturnsFalseWhenBidsEmpty)
{
    std::deque<Order> bids;
    std::deque<Order> asks = {Order(SELL, 1, 100.0)};

    MatchingAlgorithm algo(bids, asks);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, OrdersCanBeFilled)
{
    std::deque<Order> bids = {Order(BUY, 1, 100.0)};
    std::deque<Order> asks = {Order(SELL, 1, 100.0)};

    MatchingAlgorithm algo(bids, asks);

    EXPECT_TRUE(algo.validateOrdersToMatch());
}

TEST(MatchingAlgorithmTest, OrdersCannotBeFilled)
{
    std::deque<Order> bids = {Order(BUY, 1, 100.0)};
    std::deque<Order> asks = {Order(SELL, 1, 110.0)};

    MatchingAlgorithm algo(bids, asks);

    EXPECT_FALSE(algo.validateOrdersToMatch());
}