#include "database/repositories/RoomRepository.hpp"
#include "utils/Logger.hpp"
#include <map>

namespace HotelManagement {

RoomRepository::RoomRepository(DatabaseManager& db) : dbManager(db) {}

std::optional<Room> RoomRepository::findById(int id) {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) -> std::optional<Room> {
            auto result = txn.exec_params(
                "SELECT id, room_number, room_type_id, floor_number, status, notes, "
                "created_at, updated_at FROM rooms WHERE id = $1",
                id
            );

            if (result.empty()) {
                return std::nullopt;
            }

            return rowToRoom(result[0]);
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::findById failed: ", e.what());
        return std::nullopt;
    }
}

std::vector<Room> RoomRepository::findAll() {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec(
                "SELECT id, room_number, room_type_id, floor_number, status, notes, "
                "created_at, updated_at FROM rooms ORDER BY floor_number, room_number"
            );

            std::vector<Room> rooms;
            for (const auto& row : result) {
                rooms.push_back(rowToRoom(row));
            }
            return rooms;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::findAll failed: ", e.what());
        return {};
    }
}

std::vector<Room> RoomRepository::findByFloor(int floorNumber) {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec_params(
                "SELECT id, room_number, room_type_id, floor_number, status, notes, "
                "created_at, updated_at FROM rooms WHERE floor_number = $1 "
                "ORDER BY room_number",
                floorNumber
            );

            std::vector<Room> rooms;
            for (const auto& row : result) {
                rooms.push_back(rowToRoom(row));
            }
            return rooms;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::findByFloor failed: ", e.what());
        return {};
    }
}

std::vector<Room> RoomRepository::findByStatus(RoomStatus status) {
    try {
        std::string statusStr = Room().statusToString(); // Temporary room for conversion
        Room temp;
        temp.status = status;
        statusStr = temp.statusToString();

        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec_params(
                "SELECT id, room_number, room_type_id, floor_number, status, notes, "
                "created_at, updated_at FROM rooms WHERE status = $1 "
                "ORDER BY floor_number, room_number",
                statusStr
            );

            std::vector<Room> rooms;
            for (const auto& row : result) {
                rooms.push_back(rowToRoom(row));
            }
            return rooms;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::findByStatus failed: ", e.what());
        return {};
    }
}

std::vector<Room> RoomRepository::findByRoomType(int roomTypeId) {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec_params(
                "SELECT id, room_number, room_type_id, floor_number, status, notes, "
                "created_at, updated_at FROM rooms WHERE room_type_id = $1 "
                "ORDER BY floor_number, room_number",
                roomTypeId
            );

            std::vector<Room> rooms;
            for (const auto& row : result) {
                rooms.push_back(rowToRoom(row));
            }
            return rooms;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::findByRoomType failed: ", e.what());
        return {};
    }
}

int RoomRepository::create(const Room& room) {
    try {
        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params(
                "INSERT INTO rooms (room_number, room_type_id, floor_number, status, notes) "
                "VALUES ($1, $2, $3, $4, $5) RETURNING id",
                room.roomNumber,
                room.roomTypeId,
                room.floorNumber,
                room.statusToString(),
                room.notes
            );

            int newId = result[0][0].as<int>();
            Logger::info("Room created: ", room.roomNumber, " (ID: ", newId, ")");
            return newId;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::create failed: ", e.what());
        return -1;
    }
}

bool RoomRepository::update(const Room& room) {
    try {
        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params(
                "UPDATE rooms SET room_number = $1, room_type_id = $2, floor_number = $3, "
                "status = $4, notes = $5 WHERE id = $6",
                room.roomNumber,
                room.roomTypeId,
                room.floorNumber,
                room.statusToString(),
                room.notes,
                room.id
            );

            bool success = result.affected_rows() > 0;
            if (success) {
                Logger::info("Room updated: ", room.roomNumber);
            }
            return success;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::update failed: ", e.what());
        return false;
    }
}

bool RoomRepository::deleteById(int id) {
    try {
        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params("DELETE FROM rooms WHERE id = $1", id);
            bool success = result.affected_rows() > 0;
            if (success) {
                Logger::info("Room deleted: ID ", id);
            }
            return success;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::deleteById failed: ", e.what());
        return false;
    }
}

bool RoomRepository::isRoomAvailable(int roomId, const std::string& startDate, const std::string& endDate) {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec_params(
                "SELECT COUNT(*) FROM bookings "
                "WHERE room_id = $1 "
                "AND status NOT IN ('cancelled', 'checked_out') "
                "AND (check_in_date, check_out_date) OVERLAPS ($2::date, $3::date)",
                roomId, startDate, endDate
            );

            int count = result[0][0].as<int>();
            return count == 0;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::isRoomAvailable failed: ", e.what());
        return false;
    }
}

std::vector<int> RoomRepository::getAvailableRoomIds(const std::string& startDate, const std::string& endDate) {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec_params(
                "SELECT r.id FROM rooms r "
                "WHERE r.status = 'available' "
                "AND NOT EXISTS ("
                "  SELECT 1 FROM bookings b "
                "  WHERE b.room_id = r.id "
                "  AND b.status NOT IN ('cancelled', 'checked_out') "
                "  AND (b.check_in_date, b.check_out_date) OVERLAPS ($1::date, $2::date)"
                ")",
                startDate, endDate
            );

            std::vector<int> availableRooms;
            for (const auto& row : result) {
                availableRooms.push_back(row[0].as<int>());
            }
            return availableRooms;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::getAvailableRoomIds failed: ", e.what());
        return {};
    }
}

