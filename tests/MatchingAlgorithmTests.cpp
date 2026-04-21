#include <gtest/gtest.h>

#include "Order.hpp"
#include "MatchingAlgorithm.hpp"
#include "ILogger.hpp"
#include <cstdint>


class MockLogger : public ILogger
{
public:
    int callCount = 0;

    uint64_t lastBidID = 0;
    uint64_t lastAskID = 0;
    double lastPrice = 0.0;
    int lastQty = 0;

    void logTrade(uint64_t bidID,
                  uint64_t askID,
                  double priceExecuted,
                  int quantity) override
    {
        callCount++;

        lastBidID = bidID;
        lastAskID = askID;
        lastPrice = priceExecuted;
        lastQty = quantity;
    }
};

TEST(MatchingAlgorithmTest, ReturnsFalseWhenBothSidesEmpty)
{
    std::deque<Order> bids;
    std::deque<Order> asks;

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, log);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, ReturnsFalseWhenAsksEmpty)
{
    std::deque<Order> bids = {Order(BUY, 1, 100.0)};
    std::deque<Order> asks;

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, log);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, ReturnsFalseWhenBidsEmpty)
{
    std::deque<Order> bids;
    std::deque<Order> asks = {Order(SELL, 1, 100.0)};

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, log);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, OrdersCanBeFilled)
{
    std::deque<Order> bids = {Order(BUY, 1, 100.0)};
    std::deque<Order> asks = {Order(SELL, 1, 100.0)};

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, log);

    EXPECT_TRUE(algo.validateOrdersToMatch());
}

TEST(MatchingAlgorithmTest, OrdersCannotBeFilled)
{
    std::deque<Order> bids = {Order(BUY, 1, 100.0)};
    std::deque<Order> asks = {Order(SELL, 1, 110.0)};

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, log);

    EXPECT_FALSE(algo.validateOrdersToMatch());
}

TEST(MatchingAlgorithmTest, FillOrder)
{
    std::deque<Order> bids = {Order(BUY, 1, 120.0)};
    std::deque<Order> asks = {Order(SELL, 3, 110.0)};

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, log);

    algo.FillOrder();

    ASSERT_EQ(asks.front().getQuantity(), 2);
    ASSERT_EQ(bids.front().getQuantity(), 0);
}

TEST(MatchingAlgorithmTest, CleanOrders)
{
    std::deque<Order> bids = {Order(BUY, 1, 120.0)};
    std::deque<Order> asks = {Order(SELL, 0, 110.0)};

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, log);

    algo.CleanOrders();

    ASSERT_TRUE(asks.empty());
}

TEST(MatchingAlgorithmTest, MatchExecutesTradeAndCleansOrders)
{
    std::deque<Order> bids = {Order(BUY, 1, 120.0)};
    std::deque<Order> asks = {Order(SELL, 1, 110.0)};

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, log);

    algo.matchTopOfBook();

    ASSERT_TRUE(bids.empty());
    ASSERT_TRUE(asks.empty());
}

TEST(MatchingAlgorithmTest, MatchExecutesTradeAndCleansOrdersLeaveRemaining)
{
    std::deque<Order> bids = {Order(BUY, 1, 120.0)};
    std::deque<Order> asks = {Order(SELL, 2, 110.0)};

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, log);

    algo.matchTopOfBook();

    ASSERT_TRUE(bids.empty());
    ASSERT_FALSE(asks.empty());
}

TEST(MatchingAlgorithmTest, LogsTradeOnFillOrder)
{
    Order buy(BUY, 1, 120.0);
    Order sell(SELL, 1, 110.0);

    std::deque<Order> bids = { buy };
    std::deque<Order> asks = { sell };

    auto mockLogger = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, mockLogger);

    algo.FillOrder();

    EXPECT_EQ(mockLogger->callCount, 1);

    EXPECT_EQ(mockLogger->lastBidID, buy.getID());
    EXPECT_EQ(mockLogger->lastAskID, sell.getID());

    EXPECT_EQ(mockLogger->lastQty, 1);
}