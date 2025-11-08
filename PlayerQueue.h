#ifndef PLAYERQUEUE_H
#define PLAYERQUEUE_H

#include "Types.h"
#include <queue>
#include <mutex>
#include <memory>

class PlayerQueue {
private:
    std::queue<int> tanks;
    std::queue<int> healers;
    std::queue<int> dps;
    std::mutex queue_mutex;
    
public:
    // Add players to their respective queues
    void addPlayers(int num_tanks, int num_healers, int num_dps);
    
    // Try to form a party (1 tank, 1 healer, 3 DPS)
    std::unique_ptr<Party> tryFormParty();
    
    // Check if more parties can be formed
    bool canFormMoreParties();
    
    // Get current queue sizes
    void getQueueSizes(int& t, int& h, int& d);
};

#endif