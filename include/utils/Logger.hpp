#pragma once

#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <sstream>
#include <chrono>
#include <iomanip>

namespace HotelManagement {

enum class LogLevel {
    DEBUG = 0,
    INFO = 1,
    WARNING = 2,
    ERROR = 3
};

class Logger {
public:
    // Singleton instance
    static Logger& getInstance();

    // Initialize logger with file path
    static void init(const std::string& filename = "hotel_system.log");

    // Set minimum log level
    static void setLevel(LogLevel level);

    // Logging methods
    static void debug(const std::string& message);
    static void info(const std::string& message);
    static void warning(const std::string& message);
    static void error(const std::string& message);

    // Template for easier logging with multiple arguments
    template<typename... Args>
    static void debug(Args&&... args) {
        log(LogLevel::DEBUG, formatMessage(std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void info(Args&&... args) {
        log(LogLevel::INFO, formatMessage(std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void warning(Args&&... args) {
        log(LogLevel::WARNING, formatMessage(std::forward<Args>(args)...));
    }

    template<typename... Args>
    static void error(Args&&... args) {
        log(LogLevel::ERROR, formatMessage(std::forward<Args>(args)...));
    }

    // Close log file
    static void shutdown();

    // Delete copy constructor and assignment operator
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

private:
    Logger() = default;
    ~Logger();

    void initImpl(const std::string& filename);
    void setLevelImpl(LogLevel level);
    void logImpl(LogLevel level, const std::string& message);
    void shutdownImpl();

    static void log(LogLevel level, const std::string& message);

    // Helper to format current timestamp
    static std::string getCurrentTimestamp();

    // Helper to convert log level to string
    static std::string levelToString(LogLevel level);

    // Helper to format multiple arguments into a single string
    template<typename... Args>
    static std::string formatMessage(Args&&... args) {
        std::ostringstream oss;
        (oss << ... << std::forward<Args>(args));
        return oss.str();
    }

    std::ofstream logFile;
    LogLevel currentLevel = LogLevel::INFO;
    std::mutex logMutex;
    bool initialized = false;
};

} // namespace HotelManagement
