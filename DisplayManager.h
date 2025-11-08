#ifndef DISPLAYMANAGER_H
#define DISPLAYMANAGER_H

#include "Types.h"
#include "Instance.h"
#include <vector>
#include <mutex>
#include <memory>
#include <atomic>
#include <thread>

class DisplayManager {
private:
    std::vector<Instance*> instances;
    std::mutex display_mutex;
    std::atomic<bool> should_update;
    std::thread update_thread;
    
    void clearScreen();
    void updateDisplay();
    void displayLoop();
    
public:
    DisplayManager(std::vector<Instance*> inst);
    ~DisplayManager();
    
    void start();
    void stop();
    void logEvent(const std::string& message, const std::string& color = RESET);
};

#endif