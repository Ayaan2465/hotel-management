#pragma once
#include <iostream>
#include <string>
#include "Utils.h"

// Room type names in one place (instead of "Single Room" typed in many files)
namespace RoomTypes {
    // constexpr at namespace scope has internal linkage -> safe in a header
    // (works on old compilers too, no C++17 "inline variable" needed)
    constexpr const char* SINGLE = "Single Room";
    constexpr const char* DOUBLE = "Double Room";
    constexpr const char* SUITE  = "Suite";
}

// ============================================================
// ABSTRACTION: Room is an Abstract Base Class
// Cannot create Room object directly
// All room types MUST implement calculatePrice()
// ============================================================
class Room {
protected:
    int         roomNumber;
    int         floor;
    bool        isBooked;
    std::string roomType;

public:
    Room(int number, int floor, std::string type)
        : roomNumber(number), floor(floor),
          isBooked(false), roomType(std::move(type)) {}

    virtual ~Room() {}

    // ---- ABSTRACTION: Pure Virtual ----
    virtual double calculatePrice(int days) const = 0;
    virtual double getPricePerDay()         const = 0;

    // ---- Getters (ENCAPSULATION) ----
    int         getRoomNumber() const { return roomNumber; }
    int         getFloor()      const { return floor; }
    bool        getIsBooked()   const { return isBooked; }
    std::string getRoomType()   const { return roomType; }

    // ---- Room actions ----
    void book()   { isBooked = true; }
    void vacate() { isBooked = false; }

    // ---- Display ----
    virtual void displayInfo() const {
        std::cout << "  Room No : " << roomNumber << std::endl;
        std::cout << "  Type    : " << roomType   << std::endl;
        std::cout << "  Floor   : " << floor      << std::endl;
        std::cout << "  Price   : Rs." << util::formatMoney(getPricePerDay()) << "/day" << std::endl;
        std::cout << "  Status  : " << (isBooked ? "BOOKED" : "AVAILABLE") << std::endl;
    }

    // ---- OPERATOR OVERLOADING ----
    friend std::ostream& operator<<(std::ostream& os, const Room& r) {
        os << "Room " << r.roomNumber
           << " [" << r.roomType << "]"
           << " Floor-" << r.floor
           << " Rs." << util::formatMoney(r.getPricePerDay()) << "/day"
           << " | " << (r.isBooked ? "BOOKED" : "AVAILABLE");
        return os;
    }
};