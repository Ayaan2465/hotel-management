#pragma once
// ============================================================
// Utils.h — shared helpers (NEW FILE)
//  * safe input functions: every read is a whole line, so cin
//    never ends up in a broken state (fixes bad-input bugs)
//  * date parsing / day counting (days are now derived from dates)
//  * money formatting (no more 3.5e+06)
// ============================================================
#include <algorithm>
#include <cctype>
#include <cmath>
#include <cstdio>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

namespace util {

// ---------- money ----------
inline std::string formatMoney(double amount) {
    std::ostringstream os;
    os << std::fixed << std::setprecision(2) << amount;
    return os.str();
}

// ---------- strings ----------
inline std::string trim(const std::string& s) {
    size_t b = s.find_first_not_of(" \t\r\n");
    if (b == std::string::npos) return "";
    size_t e = s.find_last_not_of(" \t\r\n");
    return s.substr(b, e - b + 1);
}

inline std::string toLower(std::string s) {
    std::transform(s.begin(), s.end(), s.begin(),
                   [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return s;
}

// ---------- safe input ----------
// Reads one full line. Throws if input is closed (Ctrl+D / Ctrl+Z / piped EOF)
// so the program never spins forever in an input loop.
inline std::string readLine(const std::string& prompt) {
    std::string s;
    std::cout << prompt;
    if (!std::getline(std::cin, s)) throw std::runtime_error("Input closed");
    return s;
}

inline int readInt(const std::string& prompt, int lo, int hi) {
    while (true) {
        std::string s = trim(readLine(prompt));
        try {
            size_t pos = 0;
            long v = std::stol(s, &pos);
            if (pos == s.size() && v >= lo && v <= hi) return static_cast<int>(v);
        } catch (...) { /* not a number / out of range -> fall through */ }
        std::cout << "  [!] Enter a whole number between " << lo << " and " << hi << "." << std::endl;
    }
}

// Money amount: must be a finite number between 0 and maxVal (no negatives).
inline double readMoney(const std::string& prompt, double maxVal = 1000000.0) {
    while (true) {
        std::string s = trim(readLine(prompt));
        try {
            size_t pos = 0;
            double v = std::stod(s, &pos);
            if (pos == s.size() && std::isfinite(v) && v >= 0 && v <= maxVal) return v;
        } catch (...) {}
        std::cout << "  [!] Enter an amount between 0 and " << formatMoney(maxVal) << "." << std::endl;
    }
}

inline std::string readNonEmpty(const std::string& prompt) {
    while (true) {
        std::string s = trim(readLine(prompt));
        if (!s.empty()) return s;
        std::cout << "  [!] This field cannot be empty." << std::endl;
    }
}

// Phone: exactly 10 digits (change PHONE_LEN if you need another format)
inline std::string readPhone(const std::string& prompt) {
    const size_t PHONE_LEN = 10;
    while (true) {
        std::string s = trim(readLine(prompt));
        bool ok = s.size() == PHONE_LEN &&
                  std::all_of(s.begin(), s.end(), [](unsigned char c) { return std::isdigit(c); });
        if (ok) return s;
        std::cout << "  [!] Phone must be exactly " << PHONE_LEN << " digits." << std::endl;
    }
}

// ---------- dates ----------
struct Date { int day, month, year; };

inline bool isLeap(int y) { return (y % 4 == 0 && y % 100 != 0) || y % 400 == 0; }

inline int daysInMonth(int m, int y) {
    static const int d[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return (m == 2 && isLeap(y)) ? 29 : d[m - 1];
}

// Parses strictly "DD/MM/YYYY" and checks that the date really exists.
inline bool parseDate(const std::string& s, Date& out) {
    int d = 0, m = 0, y = 0, n = 0;
    if (std::sscanf(s.c_str(), "%d/%d/%d%n", &d, &m, &y, &n) != 3) return false;
    if (n != static_cast<int>(s.size())) return false;
    if (y < 1900 || y > 2100 || m < 1 || m > 12) return false;
    if (d < 1 || d > daysInMonth(m, y)) return false;
    out = {d, m, y};
    return true;
}

// Days since 1970-01-01 (civil-calendar algorithm) -> subtract two to get nights.
inline long dayNumber(const Date& dt) {
    int y = dt.year - (dt.month <= 2 ? 1 : 0);
    long era = (y >= 0 ? y : y - 399) / 400;
    unsigned yoe = static_cast<unsigned>(y - era * 400);
    unsigned doy = (153 * (dt.month + (dt.month > 2 ? -3 : 9)) + 2) / 5 + dt.day - 1;
    unsigned doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + static_cast<long>(doe) - 719468;
}

inline std::string formatDate(const Date& dt) {
    char buf[16];
    std::snprintf(buf, sizeof buf, "%02d/%02d/%04d", dt.day, dt.month, dt.year);
    return buf;
}

inline Date readDate(const std::string& prompt) {
    while (true) {
        std::string s = trim(readLine(prompt));
        Date d;
        if (parseDate(s, d)) return d;
        std::cout << "  [!] Enter a valid date as DD/MM/YYYY (e.g. 05/03/2026)." << std::endl;
    }
}

}  // namespace util
