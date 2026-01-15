#pragma once

#include <string>

namespace HotelManagement {

struct Service {
    int id = 0;
    std::string serviceName;
    std::string description;
    double price = 0.0;
    std::string category;
    bool active = true;
    std::string createdAt;

    // Default constructor
    Service() = default;

    // Helper methods
    bool isValid() const {
        return !serviceName.empty() && price >= 0.0;
    }

    bool isActive() const {
        return active;
    }

    std::string toString() const {
        return serviceName + " ($" + std::to_string(price) + ")";
    }
};

} // namespace HotelManagement
