#include "database/repositories/GuestRepository.hpp"
#include "utils/Logger.hpp"

namespace HotelManagement {

GuestRepository::GuestRepository(DatabaseManager& db) : dbManager(db) {}

std::optional<Guest> GuestRepository::findById(int id) {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) -> std::optional<Guest> {
            auto result = txn.exec_params(
                "SELECT id, first_name, last_name, email, phone, address, id_type, id_number, "
                "date_of_birth, nationality, preferences::text, vip_status, created_at, updated_at "
                "FROM guests WHERE id = $1", id
            );
            if (result.empty()) return std::nullopt;
            return rowToGuest(result[0]);
        });
    } catch (const std::exception& e) {
        Logger::error("GuestRepository::findById failed: ", e.what());
        return std::nullopt;
    }
}

std::vector<Guest> GuestRepository::findAll() {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec(
                "SELECT id, first_name, last_name, email, phone, address, id_type, id_number, "
                "date_of_birth, nationality, preferences::text, vip_status, created_at, updated_at "
                "FROM guests ORDER BY last_name, first_name"
            );
            std::vector<Guest> guests;
            for (const auto& row : result) {
                guests.push_back(rowToGuest(row));
            }
            return guests;
        });
    } catch (const std::exception& e) {
        Logger::error("GuestRepository::findAll failed: ", e.what());
        return {};
    }
}

std::vector<Guest> GuestRepository::searchByName(const std::string& name) {
    try {
        std::string searchPattern = "%" + name + "%";
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec_params(
                "SELECT id, first_name, last_name, email, phone, address, id_type, id_number, "
                "date_of_birth, nationality, preferences::text, vip_status, created_at, updated_at "
                "FROM guests WHERE first_name ILIKE $1 OR last_name ILIKE $1",
                searchPattern
            );
            std::vector<Guest> guests;
            for (const auto& row : result) {
                guests.push_back(rowToGuest(row));
            }
            return guests;
        });
    } catch (const std::exception& e) {
        Logger::error("GuestRepository::searchByName failed: ", e.what());
        return {};
    }
}

int GuestRepository::create(const Guest& guest) {
    try {
        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params(
                "INSERT INTO guests (first_name, last_name, email, phone, address, id_type, "
                "id_number, date_of_birth, nationality, vip_status) "
                "VALUES ($1, $2, $3, $4, $5, $6, $7, $8, $9, $10) RETURNING id",
                guest.firstName, guest.lastName, guest.email, guest.phone, guest.address,
                guest.idType, guest.idNumber, guest.dateOfBirth, guest.nationality, guest.vipStatus
            );
            return result[0][0].as<int>();
        });
    } catch (const std::exception& e) {
        Logger::error("GuestRepository::create failed: ", e.what());
        return -1;
    }
}

bool GuestRepository::update(const Guest& guest) {
    try {
        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params(
                "UPDATE guests SET first_name=$1, last_name=$2, email=$3, phone=$4, address=$5, "
                "id_type=$6, id_number=$7, date_of_birth=$8, nationality=$9, vip_status=$10 WHERE id=$11",
                guest.firstName, guest.lastName, guest.email, guest.phone, guest.address,
                guest.idType, guest.idNumber, guest.dateOfBirth, guest.nationality, guest.vipStatus, guest.id
            );
            return result.affected_rows() > 0;
        });
    } catch (const std::exception& e) {
        Logger::error("GuestRepository::update failed: ", e.what());
        return false;
    }
}

bool GuestRepository::deleteById(int id) {
    try {
        return dbManager.executeTransaction([&](pqxx::work& txn) {
            auto result = txn.exec_params("DELETE FROM guests WHERE id = $1", id);
            return result.affected_rows() > 0;
        });
    } catch (const std::exception& e) {
        Logger::error("GuestRepository::deleteById failed: ", e.what());
        return false;
    }
}

int GuestRepository::getTotalGuests() {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec("SELECT COUNT(*) FROM guests");
            return result[0][0].as<int>();
        });
    } catch (const std::exception& e) {
        return 0;
    }
}

int GuestRepository::getVIPCount() {
    try {
        return dbManager.executeReadTransaction([&](pqxx::nontransaction& txn) {
            auto result = txn.exec("SELECT COUNT(*) FROM guests WHERE vip_status = true");
            return result[0][0].as<int>();
        });
    } catch (const std::exception& e) {
        return 0;
    }
}

Guest GuestRepository::rowToGuest(const pqxx::row& row) {
    Guest guest;
    guest.id = row["id"].as<int>();
    guest.firstName = row["first_name"].as<std::string>();
    guest.lastName = row["last_name"].as<std::string>();
    guest.email = row["email"].is_null() ? "" : row["email"].as<std::string>();
    guest.phone = row["phone"].as<std::string>();
    guest.address = row["address"].is_null() ? "" : row["address"].as<std::string>();
    guest.idType = row["id_type"].as<std::string>();
    guest.idNumber = row["id_number"].as<std::string>();
    guest.dateOfBirth = row["date_of_birth"].is_null() ? "" : row["date_of_birth"].as<std::string>();
    guest.nationality = row["nationality"].is_null() ? "" : row["nationality"].as<std::string>();
    guest.preferencesJson = row["preferences"].is_null() ? "{}" : row["preferences"].as<std::string>();
    guest.vipStatus = row["vip_status"].as<bool>();
    guest.createdAt = row["created_at"].as<std::string>();
    guest.updatedAt = row["updated_at"].as<std::string>();
    return guest;
}

} // namespace HotelManagement
