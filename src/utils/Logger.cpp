#include "utils/Logger.hpp"
#include <iostream>
#include <ctime>

namespace HotelManagement {

Logger::~Logger() {
    shutdownImpl();
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::init(const std::string& filename) {
    getInstance().initImpl(filename);
}

void Logger::setLevel(LogLevel level) {
    getInstance().setLevelImpl(level);
}

void Logger::debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void Logger::info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void Logger::warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void Logger::error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

void Logger::shutdown() {
    getInstance().shutdownImpl();
}

void Logger::initImpl(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);

    if (initialized) {
        return; // Already initialized
    }

    logFile.open(filename, std::ios::app);
    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
        return;
    }

    initialized = true;

    // Write initialization message
    logFile << "\n";
    logFile << "========================================\n";
    logFile << "Logger initialized at " << getCurrentTimestamp() << "\n";
    logFile << "========================================\n";
    logFile.flush();
}

void Logger::setLevelImpl(LogLevel level) {
    std::lock_guard<std::mutex> lock(logMutex);
    currentLevel = level;
}

void Logger::log(LogLevel level, const std::string& message) {
    getInstance().logImpl(level, message);
}

void Logger::logImpl(LogLevel level, const std::string& message) {
    std::lock_guard<std::mutex> lock(logMutex);

    // Check if message should be logged based on current level
    if (static_cast<int>(level) < static_cast<int>(currentLevel)) {
        return;
    }

    std::string logEntry = getCurrentTimestamp() + " [" + levelToString(level) + "] " + message;

    // Write to file
    if (initialized && logFile.is_open()) {
        logFile << logEntry << std::endl;
        logFile.flush(); // Ensure immediate write
    }

    // Also output to console for ERROR and WARNING
    if (level == LogLevel::ERROR || level == LogLevel::WARNING) {
        std::cerr << logEntry << std::endl;
    } else if (level == LogLevel::INFO) {
        std::cout << logEntry << std::endl;
    }
}

void Logger::shutdownImpl() {
    std::lock_guard<std::mutex> lock(logMutex);

    if (initialized && logFile.is_open()) {
        logFile << "========================================\n";
        logFile << "Logger shutdown at " << getCurrentTimestamp() << "\n";
        logFile << "========================================\n\n";
        logFile.close();
        initialized = false;
    }
}

std::string Logger::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::tm tm_buf;
#ifdef _WIN32
    localtime_s(&tm_buf, &time);
#else
    localtime_r(&time, &tm_buf);
#endif

    std::ostringstream oss;
    oss << std::put_time(&tm_buf, "%Y-%m-%d %H:%M:%S");
    oss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return oss.str();
}

std::string Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::DEBUG:   return "DEBUG  ";
        case LogLevel::INFO:    return "INFO   ";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR  ";
        default:                return "UNKNOWN";
    }
}

} // namespace HotelManagement
