#pragma once

#include <string>
#include <regex>

namespace HotelManagement {

class Validators {
public:
    // Email validation (RFC 5322 simplified)
    static bool isValidEmail(const std::string& email);

    // Phone number validation (international format)
    static bool isValidPhone(const std::string& phone);

    // Price validation (positive decimal)
    static bool isValidPrice(double price);
    static bool isValidPrice(const std::string& priceStr);

    // Room number validation (alphanumeric, typically 3-4 characters)
    static bool isValidRoomNumber(const std::string& roomNumber);

    // Name validation (letters, spaces, hyphens, apostrophes)
    static bool isValidName(const std::string& name);

    // ID number validation (alphanumeric)
    static bool isValidIDNumber(const std::string& idNumber);

    // Postal/ZIP code validation
    static bool isValidPostalCode(const std::string& postalCode);

    // Currency validation
    static bool isValidCurrency(const std::string& amount);

    // Percentage validation (0-100)
    static bool isValidPercentage(double percentage);
    static bool isValidPercentage(const std::string& percentageStr);

    // Positive integer validation
    static bool isValidPositiveInteger(int value);
    static bool isValidPositiveInteger(const std::string& valueStr);

    // String length validation
    static bool isValidLength(const std::string& str, size_t minLength, size_t maxLength);

    // Alphanumeric validation
    static bool isAlphanumeric(const std::string& str);

    // Alphabetic validation (letters only)
    static bool isAlphabetic(const std::string& str);

    // Numeric validation (digits only)
    static bool isNumeric(const std::string& str);

    // Sanitize input (remove potentially harmful characters)
    static std::string sanitizeInput(const std::string& input);

    // Trim whitespace
    static std::string trim(const std::string& str);

    // Password strength validation (minimum 8 chars, 1 uppercase, 1 lowercase, 1 digit)
    static bool isStrongPassword(const std::string& password);

    // Credit card number validation (Luhn algorithm)
    static bool isValidCreditCard(const std::string& cardNumber);

    // Transaction ID validation
    static bool isValidTransactionID(const std::string& transactionID);

    // Booking reference validation
    static bool isValidBookingReference(const std::string& reference);

private:
    // Helper: Check if string matches regex pattern
    static bool matchesPattern(const std::string& str, const std::string& pattern);
};

} // namespace HotelManagement
