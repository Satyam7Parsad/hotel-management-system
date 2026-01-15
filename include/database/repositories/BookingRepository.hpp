#pragma once

#include "database/DatabaseManager.hpp"
#include "database/models/Booking.hpp"
#include <vector>
#include <optional>

namespace HotelManagement {

class BookingRepository {
public:
    explicit BookingRepository(DatabaseManager& dbManager);
    ~BookingRepository() = default;

    std::optional<Booking> findById(int id);
    std::vector<Booking> findAll();
    std::vector<Booking> findByGuestId(int guestId);
    std::vector<Booking> findByRoomId(int roomId);
    std::vector<Booking> findByStatus(BookingStatus status);

    int create(const Booking& booking);
    bool update(const Booking& booking);
    bool deleteById(int id);

    bool checkIn(int bookingId);
    bool checkOut(int bookingId);

    int getActiveBookingsCount();
    int getTodayCheckIns();
    int getTodayCheckOuts();

private:
    DatabaseManager& dbManager;
    Booking rowToBooking(const pqxx::row& row);
};

} // namespace HotelManagement
