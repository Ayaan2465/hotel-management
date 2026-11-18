#pragma once
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "Room.h"
#include "SingleRoom.h"
#include "DoubleRoom.h"
#include "Suite.h"
#include "Guest.h"
#include "Booking.h"
#include "Utils.h"

// ============================================================
// Hotel Class — Main controller
// STL: vector<unique_ptr<Room>> and vector<Booking>
// ============================================================
class Hotel {
private:
    std::string                        hotelName;
    std::vector<std::unique_ptr<Room>> rooms;     // POLYMORPHISM via Room*
    std::vector<Booking>               bookings;

    static constexpr int MAX_STAY_DAYS = 365;

    Room* findRoom(int roomNumber) {
        for (auto& r : rooms)
            if (r->getRoomNumber() == roomNumber) return r.get();
        return nullptr;
    }

    Booking* findBooking(int bookingId) {
        for (Booking& b : bookings)
            if (b.getBookingId() == bookingId && b.getIsActive()) return &b;
        return nullptr;
    }

    // FIX: one helper instead of the same loop copy-pasted 3 times.
    // The price is read from the room itself, not hardcoded in a string.
    void listAvailableOfType(const std::string& type) const {
        const Room* sample = nullptr;
        for (const auto& r : rooms)
            if (r->getRoomType() == type) { sample = r.get(); break; }
        if (!sample) return;

        std::cout << "\n  -- Available " << type << " (Rs."
                  << util::formatMoney(sample->getPricePerDay()) << "/day) --" << std::endl;
        bool found = false;
        for (const auto& r : rooms) {
            if (!r->getIsBooked() && r->getRoomType() == type) {
                std::cout << "    Room " << r->getRoomNumber() << std::endl;
                found = true;
            }
        }
        if (!found) std::cout << "    None available" << std::endl;
    }

public:
    Hotel(std::string name) : hotelName(std::move(name)) {
        // Floor 1: 5 Single Rooms (101-105)
        for (int i = 1; i <= 5; i++)
            rooms.push_back(std::make_unique<SingleRoom>(100 + i, 1));
        // Floor 2: 5 Double Rooms (201-205)
        for (int i = 1; i <= 5; i++)
            rooms.push_back(std::make_unique<DoubleRoom>(200 + i, 2));
        // Floor 3: 2 Suites (301-302)
        for (int i = 1; i <= 2; i++)
            rooms.push_back(std::make_unique<Suite>(300 + i, 3));
    }

    // FIX: Hotel owns rooms, so copying it makes no sense (it used to
    // double-free). unique_ptr already blocks copying; we say it explicitly.
    Hotel(const Hotel&)            = delete;
    Hotel& operator=(const Hotel&) = delete;
    // No manual destructor needed: unique_ptr frees the rooms.

    // ================================================================
    // 1. Check Room Availability
    // ================================================================
    void checkAvailability() {
        std::cout << std::endl;
        std::cout << "  ======================================" << std::endl;
        std::cout << "          AVAILABLE ROOMS               " << std::endl;
        std::cout << "  ======================================" << std::endl;

        bool anyAvailable = false;
        for (const auto& r : rooms) {
            if (!r->getIsBooked()) {
                std::cout << "  " << *r << std::endl;
                anyAvailable = true;
            }
        }
        if (!anyAvailable)
            std::cout << "  [!] No rooms available right now." << std::endl;

        std::cout << "  ======================================" << std::endl;
    }

    // ================================================================
    // 2. Book a Room
    // ================================================================
    void bookRoom() {
        std::cout << std::endl;
        std::cout << "  ======================================" << std::endl;
        std::cout << "            BOOK A ROOM                 " << std::endl;
        std::cout << "  ======================================" << std::endl;

        listAvailableOfType(RoomTypes::SINGLE);
        listAvailableOfType(RoomTypes::DOUBLE);
        listAvailableOfType(RoomTypes::SUITE);

        std::cout << std::endl;
        int roomNo = util::readInt("  Enter Room Number to book (0 to cancel): ", 0, 9999);
        if (roomNo == 0) return;

        Room* selectedRoom = findRoom(roomNo);
        if (!selectedRoom) {
            std::cout << "  [!] Room not found!" << std::endl;
            return;
        }
        if (selectedRoom->getIsBooked()) {
            std::cout << "  [!] Room " << roomNo << " is already booked!" << std::endl;
            return;
        }

        // ---- Guest details (validated: no empty name, 10-digit phone) ----
        std::cout << "\n  -- Guest Details --" << std::endl;
        std::string name    = util::readNonEmpty("  Name    : ");
        std::string phone   = util::readPhone   ("  Phone   : ");
        std::string address = util::readNonEmpty("  Address : ");

        // ---- Dates: days are now DERIVED from the dates ----
        std::cout << std::endl;
        util::Date in = util::readDate("  Check-in Date  (DD/MM/YYYY): ");
        util::Date out;
        int days = 0;
        while (true) {
            out = util::readDate("  Check-out Date (DD/MM/YYYY): ");
            long diff = util::dayNumber(out) - util::dayNumber(in);
            if (diff <= 0) {
                std::cout << "  [!] Check-out must be AFTER check-in." << std::endl;
            } else if (diff > MAX_STAY_DAYS) {
                std::cout << "  [!] Maximum stay is " << MAX_STAY_DAYS << " days." << std::endl;
            } else {
                days = static_cast<int>(diff);
                break;
            }
        }
        std::cout << "  Number of Days             : " << days << " (calculated)" << std::endl;

        Guest   g(name, phone, address);
        Booking b(g, selectedRoom, util::formatDate(in), util::formatDate(out), days);
        selectedRoom->book();
        bookings.push_back(b);

        std::cout << "\n  [+] Booking Confirmed!" << std::endl;
        std::cout << "  Booking ID : " << bookings.back().getBookingId() << std::endl;
        std::cout << "  Room       : " << roomNo << " (" << selectedRoom->getRoomType() << ")" << std::endl;
        std::cout << "  Guest      : " << name << std::endl;
        std::cout << "  Estimated  : Rs." << util::formatMoney(selectedRoom->calculatePrice(days)) << std::endl;
    }

