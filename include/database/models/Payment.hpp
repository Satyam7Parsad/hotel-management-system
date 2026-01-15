#pragma once

#include <string>

namespace HotelManagement {

enum class PaymentMethod {
    Cash,
    CreditCard,
    DebitCard,
    BankTransfer,
    MobilePayment
};

enum class PaymentStatus {
    Pending,
    Completed,
    Failed,
    Refunded
};

struct Payment {
    int id = 0;
    int bookingId = 0;
    double amount = 0.0;
    PaymentMethod paymentMethod = PaymentMethod::Cash;
    PaymentStatus status = PaymentStatus::Pending;
    std::string transactionId;
    std::string paymentDate; // YYYY-MM-DD HH:MM:SS
    std::string notes;
    std::string createdAt;

    // Default constructor
    Payment() = default;

    // Helper methods
    bool isValid() const {
        return bookingId > 0 && amount > 0.0;
    }

    bool isCompleted() const {
        return status == PaymentStatus::Completed;
    }

    // Convert payment method enum to string
    std::string paymentMethodToString() const {
        switch (paymentMethod) {
            case PaymentMethod::Cash:          return "cash";
            case PaymentMethod::CreditCard:    return "credit_card";
            case PaymentMethod::DebitCard:     return "debit_card";
            case PaymentMethod::BankTransfer:  return "bank_transfer";
            case PaymentMethod::MobilePayment: return "mobile_payment";
            default:                           return "unknown";
        }
    }

    // Convert string to payment method enum
    static PaymentMethod stringToPaymentMethod(const std::string& methodStr) {
        if (methodStr == "cash") return PaymentMethod::Cash;
        if (methodStr == "credit_card") return PaymentMethod::CreditCard;
        if (methodStr == "debit_card") return PaymentMethod::DebitCard;
        if (methodStr == "bank_transfer") return PaymentMethod::BankTransfer;
        if (methodStr == "mobile_payment") return PaymentMethod::MobilePayment;
        return PaymentMethod::Cash; // default
    }

    // Convert status enum to string
    std::string statusToString() const {
        switch (status) {
            case PaymentStatus::Pending:   return "pending";
            case PaymentStatus::Completed: return "completed";
            case PaymentStatus::Failed:    return "failed";
            case PaymentStatus::Refunded:  return "refunded";
            default:                       return "unknown";
        }
    }

    // Convert string to status enum
    static PaymentStatus stringToStatus(const std::string& statusStr) {
        if (statusStr == "pending") return PaymentStatus::Pending;
        if (statusStr == "completed") return PaymentStatus::Completed;
        if (statusStr == "failed") return PaymentStatus::Failed;
        if (statusStr == "refunded") return PaymentStatus::Refunded;
        return PaymentStatus::Pending; // default
    }

    std::string toString() const {
        return "Payment #" + std::to_string(id) + " ($" + std::to_string(amount) +
               ", " + statusToString() + ")";
    }
};

} // namespace HotelManagement