bool RoomRepository::updateRoomStatus(int roomId, RoomStatus newStatus) {
    try {
        Room temp;
        temp.status = newStatus;
        std::string statusStr = temp.statusToString();

        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params(
                "UPDATE rooms SET status = $1 WHERE id = $2",
                statusStr, roomId
            );
            return result.affected_rows() > 0;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::updateRoomStatus failed: ", e.what());
        return false;
    }
}

int RoomRepository::getTotalRooms() {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec("SELECT COUNT(*) FROM rooms");
            return result[0][0].as<int>();
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::getTotalRooms failed: ", e.what());
        return 0;
    }
}

int RoomRepository::getRoomsByStatus(RoomStatus status) {
    try {
        Room temp;
        temp.status = status;
        std::string statusStr = temp.statusToString();

        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec_params(
                "SELECT COUNT(*) FROM rooms WHERE status = $1",
                statusStr
            );
            return result[0][0].as<int>();
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::getRoomsByStatus failed: ", e.what());
        return 0;
    }
}

std::map<RoomStatus, int> RoomRepository::getRoomCountByStatus() {
    std::map<RoomStatus, int> counts;
    counts[RoomStatus::Available] = getRoomsByStatus(RoomStatus::Available);
    counts[RoomStatus::Occupied] = getRoomsByStatus(RoomStatus::Occupied);
    counts[RoomStatus::Maintenance] = getRoomsByStatus(RoomStatus::Maintenance);
    counts[RoomStatus::Reserved] = getRoomsByStatus(RoomStatus::Reserved);
    return counts;
}

// Room Type operations
std::optional<RoomType> RoomRepository::findRoomTypeById(int id) {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) -> std::optional<RoomType> {
            auto result = txn.exec_params(
                "SELECT id, type_name, base_price, max_occupancy, description, "
                "amenities::text, created_at, updated_at FROM room_types WHERE id = $1",
                id
            );

            if (result.empty()) {
                return std::nullopt;
            }

            return rowToRoomType(result[0]);
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::findRoomTypeById failed: ", e.what());
        return std::nullopt;
    }
}

std::vector<RoomType> RoomRepository::findAllRoomTypes() {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec(
                "SELECT id, type_name, base_price, max_occupancy, description, "
                "amenities::text, created_at, updated_at FROM room_types ORDER BY base_price"
            );

            std::vector<RoomType> roomTypes;
            for (const auto& row : result) {
                roomTypes.push_back(rowToRoomType(row));
            }
            return roomTypes;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::findAllRoomTypes failed: ", e.what());
        return {};
    }
}

int RoomRepository::createRoomType(const RoomType& roomType) {
    try {
        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params(
                "INSERT INTO room_types (type_name, base_price, max_occupancy, description) "
                "VALUES ($1, $2, $3, $4) RETURNING id",
                roomType.typeName,
                roomType.basePrice,
                roomType.maxOccupancy,
                roomType.description
            );

            return result[0][0].as<int>();
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::createRoomType failed: ", e.what());
        return -1;
    }
}

bool RoomRepository::updateRoomType(const RoomType& roomType) {
    try {
        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params(
                "UPDATE room_types SET type_name = $1, base_price = $2, max_occupancy = $3, "
                "description = $4 WHERE id = $5",
                roomType.typeName,
                roomType.basePrice,
                roomType.maxOccupancy,
                roomType.description,
                roomType.id
            );

            return result.affected_rows() > 0;
        });
    } catch (const std::exception& e) {
        Logger::error("RoomRepository::updateRoomType failed: ", e.what());
        return false;
    }
}

// Helper methods
Room RoomRepository::rowToRoom(const pqxx::row& row) {
    Room room;
    room.id = row["id"].as<int>();
    room.roomNumber = row["room_number"].as<std::string>();
    room.roomTypeId = row["room_type_id"].as<int>();
    room.floorNumber = row["floor_number"].as<int>();
    room.status = Room::stringToStatus(row["status"].as<std::string>());
    room.notes = row["notes"].is_null() ? "" : row["notes"].as<std::string>();
    room.createdAt = row["created_at"].as<std::string>();
    room.updatedAt = row["updated_at"].as<std::string>();
    return room;
}

RoomType RoomRepository::rowToRoomType(const pqxx::row& row) {
    RoomType roomType;
    roomType.id = row["id"].as<int>();
    roomType.typeName = row["type_name"].as<std::string>();
    roomType.basePrice = row["base_price"].as<double>();
    roomType.maxOccupancy = row["max_occupancy"].as<int>();
    roomType.description = row["description"].is_null() ? "" : row["description"].as<std::string>();
    roomType.amenitiesJson = row["amenities"].is_null() ? "{}" : row["amenities"].as<std::string>();
    roomType.createdAt = row["created_at"].as<std::string>();
    roomType.updatedAt = row["updated_at"].as<std::string>();
    return roomType;
}

} // namespace HotelManagement
