#pragma once

#include <string>
#include <map>
#include <optional>

namespace HotelManagement {

class Config {
public:
    Config() = default;
    ~Config() = default;

    // Load configuration from INI file
    bool load(const std::string& filename);

    // Get string value
    std::optional<std::string> getString(const std::string& section, const std::string& key) const;
    std::string getString(const std::string& section, const std::string& key, const std::string& defaultValue) const;

    // Get integer value
    std::optional<int> getInt(const std::string& section, const std::string& key) const;
    int getInt(const std::string& section, const std::string& key, int defaultValue) const;

    // Get float value
    std::optional<float> getFloat(const std::string& section, const std::string& key) const;
    float getFloat(const std::string& section, const std::string& key, float defaultValue) const;

    // Get boolean value
    std::optional<bool> getBool(const std::string& section, const std::string& key) const;
    bool getBool(const std::string& section, const std::string& key, bool defaultValue) const;

    // Set value
    void set(const std::string& section, const std::string& key, const std::string& value);

    // Check if key exists
    bool has(const std::string& section, const std::string& key) const;

    // Save configuration to file
    bool save(const std::string& filename) const;

    // Database connection helpers
    std::string getDatabaseHost() const;
    int getDatabasePort() const;
    std::string getDatabaseName() const;
    std::string getDatabaseUser() const;
    std::string getDatabasePassword() const;
    std::string getDatabaseSSLMode() const;

    // Build connection string for libpqxx
    std::string buildConnectionString() const;

    // Application settings helpers
    int getWindowWidth() const;
    int getWindowHeight() const;
    std::string getWindowTitle() const;
    bool isFullscreen() const;
    bool isVSyncEnabled() const;
    int getMSAASamples() const;

    // Logging settings
    std::string getLogLevel() const;
    std::string getLogFile() const;

    // Clear all configuration
    void clear();

private:
    // Section -> (Key -> Value)
    std::map<std::string, std::map<std::string, std::string>> data;

    // Helper to trim whitespace
    static std::string trim(const std::string& str);

    // Helper to convert string to lowercase
    static std::string toLower(const std::string& str);
};

} // namespace HotelManagement
