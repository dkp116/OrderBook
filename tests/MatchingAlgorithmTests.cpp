#include <gtest/gtest.h>

#include "Order.hpp"
#include "MatchingAlgorithm.hpp"

TEST(MatchingAlgorithmTest, ReturnsFalseWhenBothSidesEmpty)
{
    MatchingAlgorithm algo;

    std::deque<Order> bids;
    std::deque<Order> asks;

    EXPECT_FALSE(algo.anyOrdersToMatch(bids, asks));
}

TEST(MatchingAlgorithmTest, ReturnsFalseWhenAsksEmpty)
{
    MatchingAlgorithm algo;

    std::deque<Order> bids = { Order(BUY, 1, 100.0) };
    std::deque<Order> asks;

    EXPECT_FALSE(algo.anyOrdersToMatch(bids, asks));
}

TEST(MatchingAlgorithmTest, ReturnsFalseWhenBidsEmpty)
{
    MatchingAlgorithm algo;

    std::deque<Order> bids = { Order(BUY, 1, 100.0) };
        std::deque<Order> asks = { Order(SELL, 1, 100.0) };

    EXPECT_TRUE(algo.anyOrdersToMatch(bids, asks));
}