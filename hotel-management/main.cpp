#include <iostream>
#include <stdexcept>
#include <string>
#include "Hotel.h"
#include "Utils.h"

// ============================================================
// Helper: clear screen (FIX: works on Windows AND Linux/macOS)
// ============================================================
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    std::cout << "\033[2J\033[H";
#endif
}

// ============================================================
// Helper: pause until Enter
// FIX: every input is a whole line now, so ONE Enter is enough
// ============================================================
void pauseScreen() {
    util::readLine("\n  [Press ENTER to continue...]");
}

// ============================================================
// Main Menu display
// ============================================================
void showMenu(const std::string& hotelName) {
    clearScreen();
    std::cout << std::endl;
    std::cout << "  ============================================" << std::endl;
    std::cout << "         " << hotelName                         << std::endl;
    std::cout << "         HOTEL MANAGEMENT SYSTEM              " << std::endl;
    std::cout << "  ============================================" << std::endl;
    std::cout << "  1. Check Room Availability"                   << std::endl;
    std::cout << "  2. Book a Room"                               << std::endl;
    std::cout << "  3. Check-out Guest"                           << std::endl;
    std::cout << "  4. View All Rooms"                            << std::endl;
    std::cout << "  5. View All Bookings"                         << std::endl;
    std::cout << "  6. Search Booking by Guest Name"              << std::endl;
    std::cout << "  7. View Room Details"                         << std::endl;
    std::cout << "  8. Exit"                                      << std::endl;
    std::cout << "  ============================================" << std::endl;
}

// ============================================================
// MAIN — Entry Point
// ============================================================
int main() {
    Hotel hotel("Grand Palace Hotel");

    try {
        bool running = true;
        while (running) {
            showMenu(hotel.getHotelName());
            int choice = util::readInt("  Your choice: ", 1, 8);

            switch (choice) {
                case 1: hotel.checkAvailability(); pauseScreen(); break;
                case 2: hotel.bookRoom();          pauseScreen(); break;
                case 3: hotel.checkOut();          pauseScreen(); break;
                case 4: hotel.viewAllRooms();      pauseScreen(); break;
                case 5: hotel.viewAllBookings();   pauseScreen(); break;
                case 6: hotel.searchBooking();     pauseScreen(); break;
                case 7: hotel.viewRoomDetails();   pauseScreen(); break;
                case 8:
                    running = false;
                    std::cout << "\n  Thank you for using Hotel Management System!" << std::endl;
                    std::cout << "  Goodbye!" << std::endl;
                    break;
            }
        }
    } catch (const std::runtime_error&) {
        // input stream closed (Ctrl+D / Ctrl+Z) -> exit cleanly instead of looping forever
        std::cout << "\n  Input closed. Exiting." << std::endl;
    }

    return 0;
}
