#include "MatchingAlgorithm.hpp"

bool MatchingAlgorithm::anyOrdersToMatch(std::deque<Order>& Bids, std::deque<Order>& Asks){
    if(!Bids.empty() && !Asks.empty()){
        return true;
    }
    return false;
}

