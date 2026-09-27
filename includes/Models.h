#ifndef MODELS_H
#define MODELS_H

#include <string>
#include <vector>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

struct UserInfo {
    std::string wholeName;
    std::string phoneNumber;
    std::string idNumber;
    std::string vehicleRegistration;
};

struct ParkingSpot {
    int id;
    std::string label;
    bool isOccupied;
    UserInfo occupant;
    std::chrono::system_clock::time_point checkInTime;
};

struct BillReceipt {
    int spotId;
    std::string spotLabel;
    UserInfo occupant;
    std::string checkInTimeStr;
    std::string checkOutTimeStr;
    double durationMinutes;
    double totalFeeKes;
    std::string receiptText;
};

#endif
