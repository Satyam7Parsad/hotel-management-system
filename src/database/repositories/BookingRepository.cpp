#include "database/repositories/BookingRepository.hpp"
#include "utils/Logger.hpp"
#include "utils/DateUtils.hpp"

namespace HotelManagement {

BookingRepository::BookingRepository(DatabaseManager& db) : dbManager(db) {}

std::optional<Booking> BookingRepository::findById(int id) {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) -> std::optional<Booking> {
            auto result = txn.exec_params(
                "SELECT id, guest_id, room_id, check_in_date, check_out_date, actual_check_in, "
                "actual_check_out, num_adults, num_children, status, special_requests, total_amount, "
                "created_at, updated_at FROM bookings WHERE id = $1", id
            );
            if (result.empty()) return std::nullopt;
            return rowToBooking(result[0]);
        });
    } catch (const std::exception& e) {
        Logger::error("BookingRepository::findById failed: ", e.what());
        return std::nullopt;
    }
}

std::vector<Booking> BookingRepository::findAll() {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec(
                "SELECT id, guest_id, room_id, check_in_date, check_out_date, actual_check_in, "
                "actual_check_out, num_adults, num_children, status, special_requests, total_amount, "
                "created_at, updated_at FROM bookings ORDER BY check_in_date DESC"
            );
            std::vector<Booking> bookings;
            for (const auto& row : result) {
                bookings.push_back(rowToBooking(row));
            }
            return bookings;
        });
    } catch (const std::exception& e) {
        Logger::error("BookingRepository::findAll failed: ", e.what());
        return {};
    }
}

std::vector<Booking> BookingRepository::findByStatus(BookingStatus status) {
    try {
        Booking temp;
        temp.status = status;
        std::string statusStr = temp.statusToString();

        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec_params(
                "SELECT id, guest_id, room_id, check_in_date, check_out_date, actual_check_in, "
                "actual_check_out, num_adults, num_children, status, special_requests, total_amount, "
                "created_at, updated_at FROM bookings WHERE status = $1", statusStr
            );
            std::vector<Booking> bookings;
            for (const auto& row : result) {
                bookings.push_back(rowToBooking(row));
            }
            return bookings;
        });
    } catch (const std::exception& e) {
        return {};
    }
}

int BookingRepository::create(const Booking& booking) {
    try {
        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params(
                "INSERT INTO bookings (guest_id, room_id, check_in_date, check_out_date, "
                "num_adults, num_children, status, special_requests, total_amount) "
                "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9) RETURNING id",
                booking.guestId, booking.roomId, booking.checkInDate, booking.checkOutDate,
                booking.numAdults, booking.numChildren, booking.statusToString(),
                booking.specialRequests, booking.totalAmount
            );
            return result[0][0].as<int>();
        });
    } catch (const std::exception& e) {
        Logger::error("BookingRepository::create failed: ", e.what());
        return -1;
    }
}

bool BookingRepository::checkIn(int bookingId) {
    try {
        std::string now = DateUtils::getCurrentDateTime();
        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params(
                "UPDATE bookings SET status = 'checked_in', actual_check_in = $1 WHERE id = $2",
                now, bookingId
            );
            return result.affected_rows() > 0;
        });
    } catch (const std::exception& e) {
        return false;
    }
}

bool BookingRepository::checkOut(int bookingId) {
    try {
        std::string now = DateUtils::getCurrentDateTime();
        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params(
                "UPDATE bookings SET status = 'checked_out', actual_check_out = $1 WHERE id = $2",
                now, bookingId
            );
            return result.affected_rows() > 0;
        });
    } catch (const std::exception& e) {
        return false;
    }
}

int BookingRepository::getActiveBookingsCount() {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec(
                "SELECT COUNT(*) FROM bookings WHERE status IN ('confirmed', 'checked_in')"
            );
            return result[0][0].as<int>();
        });
    } catch (const std::exception& e) {
        return 0;
    }
}

Booking BookingRepository::rowToBooking(const pqxx::row& row) {
    Booking booking;
    booking.id = row["id"].as<int>();
    booking.guestId = row["guest_id"].as<int>();
    booking.roomId = row["room_id"].as<int>();
    booking.checkInDate = row["check_in_date"].as<std::string>();
    booking.checkOutDate = row["check_out_date"].as<std::string>();
    booking.actualCheckIn = row["actual_check_in"].is_null() ? "" : row["actual_check_in"].as<std::string>();
    booking.actualCheckOut = row["actual_check_out"].is_null() ? "" : row["actual_check_out"].as<std::string>();
    booking.numAdults = row["num_adults"].as<int>();
    booking.numChildren = row["num_children"].as<int>();
    booking.status = Booking::stringToStatus(row["status"].as<std::string());
    booking.specialRequests = row["special_requests"].is_null() ? "" : row["special_requests"].as<std::string>();
    booking.totalAmount = row["total_amount"].is_null() ? 0.0 : row["total_amount"].as<double>();
    booking.createdAt = row["created_at"].as<std::string>();
    booking.updatedAt = row["updated_at"].as<std::string>();
    return booking;
}

std::vector<Booking> BookingRepository::findByGuestId(int guestId) { return {}; }
std::vector<Booking> BookingRepository::findByRoomId(int roomId) { return {}; }
bool BookingRepository::update(const Booking& booking) { return false; }
bool BookingRepository::deleteById(int id) { return false; }
int BookingRepository::getTodayCheckIns() { return 0; }
int BookingRepository::getTodayCheckOuts() { return 0; }

} // namespace HotelManagement
