#include "Types.h"
#include "PlayerQueue.h"
#include "DungeonManager.h"
#include <iostream>
#include <limits>
#include <algorithm>
#include <cstdlib>

// Input validation helper
int getValidatedInt(const std::string& prompt, int min_val = 0) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min_val) {
            return value;
        }
        std::cout << RED << "Invalid input! Please enter >= " 
                  << min_val << RESET << "\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int getValidatedIntRange(const std::string& prompt, int min_val, int max_val) {
    int value;
    while (true) {
        std::cout << prompt;
        if (std::cin >> value && value >= min_val && value <= max_val) {
            return value;
        }
        std::cout << RED << "Invalid input! Enter " << min_val 
                  << "-" << max_val << RESET << "\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

int main() {
    // Clear screen
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
    
    std::cout << MAGENTA << "================================\n";
    std::cout << "  DUNGEON QUEUE SIMULATOR\n";
    std::cout << "================================" << RESET << "\n\n";
    
    // Get user input
    int n = getValidatedInt("Number of instances: ", 1);
    int t = getValidatedInt("Number of tanks: ", 0);
    int h = getValidatedInt("Number of healers: ", 0);
    int d = getValidatedInt("Number of DPS: ", 0);
    
    int t1 = getValidatedInt("Min clear time (seconds): ", 1);
    int t2 = getValidatedIntRange("Max clear time (seconds, <=15): ", t1, 15);
    
    // Calculate max parties
    int max_parties = std::min({t, h, d / 3});
    
    std::cout << "\n" << CYAN << "Configuration:" << RESET << "\n";
    std::cout << "  Instances:   " << n << "\n";
    std::cout << "  Tanks:       " << t << "\n";
    std::cout << "  Healers:     " << h << "\n";
    std::cout << "  DPS:         " << d << "\n";
    std::cout << "  Clear Time:  " << t1 << "-" << t2 << "s\n";
    std::cout << "  Max Parties: " << max_parties << "\n\n";
    
    if (max_parties == 0) {
        std::cout << RED << "Not enough players!" << RESET << "\n";
        return 0;
    }
    
    std::cout << "Press Enter to start...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
    
    // Initialize and run
    PlayerQueue player_queue;
    player_queue.addPlayers(t, h, d);
    
    DungeonManager manager(n, player_queue, t1, t2);
    int num_workers = std::min(n, max_parties);
    manager.run(num_workers);
    
    // Show summary
    manager.printSummary();
    
    return 0;
}