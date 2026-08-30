#pragma once
#include <string>
#include <vector>
#include "Schedule.h"

struct Bicycle {
    int    id;
    std::string type;
    double hourlyRate;
    bool   isAvailable;
    std::string rentedBy;
};

struct Rental {
    std::string rentBy;
    int bikeId;
    DateYMD date;
    int hours;
    bool isReturn;
};

void saveFleetToFile(const std::vector<Bicycle>& fleet);
void loadFleetFromFile(std::vector<Bicycle>& fleet);
void saveBookingToFile(int bikeId, const std::string& bikeType, int hours, double totalCost);
void saveRentalRecord(const std::vector<Rental>& rentals);
void loadRentalRecord(std::vector<Rental>& rentals);
void displayAvailableBikes(const std::vector<Bicycle>& fleet);
void viewAllBikes(const std::vector<Bicycle>& fleet);
void rentBike(std::vector<Bicycle>& fleet, std::vector<Rental>& rentals, std::vector<Schedule>& schedules);
void returnBike(std::vector<Bicycle>& fleet, std::vector<Rental>& rentals);
void addBike(std::vector<Bicycle>& fleet);
void removeBike(std::vector<Bicycle>& fleet);
void bikeRentalMenu(std::vector<Bicycle>& fleet, std::vector<Rental>& rentals, std::vector<Schedule>& schedules);