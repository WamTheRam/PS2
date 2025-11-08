#include "PlayerQueue.h"

void PlayerQueue::addPlayers(int num_tanks, int num_healers, int num_dps) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    
    for (int i = 1; i <= num_tanks; ++i) {
        tanks.push(i);
    }
    for (int i = 1; i <= num_healers; ++i) {
        healers.push(i);
    }
    for (int i = 1; i <= num_dps; ++i) {
        dps.push(i);
    }
}

std::unique_ptr<Party> PlayerQueue::tryFormParty() {
    std::lock_guard<std::mutex> lock(queue_mutex);
    
    // Check if we have enough players for a full party
    if (tanks.size() >= 1 && healers.size() >= 1 && dps.size() >= 3) {
        int tank = tanks.front(); tanks.pop();
        int healer = healers.front(); healers.pop();
        int dps1 = dps.front(); dps.pop();
        int dps2 = dps.front(); dps.pop();
        int dps3 = dps.front(); dps.pop();
        
        return std::unique_ptr<Party>(new Party(tank, healer, dps1, dps2, dps3));
    }
    
    return nullptr;
}

bool PlayerQueue::canFormMoreParties() {
    std::lock_guard<std::mutex> lock(queue_mutex);
    return (tanks.size() >= 1 && healers.size() >= 1 && dps.size() >= 3);
}

void PlayerQueue::getQueueSizes(int& t, int& h, int& d) {
    std::lock_guard<std::mutex> lock(queue_mutex);
    t = tanks.size();
    h = healers.size();
    d = dps.size();
}