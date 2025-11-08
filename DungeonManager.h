#ifndef DUNGEONMANAGER_H
#define DUNGEONMANAGER_H

#include "Types.h"
#include "Instance.h"
#include "PlayerQueue.h"
#include "DisplayManager.h"
#include <vector>
#include <memory>
#include <atomic>
#include <random>

class DungeonManager {
private:
    std::vector<std::unique_ptr<Instance>> instances;
    PlayerQueue& player_queue;
    int min_time;
    int max_time;
    std::atomic<bool> simulation_running;
    std::unique_ptr<DisplayManager> display_manager;
    
    // Random number generation
    std::random_device rd;
    std::mt19937 gen;
    std::uniform_int_distribution<> time_dist;
    
    void worker();
    
public:
    DungeonManager(int num_instances, PlayerQueue& pq, int t1, int t2);
    void run(int num_worker_threads);
    void printSummary();
};

#endif