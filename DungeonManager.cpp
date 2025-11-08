#include "DungeonManager.h"
#include <thread>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <algorithm>

DungeonManager::DungeonManager(int num_instances, PlayerQueue& pq, int t1, int t2)
    : player_queue(pq), min_time(t1), max_time(t2), 
      simulation_running(true), gen(rd()), time_dist(t1, t2) {
    
    // Create all instances
    for (int i = 0; i < num_instances; ++i) {
        // C++11: std::make_unique is not available, construct unique_ptr explicitly
        instances.push_back(std::unique_ptr<Instance>(new Instance(i + 1)));
    }
    
    // Create display manager
    std::vector<Instance*> inst_ptrs;
    for (auto& inst : instances) {
        inst_ptrs.push_back(inst.get());
    }
    // C++11: use explicit unique_ptr construction instead of make_unique
    display_manager = std::unique_ptr<DisplayManager>(new DisplayManager(inst_ptrs));
}

void DungeonManager::worker() {
    while (simulation_running) {
        // Try to form a party
        auto party = player_queue.tryFormParty();
        
        if (!party) {
            if (!player_queue.canFormMoreParties()) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            continue;
        }
        
        // Find an available instance
        Instance* available_instance = nullptr;
        while (simulation_running && !available_instance) {
            for (auto& instance : instances) {
                if (instance->tryStart()) {
                    available_instance = instance.get();
                    break;
                }
            }
            
            if (!available_instance) {
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }
        }
        
        if (!available_instance) break;
        
        // Generate random clear time
        int clear_time = time_dist(gen);
        
        // Simulate dungeon run
        std::this_thread::sleep_for(std::chrono::seconds(clear_time));
        
        // Mark as finished
        available_instance->finish(static_cast<double>(clear_time));
    }
}

void DungeonManager::run(int num_worker_threads) {
    // Start display updates
    display_manager->start();
    
    display_manager->logEvent("=== Simulation Started ===", MAGENTA);
    
    // Create worker threads
    std::vector<std::thread> workers;
    for (int i = 0; i < num_worker_threads; ++i) {
        workers.emplace_back(&DungeonManager::worker, this);
    }
    
    // Wait for all workers
    for (auto& worker : workers) {
        worker.join();
    }
    
    simulation_running = false;
    
    // Stop display and show final state
    display_manager->stop();
    display_manager->logEvent("=== Simulation Ended ===", MAGENTA);
}

void DungeonManager::printSummary() {
    std::cout << "\n" << MAGENTA << "================================\n";
    std::cout << "       SIMULATION SUMMARY        \n";
    std::cout << "================================" << RESET << "\n\n";
    
    int total_parties = 0;
    double total_time = 0.0;
    
    for (auto& instance : instances) {
        int served;
        double time;
        instance->getStats(served, time);
        
        total_parties += served;
        total_time += time;
        
        std::cout << CYAN << "Instance " << instance->getId() << ":" << RESET << "\n";
        std::cout << "  Parties Served: " << GREEN << served << RESET << "\n";
        std::cout << "  Total Time:     " << YELLOW << std::fixed 
                  << std::setprecision(1) << time << "s" << RESET << "\n\n";
    }
    
    std::cout << MAGENTA << "Overall Statistics:" << RESET << "\n";
    std::cout << "  Total Parties:  " << GREEN << total_parties << RESET << "\n";
    std::cout << "  Total Time:     " << YELLOW << std::fixed 
              << std::setprecision(1) << total_time << "s" << RESET << "\n";
    
    if (total_parties > 0) {
        std::cout << "  Avg Time/Party: " << YELLOW << std::fixed 
                  << std::setprecision(1) << (total_time / total_parties) 
                  << "s" << RESET << "\n";
    }
    
    // Show remaining players
    int t, h, d;
    player_queue.getQueueSizes(t, h, d);
    std::cout << "\n" << RED << "Remaining Players:" << RESET << "\n";
    std::cout << "  Tanks:   " << t << "\n";
    std::cout << "  Healers: " << h << "\n";
    std::cout << "  DPS:     " << d << "\n";
}