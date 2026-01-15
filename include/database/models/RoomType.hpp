#pragma once

#include <string>
#include <optional>

namespace HotelManagement {

struct RoomType {
    int id = 0;
    std::string typeName;
    double basePrice = 0.0;
    int maxOccupancy = 1;
    std::string description;
    std::string amenitiesJson; // JSON string from database
    std::string createdAt;
    std::string updatedAt;

    // Default constructor
    RoomType() = default;

    // Helper methods
    bool isValid() const {
        return !typeName.empty() && basePrice >= 0.0 && maxOccupancy > 0;
    }

    std::string toString() const {
        return typeName + " ($" + std::to_string(basePrice) + "/night, max " +
               std::to_string(maxOccupancy) + " guests)";
    }
};

} // namespace HotelManagement
