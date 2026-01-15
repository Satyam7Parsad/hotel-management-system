#include "database/models/Booking.hpp"
#include "utils/DateUtils.hpp"

namespace HotelManagement {

int Booking::getDurationDays() const {
    return DateUtils::daysBetween(checkInDate, checkOutDate);
}

bool Booking::overlaps(const std::string& start, const std::string& end) const {
    // Check if this booking overlaps with the given date range
    // Overlap exists if: (start1 < end2) AND (start2 < end1)
    int compareStartEnd = DateUtils::compareDates(checkInDate, end);
    int compareEndStart = DateUtils::compareDates(start, checkOutDate);

    return (compareStartEnd < 0) && (compareEndStart < 0);
}

} // namespace HotelManagement
