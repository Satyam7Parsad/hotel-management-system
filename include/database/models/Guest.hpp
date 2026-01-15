#pragma once

#include <string>

namespace HotelManagement {

struct Guest {
    int id = 0;
    std::string firstName;
    std::string lastName;
    std::string email;
    std::string phone;
    std::string address;
    std::string idType; // passport, drivers_license, national_id
    std::string idNumber;
    std::string dateOfBirth; // YYYY-MM-DD
    std::string nationality;
    std::string preferencesJson; // JSON string from database
    bool vipStatus = false;
    std::string createdAt;
    std::string updatedAt;

    // Default constructor
    Guest() = default;

    // Helper methods
    bool isValid() const {
        return !firstName.empty() && !lastName.empty() &&
               !phone.empty() && !idType.empty() && !idNumber.empty();
    }

    std::string getFullName() const {
        return firstName + " " + lastName;
    }

    std::string toString() const {
        std::string result = getFullName();
        if (vipStatus) {
            result += " (VIP)";
        }
        return result;
    }
};

} // namespace HotelManagement
