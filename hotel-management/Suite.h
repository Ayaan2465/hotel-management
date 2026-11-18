#pragma once
#include "Room.h"

// INHERITANCE: Suite inherits Room
// POLYMORPHISM: calculatePrice() override
class Suite : public Room {
private:
    static constexpr int PRICE_PER_DAY = 3500;

public:
    Suite(int number, int floor)
        : Room(number, floor, RoomTypes::SUITE) {}

    double calculatePrice(int days) const override {
        return static_cast<double>(PRICE_PER_DAY) * days;
    }

    double getPricePerDay() const override {
        return PRICE_PER_DAY;
    }
};
