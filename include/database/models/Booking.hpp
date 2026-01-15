#pragma once

#include <string>
#include <optional>

namespace HotelManagement {

enum class BookingStatus {
    Pending,
    Confirmed,
    CheckedIn,
    CheckedOut,
    Cancelled
};

struct Booking {
    int id = 0;
    int guestId = 0;
    int roomId = 0;
    std::string checkInDate;  // YYYY-MM-DD
    std::string checkOutDate; // YYYY-MM-DD
    std::string actualCheckIn;  // YYYY-MM-DD HH:MM:SS (optional)
    std::string actualCheckOut; // YYYY-MM-DD HH:MM:SS (optional)
    int numAdults = 1;
    int numChildren = 0;
    BookingStatus status = BookingStatus::Pending;
    std::string specialRequests;
    double totalAmount = 0.0;
    std::string createdAt;
    std::string updatedAt;

    // Default constructor
    Booking() = default;

    // Helper methods
    bool isValid() const {
        return guestId > 0 && roomId > 0 &&
               !checkInDate.empty() && !checkOutDate.empty() &&
               numAdults > 0;
    }

    bool isActive() const {
        return status == BookingStatus::Confirmed ||
               status == BookingStatus::CheckedIn;
    }

    bool canCheckIn() const {
        return status == BookingStatus::Confirmed;
    }

    bool canCheckOut() const {
        return status == BookingStatus::CheckedIn;
    }

    int getDurationDays() const;

    bool overlaps(const std::string& start, const std::string& end) const;

    // Convert status enum to string
    std::string statusToString() const {
        switch (status) {
            case BookingStatus::Pending:    return "pending";
            case BookingStatus::Confirmed:  return "confirmed";
            case BookingStatus::CheckedIn:  return "checked_in";
            case BookingStatus::CheckedOut: return "checked_out";
            case BookingStatus::Cancelled:  return "cancelled";
            default:                        return "unknown";
        }
    }

    // Convert string to status enum
    static BookingStatus stringToStatus(const std::string& statusStr) {
        if (statusStr == "pending") return BookingStatus::Pending;
        if (statusStr == "confirmed") return BookingStatus::Confirmed;
        if (statusStr == "checked_in") return BookingStatus::CheckedIn;
        if (statusStr == "checked_out") return BookingStatus::CheckedOut;
        if (statusStr == "cancelled") return BookingStatus::Cancelled;
        return BookingStatus::Pending; // default
    }

    std::string toString() const {
        return "Booking #" + std::to_string(id) + " (" + statusToString() + ")";
    }
};

} // namespace HotelManagement
