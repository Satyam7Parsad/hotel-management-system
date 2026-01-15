#include "utils/DateUtils.hpp"
#include "utils/Logger.hpp"
#include <sstream>
#include <iomanip>
#include <ctime>
#include <regex>

namespace HotelManagement {

std::string DateUtils::getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    return formatDate(now);
}

std::string DateUtils::getCurrentDateTime() {
    auto now = std::chrono::system_clock::now();
    return formatDateTime(now);
}

std::string DateUtils::formatDate(const std::chrono::system_clock::time_point& timePoint) {
    std::tm tm = timePointToTm(timePoint);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d");
    return oss.str();
}

std::string DateUtils::formatDateTime(const std::chrono::system_clock::time_point& timePoint) {
    std::tm tm = timePointToTm(timePoint);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::optional<std::chrono::system_clock::time_point> DateUtils::parseDate(const std::string& dateStr) {
    if (!isValidDate(dateStr)) {
        return std::nullopt;
    }

    std::tm tm = {};
    std::istringstream iss(dateStr);
    iss >> std::get_time(&tm, "%Y-%m-%d");

    if (iss.fail()) {
        return std::nullopt;
    }

    return tmToTimePoint(tm);
}

std::optional<std::chrono::system_clock::time_point> DateUtils::parseDateTime(const std::string& dateTimeStr) {
    if (!isValidDateTime(dateTimeStr)) {
        return std::nullopt;
    }

    std::tm tm = {};
    std::istringstream iss(dateTimeStr);
    iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if (iss.fail()) {
        return std::nullopt;
    }

    return tmToTimePoint(tm);
}

int DateUtils::daysBetween(const std::string& startDate, const std::string& endDate) {
    auto start = parseDate(startDate);
    auto end = parseDate(endDate);

    if (!start.has_value() || !end.has_value()) {
        Logger::error("Invalid date format in daysBetween: ", startDate, " or ", endDate);
        return 0;
    }

    return daysBetween(start.value(), end.value());
}

int DateUtils::daysBetween(const std::chrono::system_clock::time_point& start,
                          const std::chrono::system_clock::time_point& end) {
    auto duration = std::chrono::duration_cast<std::chrono::hours>(end - start);
    return duration.count() / 24;
}

bool DateUtils::isValidDate(const std::string& dateStr) {
    // Check format YYYY-MM-DD
    std::regex dateRegex(R"(^\d{4}-\d{2}-\d{2}$)");
    if (!std::regex_match(dateStr, dateRegex)) {
        return false;
    }

    // Parse and validate actual date values
    std::tm tm = {};
    std::istringstream iss(dateStr);
    iss >> std::get_time(&tm, "%Y-%m-%d");

    if (iss.fail()) {
        return false;
    }

    // Additional validation
    int year = tm.tm_year + 1900;
    int month = tm.tm_mon + 1;
    int day = tm.tm_mday;

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > getDaysInMonth(year, month)) return false;

    return true;
}

bool DateUtils::isValidDateTime(const std::string& dateTimeStr) {
    // Check format YYYY-MM-DD HH:MM:SS
    std::regex dateTimeRegex(R"(^\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}$)");
    if (!std::regex_match(dateTimeStr, dateTimeRegex)) {
        return false;
    }

    // Parse and validate
    std::tm tm = {};
    std::istringstream iss(dateTimeStr);
    iss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if (iss.fail()) {
        return false;
    }

    // Additional validation
    int year = tm.tm_year + 1900;
    int month = tm.tm_mon + 1;
    int day = tm.tm_mday;
    int hour = tm.tm_hour;
    int min = tm.tm_min;
    int sec = tm.tm_sec;

    if (year < 1900 || year > 2100) return false;
    if (month < 1 || month > 12) return false;
    if (day < 1 || day > getDaysInMonth(year, month)) return false;
    if (hour < 0 || hour > 23) return false;
    if (min < 0 || min > 59) return false;
    if (sec < 0 || sec > 59) return false;

    return true;
}

bool DateUtils::isDateInRange(const std::string& date,
                              const std::string& startDate,
                              const std::string& endDate) {
    auto datePoint = parseDate(date);
    auto startPoint = parseDate(startDate);
    auto endPoint = parseDate(endDate);

    if (!datePoint.has_value() || !startPoint.has_value() || !endPoint.has_value()) {
        return false;
    }

    return datePoint.value() >= startPoint.value() && datePoint.value() <= endPoint.value();
}

std::string DateUtils::addDays(const std::string& dateStr, int days) {
    auto timePoint = parseDate(dateStr);
    if (!timePoint.has_value()) {
        Logger::error("Invalid date format in addDays: ", dateStr);
        return dateStr;
    }

    auto newTimePoint = timePoint.value() + std::chrono::hours(24 * days);
    return formatDate(newTimePoint);
}

std::string DateUtils::subtractDays(const std::string& dateStr, int days) {
    return addDays(dateStr, -days);
}

int DateUtils::compareDates(const std::string& date1, const std::string& date2) {
    auto tp1 = parseDate(date1);
    auto tp2 = parseDate(date2);

    if (!tp1.has_value() || !tp2.has_value()) {
        Logger::error("Invalid date format in compareDates");
        return 0;
    }

    if (tp1.value() < tp2.value()) return -1;
    if (tp1.value() > tp2.value()) return 1;
    return 0;
}

bool DateUtils::isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

int DateUtils::getDaysInMonth(int year, int month) {
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (month < 1 || month > 12) {
        return 0;
    }

    if (month == 2 && isLeapYear(year)) {
        return 29;
    }

    return daysInMonth[month - 1];
}

int DateUtils::getDayOfWeek(const std::string& dateStr) {
    auto timePoint = parseDate(dateStr);
    if (!timePoint.has_value()) {
        Logger::error("Invalid date format in getDayOfWeek: ", dateStr);
        return 0;
    }

    std::tm tm = timePointToTm(timePoint.value());
    return tm.tm_wday;
}

int DateUtils::getWeekOfYear(const std::string& dateStr) {
    auto timePoint = parseDate(dateStr);
    if (!timePoint.has_value()) {
        Logger::error("Invalid date format in getWeekOfYear: ", dateStr);
        return 0;
    }

    std::tm tm = timePointToTm(timePoint.value());
    std::ostringstream oss;
    oss << std::put_time(&tm, "%W");
    return std::stoi(oss.str());
}

std::string DateUtils::formatDateForDisplay(const std::string& dateStr) {
    auto timePoint = parseDate(dateStr);
    if (!timePoint.has_value()) {
        return dateStr;
    }

    std::tm tm = timePointToTm(timePoint.value());
    std::ostringstream oss;
    oss << std::put_time(&tm, "%B %d, %Y");
    return oss.str();
}

std::string DateUtils::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    std::tm tm = timePointToTm(now);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%H:%M:%S");
    return oss.str();
}

// Private helper functions
std::chrono::system_clock::time_point DateUtils::tmToTimePoint(const std::tm& tm) {
    std::tm tm_copy = tm;
    std::time_t time = std::mktime(&tm_copy);
    return std::chrono::system_clock::from_time_t(time);
}

std::tm DateUtils::timePointToTm(const std::chrono::system_clock::time_point& tp) {
    std::time_t time = std::chrono::system_clock::to_time_t(tp);
    std::tm tm_buf;
#ifdef _WIN32
    localtime_s(&tm_buf, &time);
#else
    localtime_r(&time, &tm_buf);
#endif
    return tm_buf;
}

} // namespace HotelManagement