    // ================================================================
    // 3. Check-out
    // ================================================================
    void checkOut() {
        std::cout << std::endl;
        std::cout << "  ======================================" << std::endl;
        std::cout << "              CHECK-OUT                 " << std::endl;
        std::cout << "  ======================================" << std::endl;

        int id = util::readInt("  Enter Booking ID: ", 1, 999999);

        Booking* b = findBooking(id);
        if (!b) {
            std::cout << "  [!] Booking not found or already checked out!" << std::endl;
            return;
        }

        double service = util::readMoney("  Enter Room Service charges (0 if none): Rs.");
        if (!b->setRoomService(service)) {   // extra safety, readMoney already blocks negatives
            std::cout << "  [!] Invalid room service amount!" << std::endl;
            return;
        }

        b->generateBill();

        b->getRoom()->vacate();
        b->deactivate();

        std::cout << "\n  [+] Check-out successful! Thank you for staying with us." << std::endl;
    }

    // ================================================================
    // 4. View All Rooms
    // ================================================================
    void viewAllRooms() {
        std::cout << std::endl;
        std::cout << "  ======================================" << std::endl;
        std::cout << "            ALL ROOMS STATUS            " << std::endl;
        std::cout << "  ======================================" << std::endl;

        const char* titles[] = {"Floor 1: Single Rooms", "Floor 2: Double Rooms", "Floor 3: Suites"};
        for (int floor = 1; floor <= 3; floor++) {
            std::cout << "\n  -- " << titles[floor - 1] << " --" << std::endl;
            for (const auto& r : rooms)
                if (r->getFloor() == floor)
                    std::cout << "  " << *r << std::endl;
        }

        std::cout << "  ======================================" << std::endl;

        int booked = 0, available = 0;
        for (const auto& r : rooms) {
            if (r->getIsBooked()) booked++;
            else available++;
        }
        std::cout << "  Total: " << rooms.size()
                  << " | Booked: " << booked
                  << " | Available: " << available << std::endl;
    }

    // ================================================================
    // 5. View All Bookings
    // ================================================================
    void viewAllBookings() {
        std::cout << std::endl;
        std::cout << "  ======================================" << std::endl;
        std::cout << "           ALL BOOKINGS                 " << std::endl;
        std::cout << "  ======================================" << std::endl;

        if (bookings.empty()) {
            std::cout << "  No bookings found." << std::endl;
            std::cout << "  ======================================" << std::endl;
            return;
        }

        std::cout << "\n  -- Active Bookings --" << std::endl;
        bool anyActive = false;
        for (const Booking& b : bookings) {
            if (b.getIsActive()) { b.displaySummary(); anyActive = true; }
        }
        if (!anyActive) std::cout << "  None" << std::endl;

        std::cout << "\n  -- Past Bookings --" << std::endl;
        bool anyPast = false;
        for (const Booking& b : bookings) {
            if (!b.getIsActive()) { b.displaySummary(); anyPast = true; }
        }
        if (!anyPast) std::cout << "  None" << std::endl;

        std::cout << "  ======================================" << std::endl;
        std::cout << "  Total Bookings: " << bookings.size() << std::endl;
    }

    // ================================================================
    // 6. Search Booking by Guest Name  (FIX: case-insensitive, partial match)
    // ================================================================
    void searchBooking() {
        std::cout << std::endl;
        std::cout << "  ======================================" << std::endl;
        std::cout << "            SEARCH BOOKING              " << std::endl;
        std::cout << "  ======================================" << std::endl;

        std::string query = util::toLower(util::readNonEmpty("  Enter Guest Name: "));

        bool found = false;
        for (const Booking& b : bookings) {
            if (util::toLower(b.getGuest().getName()).find(query) != std::string::npos) {
                b.displaySummary();
                found = true;
            }
        }
        if (!found)
            std::cout << "  [!] No booking found for '" << query << "'" << std::endl;
    }

    // ================================================================
    // 7. View Room Details
    // ================================================================
    void viewRoomDetails() {
        std::cout << std::endl;
        int no = util::readInt("  Enter Room Number: ", 0, 9999);
        Room* r = findRoom(no);
        if (!r) { std::cout << "  [!] Room not found!" << std::endl; return; }
        std::cout << std::endl;
        r->displayInfo();

        if (r->getIsBooked()) {
            for (const Booking& b : bookings) {
                if (b.getRoom()->getRoomNumber() == no && b.getIsActive()) {
                    std::cout << "  Current Guest : " << b.getGuest().getName() << std::endl;
                    std::cout << "  Check-in      : " << b.getCheckIn()         << std::endl;
                    std::cout << "  Check-out     : " << b.getCheckOut()        << std::endl;
                    break;
                }
            }
        }
    }

    std::string getHotelName() const { return hotelName; }
};
