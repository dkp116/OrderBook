#include <gtest/gtest.h>

#include "Order.hpp"
#include "MatchingAlgorithm.hpp"
#include "ILogger.hpp"
#include <map>
#include <unordered_map>
#include <functional>
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
    std::multimap<double, Order, std::greater<double>> bids;
    std::multimap<double, Order, std::less<double>> asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator> bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator> askIterators;

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, bidIterators, askIterators, log);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, ReturnsFalseWhenAsksEmpty)
{
    std::multimap<double, Order, std::greater<double>> bids;
    std::multimap<double, Order, std::less<double>> asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator> bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator> askIterators;

    Order buy(BUY, 1, 100.0);
    auto it = bids.insert({buy.getPrice(), buy});
    bidIterators[buy.getID()] = it;

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, bidIterators, askIterators, log);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, ReturnsFalseWhenBidsEmpty)
{
    std::multimap<double, Order, std::greater<double>> bids;
    std::multimap<double, Order, std::less<double>> asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator> bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator> askIterators;

    Order sell(SELL, 1, 100.0);
    auto it = asks.insert({sell.getPrice(), sell});
    askIterators[sell.getID()] = it;

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, bidIterators, askIterators, log);

    EXPECT_FALSE(algo.anyOrdersToMatch());
}

TEST(MatchingAlgorithmTest, OrdersCanBeFilled)
{
    std::multimap<double, Order, std::greater<double>> bids;
    std::multimap<double, Order, std::less<double>> asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator> bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator> askIterators;

    Order buy(BUY, 1, 100.0);
    auto bit = bids.insert({buy.getPrice(), buy});
    bidIterators[buy.getID()] = bit;

    Order sell(SELL, 1, 100.0);
    auto ait = asks.insert({sell.getPrice(), sell});
    askIterators[sell.getID()] = ait;

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, bidIterators, askIterators, log);

    EXPECT_TRUE(algo.validateOrdersToMatch());
}

TEST(MatchingAlgorithmTest, OrdersCannotBeFilled)
{
    std::multimap<double, Order, std::greater<double>> bids;
    std::multimap<double, Order, std::less<double>> asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator> bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator> askIterators;

    Order buy(BUY, 1, 100.0);
    auto bit = bids.insert({buy.getPrice(), buy});
    bidIterators[buy.getID()] = bit;

    Order sell(SELL, 1, 110.0);
    auto ait = asks.insert({sell.getPrice(), sell});
    askIterators[sell.getID()] = ait;

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, bidIterators, askIterators, log);

    EXPECT_FALSE(algo.validateOrdersToMatch());
}

TEST(MatchingAlgorithmTest, FillOrder)
{
    std::multimap<double, Order, std::greater<double>> bids;
    std::multimap<double, Order, std::less<double>> asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator> bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator> askIterators;

    Order buy(BUY, 1, 120.0);
    auto bit = bids.insert({buy.getPrice(), buy});
    bidIterators[buy.getID()] = bit;

    Order sell(SELL, 3, 110.0);
    auto ait = asks.insert({sell.getPrice(), sell});
    askIterators[sell.getID()] = ait;

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, bidIterators, askIterators, log);

    algo.FillOrder();

    ASSERT_EQ(asks.begin()->second.getQuantity(), 2);
    ASSERT_EQ(bids.begin()->second.getQuantity(), 0);
}

TEST(MatchingAlgorithmTest, CleanOrders)
{
    std::multimap<double, Order, std::greater<double>> bids;
    std::multimap<double, Order, std::less<double>> asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator> bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator> askIterators;

    Order buy(BUY, 1, 120.0);
    auto bit = bids.insert({buy.getPrice(), buy});
    bidIterators[buy.getID()] = bit;

    Order sell(SELL, 0, 110.0);
    auto ait = asks.insert({sell.getPrice(), sell});
    askIterators[sell.getID()] = ait;

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, bidIterators, askIterators, log);

    algo.CleanOrders();

    ASSERT_TRUE(asks.empty());
}

TEST(MatchingAlgorithmTest, MatchExecutesTradeAndCleansOrders)
{
    std::multimap<double, Order, std::greater<double>> bids;
    std::multimap<double, Order, std::less<double>> asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator> bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator> askIterators;

    Order buy(BUY, 1, 120.0);
    auto bit = bids.insert({buy.getPrice(), buy});
    bidIterators[buy.getID()] = bit;

    Order sell(SELL, 1, 110.0);
    auto ait = asks.insert({sell.getPrice(), sell});
    askIterators[sell.getID()] = ait;

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, bidIterators, askIterators, log);

    algo.matchTopOfBook();

    ASSERT_TRUE(bids.empty());
    ASSERT_TRUE(asks.empty());
}

TEST(MatchingAlgorithmTest, MatchExecutesTradeAndCleansOrdersLeaveRemaining)
{
    std::multimap<double, Order, std::greater<double>> bids;
    std::multimap<double, Order, std::less<double>> asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator> bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator> askIterators;

    Order buy(BUY, 1, 120.0);
    auto bit = bids.insert({buy.getPrice(), buy});
    bidIterators[buy.getID()] = bit;

    Order sell(SELL, 2, 110.0);
    auto ait = asks.insert({sell.getPrice(), sell});
    askIterators[sell.getID()] = ait;

    auto log = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, bidIterators, askIterators, log);

    algo.matchTopOfBook();

    ASSERT_TRUE(bids.empty());
    ASSERT_FALSE(asks.empty());
}

TEST(MatchingAlgorithmTest, LogsTradeOnFillOrder)
{
    std::multimap<double, Order, std::greater<double>> bids;
    std::multimap<double, Order, std::less<double>> asks;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::greater<double>>::iterator> bidIterators;
    std::unordered_map<uint64_t, typename std::multimap<double, Order, std::less<double>>::iterator> askIterators;

    Order buy(BUY, 1, 120.0);
    auto bit = bids.insert({buy.getPrice(), buy});
    bidIterators[buy.getID()] = bit;

    Order sell(SELL, 1, 110.0);
    auto ait = asks.insert({sell.getPrice(), sell});
    askIterators[sell.getID()] = ait;

    auto mockLogger = std::make_shared<MockLogger>();
    MatchingAlgorithm algo(bids, asks, bidIterators, askIterators, mockLogger);

    algo.FillOrder();

    EXPECT_EQ(mockLogger->callCount, 1);

    EXPECT_EQ(mockLogger->lastBidID, buy.getID());
    EXPECT_EQ(mockLogger->lastAskID, sell.getID());

    EXPECT_EQ(mockLogger->lastQty, 1);
}