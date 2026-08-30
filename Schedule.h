#pragma once
#include <vector>

struct Schedule {
    int bike_id = -1;
    char time_slot_status[12] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '}; //9am - 9pm
};

struct DateYMD {
    int year;
    int month;
    int day;
};

struct Bicycle;
struct Rental;

void scheduleMenu(std::vector<Schedule>& schedules, std::vector<Bicycle>& bikes, std::vector<Rental>& rentals);
void printSchedule(const std::vector<Schedule>& schedules, const std::vector<Bicycle>& bikes);
DateYMD getToday();
int changeTimeSlot(std::vector<Schedule>& schedules, const std::vector<Bicycle>& bikes, int bike_id = -1);
bool checkTimeSlot(const std::vector<Schedule>& schedules, int targetID, int time_slot);
void deleteTimeSlot(std::vector<Schedule>& schedules, const std::vector<Bicycle>& bikes);
void resetSchedule(std::vector<Schedule>& schedules, const std::vector<Rental>& rentals);
void addSchedule(std::vector<Schedule>& schedules, const std::vector<Bicycle>& bikes);
bool isToday(const DateYMD& date);
void saveSchedule(const std::vector<Schedule>& schedules);
void loadSchedule(std::vector<Schedule>& schedules);