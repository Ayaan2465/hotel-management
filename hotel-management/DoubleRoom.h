#pragma once
#include "Room.h"

// INHERITANCE: DoubleRoom inherits Room
// POLYMORPHISM: calculatePrice() override
class DoubleRoom : public Room {
private:
    static constexpr int PRICE_PER_DAY = 1800;

public:
    DoubleRoom(int number, int floor)
        : Room(number, floor, RoomTypes::DOUBLE) {}

    double calculatePrice(int days) const override {
        return static_cast<double>(PRICE_PER_DAY) * days;
    }

    double getPricePerDay() const override {
        return PRICE_PER_DAY;
    }
};
