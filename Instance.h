#ifndef INSTANCE_H
#define INSTANCE_H

#include "Types.h"
#include <mutex>
#include <string>
#include <chrono>

class Instance {
private:
    int id;
    bool is_active;
    int parties_served;
    double total_time_served;
    int current_run_number;
    double current_run_time;
    std::chrono::steady_clock::time_point run_start_time;
    std::mutex instance_mutex;
    
public:
    Instance(int instance_id);
    
    int getId() const;
    bool tryStart();
    void finish(double time_taken);
    
    // Get display information
    bool isActive();
    int getCurrentRun();
    double getTotalTime();
    double getCurrentElapsedTime();
    
    // Get statistics
    void getStats(int& served, double& time);
};

#endif // INSTANCE_H