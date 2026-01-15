#pragma once

#include "database/DatabaseManager.hpp"
#include "database/models/Guest.hpp"
#include <vector>
#include <optional>

namespace HotelManagement {

class GuestRepository {
public:
    explicit GuestRepository(DatabaseManager& dbManager);
    ~GuestRepository() = default;

    // CRUD operations
    std::optional<Guest> findById(int id);
    std::vector<Guest> findAll();
    std::vector<Guest> searchByName(const std::string& name);
    std::optional<Guest> findByEmail(const std::string& email);
    std::optional<Guest> findByPhone(const std::string& phone);

    int create(const Guest& guest);
    bool update(const Guest& guest);
    bool deleteById(int id);

    // Statistics
    int getTotalGuests();
    int getVIPCount();

private:
    DatabaseManager& dbManager;
    Guest rowToGuest(const pqxx::row& row);
};

} // namespace HotelManagement
