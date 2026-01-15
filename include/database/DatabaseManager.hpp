#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <functional>
#include <pqxx/pqxx>

namespace HotelManagement {

class DatabaseManager {
public:
    // Constructor with connection string
    explicit DatabaseManager(const std::string& connectionString);

    // Destructor
    ~DatabaseManager();

    // Connect to database
    bool connect();

    // Disconnect from database
    void disconnect();

    // Check if connected
    bool isConnected() const;

    // Get connection reference (for repository use)
    pqxx::connection& getConnection();

    // Execute a transaction with automatic commit/rollback
    template<typename Func>
    auto executeTransaction(Func&& func) -> decltype(func(std::declval<pqxx::work&>())) {
        std::lock_guard<std::mutex> lock(dbMutex);

        if (!connection || !connection->is_open()) {
            throw std::runtime_error("Database not connected");
        }

        pqxx::work txn(*connection);

        try {
            auto result = func(txn);
            txn.commit();
            return result;
        } catch (const std::exception& e) {
            // Transaction will automatically rollback when txn goes out of scope
            throw;
        }
    }

    // Execute a read-only transaction (potentially more efficient)
    template<typename Func>
    auto executeReadTransaction(Func&& func) -> decltype(func(std::declval<pqxx::nontransaction&>())) {
        std::lock_guard<std::mutex> lock(dbMutex);

        if (!connection || !connection->is_open()) {
            throw std::runtime_error("Database not connected");
        }

        pqxx::nontransaction txn(*connection);
        return func(txn);
    }

    // Health check - verify database connection is alive
    bool ping();

    // Get last error message
    std::string getLastError() const;

    // Delete copy constructor and assignment operator
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

private:
    std::string connectionString;
    std::unique_ptr<pqxx::connection> connection;
    mutable std::mutex dbMutex;
    std::string lastError;

    // Helper to log database errors
    void logError(const std::string& error);
};

} // namespace HotelManagement
