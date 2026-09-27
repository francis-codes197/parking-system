#ifndef PARKING_SYSTEM_H
#define PARKING_SYSTEM_H

#include "Models.h"
#include <algorithm>
#include <cmath>

class BaseParkingSystem {
protected:
    std::vector<ParkingSpot> m_spots;

public:
    virtual ~BaseParkingSystem() = default;
    virtual void initialize(int totalSpots) = 0;
    virtual bool assignSpot(int spotId, const UserInfo& user, const std::chrono::system_clock::time_point& checkInTime) = 0;
    virtual bool vacateSpot(int spotId, const std::chrono::system_clock::time_point& checkOutTime, BillReceipt& receipt) = 0;
    virtual double calculateFee(double minutes) const = 0;
    virtual const std::vector<ParkingSpot>& getAllSpots() const = 0;
    virtual const ParkingSpot* getSpot(int spotId) const = 0;
    virtual int getTotalSpots() const = 0;
    virtual int getOccupiedCount() const = 0;
    virtual int getFreeCount() const = 0;
};

class ParkingSystem : public BaseParkingSystem {
public:
    ParkingSystem(int totalSpots = 20);
    virtual ~ParkingSystem() override = default;

    virtual void initialize(int totalSpots) override;
    virtual bool assignSpot(int spotId, const UserInfo& user, const std::chrono::system_clock::time_point& checkInTime) override;
    virtual bool vacateSpot(int spotId, const std::chrono::system_clock::time_point& checkOutTime, BillReceipt& receipt) override;
    virtual double calculateFee(double minutes) const override;
    virtual const std::vector<ParkingSpot>& getAllSpots() const override;
    virtual const ParkingSpot* getSpot(int spotId) const override;
    virtual int getTotalSpots() const override;
    virtual int getOccupiedCount() const override;
    virtual int getFreeCount() const override;

    static std::string formatTime(const std::chrono::system_clock::time_point& tp);
};

#endif
