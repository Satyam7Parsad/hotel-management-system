#pragma once

#include <string>
#include <chrono>
#include <optional>

namespace HotelManagement {

class DateUtils {
public:
    // Get current date as string (YYYY-MM-DD)
    static std::string getCurrentDate();

    // Get current date and time as string (YYYY-MM-DD HH:MM:SS)
    static std::string getCurrentDateTime();

    // Format date from time_point to string (YYYY-MM-DD)
    static std::string formatDate(const std::chrono::system_clock::time_point& timePoint);

    // Format date and time from time_point to string (YYYY-MM-DD HH:MM:SS)
    static std::string formatDateTime(const std::chrono::system_clock::time_point& timePoint);

    // Parse date string (YYYY-MM-DD) to time_point
    static std::optional<std::chrono::system_clock::time_point> parseDate(const std::string& dateStr);

    // Parse date-time string (YYYY-MM-DD HH:MM:SS) to time_point
    static std::optional<std::chrono::system_clock::time_point> parseDateTime(const std::string& dateTimeStr);

    // Calculate number of days between two dates
    static int daysBetween(const std::string& startDate, const std::string& endDate);

    // Calculate number of days between two time_points
    static int daysBetween(const std::chrono::system_clock::time_point& start,
                          const std::chrono::system_clock::time_point& end);

    // Check if a date string is valid (YYYY-MM-DD)
    static bool isValidDate(const std::string& dateStr);

    // Check if a date-time string is valid (YYYY-MM-DD HH:MM:SS)
    static bool isValidDateTime(const std::string& dateTimeStr);

    // Check if date is in range [startDate, endDate]
    static bool isDateInRange(const std::string& date,
                             const std::string& startDate,
                             const std::string& endDate);

    // Add days to a date
    static std::string addDays(const std::string& dateStr, int days);

    // Subtract days from a date
    static std::string subtractDays(const std::string& dateStr, int days);

    // Compare two dates (returns -1 if date1 < date2, 0 if equal, 1 if date1 > date2)
    static int compareDates(const std::string& date1, const std::string& date2);

    // Check if year is a leap year
    static bool isLeapYear(int year);

    // Get number of days in a month
    static int getDaysInMonth(int year, int month);

    // Get day of week (0 = Sunday, 1 = Monday, ..., 6 = Saturday)
    static int getDayOfWeek(const std::string& dateStr);

    // Get week number of year
    static int getWeekOfYear(const std::string& dateStr);

    // Format date for display (e.g., "January 15, 2026")
    static std::string formatDateForDisplay(const std::string& dateStr);

    // Get time as HH:MM:SS string
    static std::string getCurrentTime();

private:
    // Helper to convert std::tm to time_point
    static std::chrono::system_clock::time_point tmToTimePoint(const std::tm& tm);

    // Helper to convert time_point to std::tm
    static std::tm timePointToTm(const std::chrono::system_clock::time_point& tp);
};

} // namespace HotelManagement
