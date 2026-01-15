#include "database/DatabaseManager.hpp"
#include "utils/Logger.hpp"
#include <stdexcept>

namespace HotelManagement {

DatabaseManager::DatabaseManager(const std::string& connStr)
    : connectionString(connStr), connection(nullptr) {
}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect() {
    std::lock_guard<std::mutex> lock(dbMutex);

    try {
        if (connection && connection->is_open()) {
            Logger::info("Database already connected");
            return true;
        }

        Logger::info("Connecting to database...");
        connection = std::make_unique<pqxx::connection>(connectionString);

        if (!connection->is_open()) {
            logError("Failed to open database connection");
            return false;
        }

        Logger::info("Database connected successfully");
        Logger::info("PostgreSQL version: ", connection->server_version());

        return true;

    } catch (const pqxx::broken_connection& e) {
        logError(std::string("Connection error: ") + e.what());
        return false;
    } catch (const std::exception& e) {
        logError(std::string("Connection exception: ") + e.what());
        return false;
    }
}

void DatabaseManager::disconnect() {
    std::lock_guard<std::mutex> lock(dbMutex);

    try {
        if (connection && connection->is_open()) {
            connection->close();
            Logger::info("Database disconnected");
        }
        connection.reset();
    } catch (const std::exception& e) {
        logError(std::string("Disconnect error: ") + e.what());
    }
}

bool DatabaseManager::isConnected() const {
    std::lock_guard<std::mutex> lock(dbMutex);
    return connection && connection->is_open();
}

pqxx::connection& DatabaseManager::getConnection() {
    std::lock_guard<std::mutex> lock(dbMutex);

    if (!connection || !connection->is_open()) {
        throw std::runtime_error("Database not connected. Call connect() first.");
    }

    return *connection;
}

bool DatabaseManager::ping() {
    try {
        std::lock_guard<std::mutex> lock(dbMutex);

        if (!connection || !connection->is_open()) {
            return false;
        }

        // Execute a simple query to test connection
        pqxx::nontransaction txn(*connection);
        pqxx::result result = txn.exec("SELECT 1");

        return !result.empty();

    } catch (const std::exception& e) {
        logError(std::string("Ping failed: ") + e.what());
        return false;
    }
}

std::string DatabaseManager::getLastError() const {
    std::lock_guard<std::mutex> lock(dbMutex);
    return lastError;
}

void DatabaseManager::logError(const std::string& error) {
    lastError = error;
    Logger::error("DatabaseManager: ", error);
}

} // namespace HotelManagement
