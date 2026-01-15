#include "utils/Validators.hpp"
#include <algorithm>
#include <cctype>
#include <stdexcept>

namespace HotelManagement {

bool Validators::isValidEmail(const std::string& email) {
    // Simplified RFC 5322 email regex
    const std::string pattern = R"(^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$)";
    return matchesPattern(email, pattern);
}

bool Validators::isValidPhone(const std::string& phone) {
    // International phone format: +[country code]-[number] or just digits with optional spaces/dashes
    const std::string pattern = R"(^[\+]?[(]?[0-9]{1,4}[)]?[-\s\.]?[(]?[0-9]{1,4}[)]?[-\s\.]?[0-9]{1,9}$)";
    return matchesPattern(phone, pattern);
}

bool Validators::isValidPrice(double price) {
    return price >= 0.0;
}

bool Validators::isValidPrice(const std::string& priceStr) {
    try {
        double price = std::stod(priceStr);
        return isValidPrice(price);
    } catch (...) {
        return false;
    }
}

bool Validators::isValidRoomNumber(const std::string& roomNumber) {
    // Room numbers: typically 2-10 characters, alphanumeric
    if (roomNumber.empty() || roomNumber.length() > 10) {
        return false;
    }
    return isAlphanumeric(roomNumber);
}

bool Validators::isValidName(const std::string& name) {
    if (name.empty() || name.length() > 100) {
        return false;
    }

    // Allow letters, spaces, hyphens, apostrophes
    const std::string pattern = R"(^[a-zA-Z\s\-'\.]+$)";
    return matchesPattern(name, pattern);
}

bool Validators::isValidIDNumber(const std::string& idNumber) {
    if (idNumber.empty() || idNumber.length() > 100) {
        return false;
    }

    // Allow alphanumeric and hyphens
    const std::string pattern = R"(^[a-zA-Z0-9\-]+$)";
    return matchesPattern(idNumber, pattern);
}

bool Validators::isValidPostalCode(const std::string& postalCode) {
    if (postalCode.empty() || postalCode.length() > 20) {
        return false;
    }

    // Allow alphanumeric, spaces, hyphens
    const std::string pattern = R"(^[a-zA-Z0-9\s\-]+$)";
    return matchesPattern(postalCode, pattern);
}

bool Validators::isValidCurrency(const std::string& amount) {
    // Format: optional sign, digits, optional decimal point with 2 digits
    const std::string pattern = R"(^[\+\-]?[0-9]+(\.[0-9]{1,2})?$)";
    return matchesPattern(amount, pattern);
}

bool Validators::isValidPercentage(double percentage) {
    return percentage >= 0.0 && percentage <= 100.0;
}

bool Validators::isValidPercentage(const std::string& percentageStr) {
    try {
        double percentage = std::stod(percentageStr);
        return isValidPercentage(percentage);
    } catch (...) {
        return false;
    }
}

bool Validators::isValidPositiveInteger(int value) {
    return value > 0;
}

bool Validators::isValidPositiveInteger(const std::string& valueStr) {
    try {
        int value = std::stoi(valueStr);
        return isValidPositiveInteger(value);
    } catch (...) {
        return false;
    }
}

bool Validators::isValidLength(const std::string& str, size_t minLength, size_t maxLength) {
    return str.length() >= minLength && str.length() <= maxLength;
}

bool Validators::isAlphanumeric(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isalnum(c);
    });
}

bool Validators::isAlphabetic(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isalpha(c);
    });
}

bool Validators::isNumeric(const std::string& str) {
    if (str.empty()) {
        return false;
    }
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isdigit(c);
    });
}

std::string Validators::sanitizeInput(const std::string& input) {
    std::string sanitized;
    sanitized.reserve(input.length());

    for (char c : input) {
        // Remove control characters and special chars that could be harmful
        if (c >= 32 && c <= 126) { // Printable ASCII
            // Skip potentially harmful characters for SQL/HTML
            if (c != '<' && c != '>' && c != '&' && c != '"' && c != '\'') {
                sanitized += c;
            }
        }
    }

    return sanitized;
}

std::string Validators::trim(const std::string& str) {
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

bool Validators::isStrongPassword(const std::string& password) {
    if (password.length() < 8) {
        return false;
    }

    bool hasUpper = false;
    bool hasLower = false;
    bool hasDigit = false;

    for (char c : password) {
        if (std::isupper(c)) hasUpper = true;
        if (std::islower(c)) hasLower = true;
        if (std::isdigit(c)) hasDigit = true;
    }

    return hasUpper && hasLower && hasDigit;
}

bool Validators::isValidCreditCard(const std::string& cardNumber) {
    // Remove spaces and dashes
    std::string cleaned;
    for (char c : cardNumber) {
        if (std::isdigit(c)) {
            cleaned += c;
        }
    }

    // Card number should be 13-19 digits
    if (cleaned.length() < 13 || cleaned.length() > 19) {
        return false;
    }

    // Luhn algorithm
    int sum = 0;
    bool alternate = false;

    for (int i = cleaned.length() - 1; i >= 0; i--) {
        int digit = cleaned[i] - '0';

        if (alternate) {
            digit *= 2;
            if (digit > 9) {
                digit -= 9;
            }
        }

        sum += digit;
        alternate = !alternate;
    }

    return (sum % 10 == 0);
}

bool Validators::isValidTransactionID(const std::string& transactionID) {
    if (transactionID.empty() || transactionID.length() > 255) {
        return false;
    }

    // Allow alphanumeric, hyphens, underscores
    const std::string pattern = R"(^[a-zA-Z0-9\-_]+$)";
    return matchesPattern(transactionID, pattern);
}

bool Validators::isValidBookingReference(const std::string& reference) {
    if (reference.empty() || reference.length() > 50) {
        return false;
    }

    // Allow alphanumeric and hyphens
    const std::string pattern = R"(^[A-Z0-9\-]+$)";
    return matchesPattern(reference, pattern);
}

bool Validators::matchesPattern(const std::string& str, const std::string& pattern) {
    try {
        std::regex regex(pattern);
        return std::regex_match(str, regex);
    } catch (const std::regex_error& e) {
        return false;
    }
}

} // namespace HotelManagement
