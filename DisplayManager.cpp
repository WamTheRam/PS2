#include "DisplayManager.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cstdlib>

DisplayManager::DisplayManager(std::vector<Instance*> inst) 
    : instances(inst), should_update(false) {}

DisplayManager::~DisplayManager() {
    stop();
}

void DisplayManager::clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void DisplayManager::updateDisplay() {
    std::lock_guard<std::mutex> lock(display_mutex);
    
    // Move cursor to top
    std::cout << "\033[H";
    
    std::cout << MAGENTA << "=== DUNGEON QUEUE SIMULATOR ===" << RESET << "\n\n";
    
    std::cout << CYAN << "Instance Status:" << RESET << "\n";
    std::cout << "------------------------------------------------------------\n";
    
    for (auto* instance : instances) {
        std::cout << "Instance " << std::setw(2) << instance->getId() << " | ";

        std::ostringstream middle;
        if (instance->isActive()) {
            double elapsed = instance->getCurrentElapsedTime();
            middle << GREEN << "RUNNING" << RESET
                   << " | Run #" << std::setw(2) << instance->getCurrentRun()
                   << " | Elapsed: " << std::fixed << std::setprecision(1)
                   << elapsed << "s";
        } else {
            middle << YELLOW << "EMPTY " << RESET
                   << "  | Run #" << std::setw(2) << instance->getCurrentRun()
                   << " | Idle";
        }

    std::string mid_str = middle.str();
    const size_t MID_WIDTH = 36;
        // ensure consistent width by padding or truncating
        if (mid_str.size() < MID_WIDTH) {
            mid_str += std::string(MID_WIDTH - mid_str.size(), ' ');
        } else if (mid_str.size() > MID_WIDTH) {
            mid_str = mid_str.substr(0, MID_WIDTH);
        }

        std::cout << mid_str;

        std::cout << " | Total Time: " << std::fixed << std::setprecision(1)
                  << instance->getTotalTime() << "s\n";
    }
    
    std::cout << "------------------------------------------------------------\n";
    std::cout << std::flush;
}

void DisplayManager::displayLoop() {
    while (should_update) {
        updateDisplay();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void DisplayManager::start() {
    clearScreen();
    should_update = true;
    update_thread = std::thread(&DisplayManager::displayLoop, this);
}

void DisplayManager::stop() {
    if (should_update) {
        should_update = false;
        if (update_thread.joinable()) {
            update_thread.join();
        }
    }
}

void DisplayManager::logEvent(const std::string& message, const std::string& color) {
    std::lock_guard<std::mutex> lock(display_mutex);
    std::cout << "\n" << color << message << RESET << "\n" << std::flush;
}