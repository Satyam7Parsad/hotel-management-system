#include "core/Config.hpp"
#include "utils/Logger.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

namespace HotelManagement {

bool Config::load(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        Logger::error("Failed to open config file: ", filename);
        return false;
    }

    data.clear();
    std::string currentSection;
    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;
        line = trim(line);

        // Skip empty lines and comments
        if (line.empty() || line[0] == '#' || line[0] == ';') {
            continue;
        }

        // Check for section header [section]
        if (line[0] == '[' && line[line.length() - 1] == ']') {
            currentSection = line.substr(1, line.length() - 2);
            currentSection = trim(currentSection);
            continue;
        }

        // Parse key=value pairs
        size_t pos = line.find('=');
        if (pos != std::string::npos) {
            std::string key = trim(line.substr(0, pos));
            std::string value = trim(line.substr(pos + 1));

            if (currentSection.empty()) {
                Logger::warning("Key-value pair found outside of section at line ", lineNumber, ": ", line);
                continue;
            }

            data[currentSection][key] = value;
        } else {
            Logger::warning("Invalid line format at line ", lineNumber, ": ", line);
        }
    }

    file.close();
    Logger::info("Configuration loaded from: ", filename);
    return true;
}

std::optional<std::string> Config::getString(const std::string& section, const std::string& key) const {
    auto sectionIt = data.find(section);
    if (sectionIt == data.end()) {
        return std::nullopt;
    }

    auto keyIt = sectionIt->second.find(key);
    if (keyIt == sectionIt->second.end()) {
        return std::nullopt;
    }

    return keyIt->second;
}

std::string Config::getString(const std::string& section, const std::string& key, const std::string& defaultValue) const {
    auto value = getString(section, key);
    return value.has_value() ? value.value() : defaultValue;
}

std::optional<int> Config::getInt(const std::string& section, const std::string& key) const {
    auto value = getString(section, key);
    if (!value.has_value()) {
        return std::nullopt;
    }

    try {
        return std::stoi(value.value());
    } catch (const std::exception& e) {
        Logger::warning("Failed to parse integer value for [", section, "].", key, ": ", value.value());
        return std::nullopt;
    }
}

int Config::getInt(const std::string& section, const std::string& key, int defaultValue) const {
    auto value = getInt(section, key);
    return value.has_value() ? value.value() : defaultValue;
}

std::optional<float> Config::getFloat(const std::string& section, const std::string& key) const {
    auto value = getString(section, key);
    if (!value.has_value()) {
        return std::nullopt;
    }

    try {
        return std::stof(value.value());
    } catch (const std::exception& e) {
        Logger::warning("Failed to parse float value for [", section, "].", key, ": ", value.value());
        return std::nullopt;
    }
}

float Config::getFloat(const std::string& section, const std::string& key, float defaultValue) const {
    auto value = getFloat(section, key);
    return value.has_value() ? value.value() : defaultValue;
}

std::optional<bool> Config::getBool(const std::string& section, const std::string& key) const {
    auto value = getString(section, key);
    if (!value.has_value()) {
        return std::nullopt;
    }

    std::string lowerValue = toLower(value.value());
    if (lowerValue == "true" || lowerValue == "1" || lowerValue == "yes" || lowerValue == "on") {
        return true;
    } else if (lowerValue == "false" || lowerValue == "0" || lowerValue == "no" || lowerValue == "off") {
        return false;
    }

    Logger::warning("Failed to parse boolean value for [", section, "].", key, ": ", value.value());
    return std::nullopt;
}

bool Config::getBool(const std::string& section, const std::string& key, bool defaultValue) const {
    auto value = getBool(section, key);
    return value.has_value() ? value.value() : defaultValue;
}

void Config::set(const std::string& section, const std::string& key, const std::string& value) {
    data[section][key] = value;
}

bool Config::has(const std::string& section, const std::string& key) const {
    auto sectionIt = data.find(section);
    if (sectionIt == data.end()) {
        return false;
    }
    return sectionIt->second.find(key) != sectionIt->second.end();
}

bool Config::save(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        Logger::error("Failed to open config file for writing: ", filename);
        return false;
    }

    for (const auto& section : data) {
        file << "[" << section.first << "]\n";
        for (const auto& kv : section.second) {
            file << kv.first << "=" << kv.second << "\n";
        }
        file << "\n";
    }

    file.close();
    Logger::info("Configuration saved to: ", filename);
    return true;
}

// Database connection helpers
std::string Config::getDatabaseHost() const {
    return getString("database", "host", "localhost");
}

int Config::getDatabasePort() const {
    return getInt("database", "port", 5432);
}

std::string Config::getDatabaseName() const {
    return getString("database", "dbname", "hotel_management_db");
}

std::string Config::getDatabaseUser() const {
    return getString("database", "user", "hotel_user");
}

std::string Config::getDatabasePassword() const {
    return getString("database", "password", "");
}

std::string Config::getDatabaseSSLMode() const {
    return getString("database", "sslmode", "prefer");
}

std::string Config::buildConnectionString() const {
    std::ostringstream oss;
    oss << "host=" << getDatabaseHost()
        << " port=" << getDatabasePort()
        << " dbname=" << getDatabaseName()
        << " user=" << getDatabaseUser()
        << " password=" << getDatabasePassword()
        << " sslmode=" << getDatabaseSSLMode();
    return oss.str();
}

// Application settings helpers
int Config::getWindowWidth() const {
    return getInt("application", "window_width", 1920);
}

int Config::getWindowHeight() const {
    return getInt("application", "window_height", 1080);
}

std::string Config::getWindowTitle() const {
    return getString("application", "window_title", "Hotel Management System");
}

bool Config::isFullscreen() const {
    return getBool("application", "fullscreen", false);
}

bool Config::isVSyncEnabled() const {
    return getBool("application", "vsync", true);
}

int Config::getMSAASamples() const {
    return getInt("application", "msaa_samples", 4);
}

// Logging settings
std::string Config::getLogLevel() const {
    return getString("logging", "level", "INFO");
}

std::string Config::getLogFile() const {
    return getString("logging", "log_file", "hotel_system.log");
}

void Config::clear() {
    data.clear();
}

// Helper functions
std::string Config::trim(const std::string& str) {
    size_t start = 0;
    size_t end = str.length();

    while (start < end && std::isspace(static_cast<unsigned char>(str[start]))) {
        start++;
    }

    while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1]))) {
        end--;
    }

    return str.substr(start, end - start);
}

std::string Config::toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    return result;
}

} // namespace HotelManagement
