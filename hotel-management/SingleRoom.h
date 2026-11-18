#pragma once
#include "Room.h"

// INHERITANCE: SingleRoom inherits Room
// POLYMORPHISM: calculatePrice() override
class SingleRoom : public Room {
private:
    static constexpr int PRICE_PER_DAY = 1000;

public:
    SingleRoom(int number, int floor)
        : Room(number, floor, RoomTypes::SINGLE) {}

    double calculatePrice(int days) const override {
        return static_cast<double>(PRICE_PER_DAY) * days;
    }

    double getPricePerDay() const override {
        return PRICE_PER_DAY;
    }
};
