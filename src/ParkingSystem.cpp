#include "ParkingSystem.h"

ParkingSystem::ParkingSystem(int totalSpots) {
    initialize(totalSpots);
}

void ParkingSystem::initialize(int totalSpots) {
    m_spots.clear();
    for (int i = 0; i < totalSpots; ++i) {
        char row = 'A' + (i / 5);
        int col = (i % 5) + 1;
        std::string label = "";
        label += row;
        label += std::to_string(col);

        ParkingSpot spot;
        spot.id = i + 1;
        spot.label = label;
        spot.isOccupied = false;
        spot.occupant = UserInfo{};
        spot.checkInTime = std::chrono::system_clock::now();
        m_spots.push_back(spot);
    }
}

bool ParkingSystem::assignSpot(int spotId, const UserInfo& user, const std::chrono::system_clock::time_point& checkInTime) {
    for (auto& spot : m_spots) {
        if (spot.id == spotId && !spot.isOccupied) {
            spot.isOccupied = true;
            spot.occupant = user;
            spot.checkInTime = checkInTime;
            return true;
        }
    }
    return false;
}

double ParkingSystem::calculateFee(double minutes) const {
    if (minutes <= 30.0) {
        return 0.0;
    } else if (minutes <= 120.0) {
        return 50.0;
    } else if (minutes <= 240.0) {
        return 100.0;
    } else if (minutes <= 360.0) {
        return 300.0;
    } else {
        return 500.0;
    }
}

bool ParkingSystem::vacateSpot(int spotId, const std::chrono::system_clock::time_point& checkOutTime, BillReceipt& receipt) {
    for (auto& spot : m_spots) {
        if (spot.id == spotId && spot.isOccupied) {
            auto diffSeconds = std::chrono::duration_cast<std::chrono::seconds>(checkOutTime - spot.checkInTime).count();
            double minutes = static_cast<double>(diffSeconds) / 60.0;
            if (minutes < 0.0) {
                minutes = 0.0;
            }

            double fee = calculateFee(minutes);

            receipt.spotId = spot.id;
            receipt.spotLabel = spot.label;
            receipt.occupant = spot.occupant;
            receipt.checkInTimeStr = formatTime(spot.checkInTime);
            receipt.checkOutTimeStr = formatTime(checkOutTime);
            receipt.durationMinutes = minutes;
            receipt.totalFeeKes = fee;

            std::string tierDesc;
            if (minutes <= 30.0) {
                tierDesc = "Upto 30 minutes (Free)";
            } else if (minutes <= 120.0) {
                tierDesc = "Upto 2 hours (KES 50)";
            } else if (minutes <= 240.0) {
                tierDesc = "Upto 4 hours (KES 100)";
            } else if (minutes <= 360.0) {
                tierDesc = "Upto 6 hours (KES 300)";
            } else {
                tierDesc = "Over 6 hours (KES 500)";
            }

            std::ostringstream ss;
            ss << "========================================\n";
            ss << "          PARKING BILL RECEIPT          \n";
            ss << "========================================\n";
            ss << "Spot:                 " << receipt.spotLabel << "\n";
            ss << "Driver Name:          " << receipt.occupant.wholeName << "\n";
            ss << "Phone Number:         " << receipt.occupant.phoneNumber << "\n";
            ss << "ID Number:            " << receipt.occupant.idNumber << "\n";
            ss << "Vehicle Registration: " << receipt.occupant.vehicleRegistration << "\n";
            ss << "----------------------------------------\n";
            ss << "Check-In Time:        " << receipt.checkInTimeStr << "\n";
            ss << "Check-Out Time:       " << receipt.checkOutTimeStr << "\n";
            ss << "Duration:             " << std::fixed << std::setprecision(1) << receipt.durationMinutes << " minutes\n";
            ss << "Rate Applied:         " << tierDesc << "\n";
            ss << "----------------------------------------\n";
            ss << "TOTAL DUE:            KES " << std::fixed << std::setprecision(2) << receipt.totalFeeKes << "\n";
            ss << "========================================\n";

            receipt.receiptText = ss.str();

            std::cout << receipt.receiptText << std::endl;

            spot.isOccupied = false;
            spot.occupant = UserInfo{};
            return true;
        }
    }
    return false;
}

const std::vector<ParkingSpot>& ParkingSystem::getAllSpots() const {
    return m_spots;
}

const ParkingSpot* ParkingSystem::getSpot(int spotId) const {
    for (const auto& spot : m_spots) {
        if (spot.id == spotId) {
            return &spot;
        }
    }
    return nullptr;
}

int ParkingSystem::getTotalSpots() const {
    return static_cast<int>(m_spots.size());
}

int ParkingSystem::getOccupiedCount() const {
    int count = 0;
    for (const auto& spot : m_spots) {
        if (spot.isOccupied) {
            count++;
        }
    }
    return count;
}

int ParkingSystem::getFreeCount() const {
    return getTotalSpots() - getOccupiedCount();
}

std::string ParkingSystem::formatTime(const std::chrono::system_clock::time_point& tp) {
    std::time_t t = std::chrono::system_clock::to_time_t(tp);
    std::tm tmBuffer;
#if defined(_WIN32)
    localtime_s(&tmBuffer, &t);
#else
    localtime_r(&t, &tmBuffer);
#endif
    std::ostringstream ss;
    ss << std::put_time(&tmBuffer, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
