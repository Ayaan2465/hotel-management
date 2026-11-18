#pragma once
#include <string>
#include <iostream>

// ============================================================
// Guest Class — ENCAPSULATION
// All data private, access through getters
// ============================================================
class Guest {
private:
    int         guestId;
    std::string name;
    std::string phone;
    std::string address;

    // FIX: "inline" lets the definition live in the header safely.
    // (Before: `int Guest::nextId = 1001;` in the header -> linker
    //  "multiple definition" as soon as 2 .cpp files included it.)
    static inline int nextId = 1001;

public:
    Guest() : guestId(0), name(""), phone(""), address("") {}

    Guest(std::string name, std::string phone, std::string address)
        : guestId(nextId++), name(std::move(name)),
          phone(std::move(phone)), address(std::move(address)) {}

    int         getGuestId() const { return guestId; }
    std::string getName()    const { return name; }
    std::string getPhone()   const { return phone; }
    std::string getAddress() const { return address; }

    void displayInfo() const {
        std::cout << "  Guest ID : " << guestId  << std::endl;
        std::cout << "  Name     : " << name     << std::endl;
        std::cout << "  Phone    : " << phone    << std::endl;
        std::cout << "  Address  : " << address  << std::endl;
    }

    friend std::ostream& operator<<(std::ostream& os, const Guest& g) {
        os << "Guest #" << g.guestId
           << " | " << g.name
           << " | Ph: " << g.phone;
        return os;
    }
};
