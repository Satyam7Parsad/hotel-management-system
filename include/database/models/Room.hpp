#pragma once

#include <string>

namespace HotelManagement {

enum class RoomStatus {
    Available,
    Occupied,
    Maintenance,
    Reserved
};

struct Room {
    int id = 0;
    std::string roomNumber;
    int roomTypeId = 0;
    int floorNumber = 1;
    RoomStatus status = RoomStatus::Available;
    std::string notes;
    std::string createdAt;
    std::string updatedAt;

    // Default constructor
    Room() = default;

    // Helper methods
    bool isValid() const {
        return !roomNumber.empty() && roomTypeId > 0 && floorNumber > 0;
    }

    bool isAvailable() const {
        return status == RoomStatus::Available;
    }

    bool isOccupied() const {
        return status == RoomStatus::Occupied;
    }

    // Convert status enum to string
    std::string statusToString() const {
        switch (status) {
            case RoomStatus::Available:   return "available";
            case RoomStatus::Occupied:    return "occupied";
            case RoomStatus::Maintenance: return "maintenance";
            case RoomStatus::Reserved:    return "reserved";
            default:                      return "unknown";
        }
    }

    // Convert string to status enum
    static RoomStatus stringToStatus(const std::string& statusStr) {
        if (statusStr == "available") return RoomStatus::Available;
        if (statusStr == "occupied") return RoomStatus::Occupied;
        if (statusStr == "maintenance") return RoomStatus::Maintenance;
        if (statusStr == "reserved") return RoomStatus::Reserved;
        return RoomStatus::Available; // default
    }

    std::string toString() const {
        return "Room " + roomNumber + " (Floor " + std::to_string(floorNumber) +
               ", " + statusToString() + ")";
    }
};

} // namespace HotelManagement
