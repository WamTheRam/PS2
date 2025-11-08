#include "Instance.h"

Instance::Instance(int instance_id) 
    : id(instance_id), is_active(false), parties_served(0), 
      total_time_served(0.0), current_run_number(0), current_run_time(0.0) {}

int Instance::getId() const { 
    return id; 
}

bool Instance::tryStart() {
    std::lock_guard<std::mutex> lock(instance_mutex);
    if (!is_active) {
        is_active = true;
        current_run_number++;
        run_start_time = std::chrono::steady_clock::now();
        return true;
    }
    return false;
}

void Instance::finish(double time_taken) {
    std::lock_guard<std::mutex> lock(instance_mutex);
    is_active = false;
    parties_served++;
    total_time_served += time_taken;
    current_run_time = time_taken;
}

bool Instance::isActive() {
    std::lock_guard<std::mutex> lock(instance_mutex);
    return is_active;
}

int Instance::getCurrentRun() {
    std::lock_guard<std::mutex> lock(instance_mutex);
    return current_run_number;
}

double Instance::getTotalTime() {
    std::lock_guard<std::mutex> lock(instance_mutex);
    return total_time_served;
}

double Instance::getCurrentElapsedTime() {
    std::lock_guard<std::mutex> lock(instance_mutex);
    if (is_active) {
        auto now = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = now - run_start_time;
        return elapsed.count();
    }
    return 0.0;
}

void Instance::getStats(int& served, double& time) {
    std::lock_guard<std::mutex> lock(instance_mutex);
    served = parties_served;
    time = total_time_served;
}