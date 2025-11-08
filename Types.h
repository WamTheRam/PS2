#ifndef TYPES_H
#define TYPES_H

// ANSI color codes for better output readability
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

// Enum to represent player roles
enum class Role {
    TANK,
    HEALER,
    DPS
};

// Structure to represent a party of 5 players
struct Party {
    int tank_id;
    int healer_id;
    int dps_ids[3];
    
    Party(int t, int h, int d1, int d2, int d3) 
        : tank_id(t), healer_id(h), dps_ids{d1, d2, d3} {}
};

#endif