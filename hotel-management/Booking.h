#pragma once
#include <string>
#include <iostream>
#include "Guest.h"
#include "Room.h"
#include "Utils.h"

// ============================================================
// Booking Class — ties Guest + Room together
// ENCAPSULATION: All fields private
// ============================================================
class Booking {
private:
    int         bookingId;
    Guest       guest;
    Room*       room;        // non-owning pointer (Hotel owns the rooms)
    std::string checkInDate;
    std::string checkOutDate;
    int         days;
    double      roomService; // Extra charges
    bool        isActive;

    // FIX: function-local static (no linker error, works on old compilers)
    static int nextBookingId() {
        static int id = 5001;
        return id++;
    }
    static constexpr double TAX_RATE = 0.10;  // FIX: no magic number

public:
    Booking(const Guest& g, Room* r, std::string checkIn, std::string checkOut, int days)
        : bookingId(nextBookingId()), guest(g), room(r),
          checkInDate(std::move(checkIn)), checkOutDate(std::move(checkOut)),
          days(days), roomService(0), isActive(true) {}

    int                getBookingId() const { return bookingId; }
    const Guest&       getGuest()     const { return guest; }   // FIX: no copy
    Room*              getRoom()      const { return room; }
    const std::string& getCheckIn()   const { return checkInDate; }
    const std::string& getCheckOut()  const { return checkOutDate; }
    int                getDays()      const { return days; }
    bool               getIsActive()  const { return isActive; }

    // FIX: negative (or NaN) room service is rejected. Returns false if refused.
    bool setRoomService(double amount) {
        if (!(amount >= 0)) return false;
        roomService = amount;
        return true;
    }
    void deactivate() { isActive = false; }

    void generateBill() const {
        double roomCharge = room->calculatePrice(days);   // POLYMORPHISM
        double tax        = (roomCharge + roomService) * TAX_RATE;
        double total      = roomCharge + roomService + tax;

        std::cout << std::endl;
        std::cout << "  ======================================" << std::endl;
        std::cout << "          HOTEL BILL / INVOICE          " << std::endl;
        std::cout << "  ======================================" << std::endl;
        std::cout << "  Booking ID   : " << bookingId             << std::endl;
        std::cout << "  Guest Name   : " << guest.getName()       << std::endl;
        std::cout << "  Phone        : " << guest.getPhone()      << std::endl;
        std::cout << "  Room Number  : " << room->getRoomNumber() << std::endl;
        std::cout << "  Room Type    : " << room->getRoomType()   << std::endl;
        std::cout << "  Check-in     : " << checkInDate           << std::endl;
        std::cout << "  Check-out    : " << checkOutDate          << std::endl;
        std::cout << "  Days Stayed  : " << days                  << std::endl;
        std::cout << "  --------------------------------------" << std::endl;
        std::cout << "  Room Charge  : Rs." << util::formatMoney(roomCharge) << std::endl;
        std::cout << "  Room Service : Rs." << util::formatMoney(roomService) << std::endl;
        std::cout << "  Tax (" << static_cast<int>(TAX_RATE * 100) << "%)    : Rs."
                  << util::formatMoney(tax) << std::endl;
        std::cout << "  --------------------------------------" << std::endl;
        std::cout << "  TOTAL        : Rs." << util::formatMoney(total) << std::endl;
        std::cout << "  ======================================" << std::endl;
    }

    void displaySummary() const {
        std::cout << "  Booking #" << bookingId
                  << " | " << guest.getName()
                  << " | Room " << room->getRoomNumber()
                  << " (" << room->getRoomType() << ")"
                  << " | " << checkInDate << " to " << checkOutDate
                  << " | " << days << " days"
                  << " | " << (isActive ? "ACTIVE" : "CHECKED-OUT")
                  << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Booking& b) {
        os << "Booking #" << b.bookingId
           << " | " << b.guest.getName()
           << " | Room " << b.room->getRoomNumber()
           << " | " << b.days << " days";
        return os;
    }
};