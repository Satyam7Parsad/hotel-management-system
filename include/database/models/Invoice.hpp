#pragma once

#include <string>

namespace HotelManagement {

enum class InvoiceStatus {
    Unpaid,
    PartiallyPaid,
    Paid,
    Cancelled
};

struct Invoice {
    int id = 0;
    int bookingId = 0;
    std::string invoiceNumber;
    std::string issueDate; // YYYY-MM-DD HH:MM:SS
    std::string dueDate;   // YYYY-MM-DD
    double subtotal = 0.0;
    double taxAmount = 0.0;
    double discountAmount = 0.0;
    double totalAmount = 0.0;
    InvoiceStatus status = InvoiceStatus::Unpaid;
    std::string createdAt;

    // Default constructor
    Invoice() = default;

    // Helper methods
    bool isValid() const {
        return bookingId > 0 && !invoiceNumber.empty() &&
               subtotal >= 0.0 && totalAmount >= 0.0;
    }

    bool isPaid() const {
        return status == InvoiceStatus::Paid;
    }

    double getBalanceDue() const {
        // This would need to be calculated based on payments
        // For now, return total amount
        return totalAmount;
    }

    // Convert status enum to string
    std::string statusToString() const {
        switch (status) {
            case InvoiceStatus::Unpaid:        return "unpaid";
            case InvoiceStatus::PartiallyPaid: return "partially_paid";
            case InvoiceStatus::Paid:          return "paid";
            case InvoiceStatus::Cancelled:     return "cancelled";
            default:                           return "unknown";
        }
    }

    // Convert string to status enum
    static InvoiceStatus stringToStatus(const std::string& statusStr) {
        if (statusStr == "unpaid") return InvoiceStatus::Unpaid;
        if (statusStr == "partially_paid") return InvoiceStatus::PartiallyPaid;
        if (statusStr == "paid") return InvoiceStatus::Paid;
        if (statusStr == "cancelled") return InvoiceStatus::Cancelled;
        return InvoiceStatus::Unpaid; // default
    }

    std::string toString() const {
        return "Invoice " + invoiceNumber + " ($" + std::to_string(totalAmount) +
               ", " + statusToString() + ")";
    }
};

} // namespace HotelManagement
