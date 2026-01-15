#pragma once

#include "database/DatabaseManager.hpp"
#include "database/models/Room.hpp"
#include "database/models/RoomType.hpp"
#include <vector>
#include <optional>

namespace HotelManagement {

class RoomRepository {
public:
    explicit RoomRepository(DatabaseManager& dbManager);
    ~RoomRepository() = default;

    // CRUD operations
    std::optional<Room> findById(int id);
    std::vector<Room> findAll();
    std::vector<Room> findByFloor(int floorNumber);
    std::vector<Room> findByStatus(RoomStatus status);
    std::vector<Room> findByRoomType(int roomTypeId);

    int create(const Room& room);
    bool update(const Room& room);
    bool deleteById(int id);

    // Business logic
    bool isRoomAvailable(int roomId, const std::string& startDate, const std::string& endDate);
    std::vector<int> getAvailableRoomIds(const std::string& startDate, const std::string& endDate);
    bool updateRoomStatus(int roomId, RoomStatus newStatus);

    // Statistics
    int getTotalRooms();
    int getRoomsByStatus(RoomStatus status);
    std::map<RoomStatus, int> getRoomCountByStatus();

    // Room Type operations
    std::optional<RoomType> findRoomTypeById(int id);
    std::vector<RoomType> findAllRoomTypes();
    int createRoomType(const RoomType& roomType);
    bool updateRoomType(const RoomType& roomType);

private:
    DatabaseManager& dbManager;

    // Helper to convert database row to Room object
    Room rowToRoom(const pqxx::row& row);
    RoomType rowToRoomType(const pqxx::row& row);
};

} // namespace HotelManagement
