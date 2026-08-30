#include "../header/Schedule.h"
#include "../header/Bicycle.h"
#include "../header/utils.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <chrono>
#include <cstring>

using namespace std;

//====================
//display layer
//====================
void scheduleMenu(vector<Schedule>& schedules, vector<Bicycle>& bikes, vector<Rental>& rentals) {
    int choice;
    do {
        clearScreen();
        cout << "\n=========================================" << endl;
        cout << "        MEMBER MANAGEMENT SYSTEM         " << endl;
        cout << "=========================================" << endl;
        cout << "1. Display Schedule" << endl;
        cout << "2. Change Time Slot" << endl;
        cout << "3. Delete Time Slot" << endl;
        cout << "4. Reset Schedule" << endl;
        cout << "5. Add Schedule" << endl;
        cout << "0. Back to Main Menu" << endl;

        cout << "Enter choice : ";
        cin >> choice;

        switch (choice) {
            case 1:
                clearScreen();
                printSchedule(schedules, bikes);
                break;
            case 2:
                clearScreen();
                printSchedule(schedules, bikes);
                changeTimeSlot(schedules, bikes);
                break;
            case 3:
                clearScreen();
                printSchedule(schedules, bikes);
                deleteTimeSlot(schedules, bikes);
                break;
            case 4:
                clearScreen();
                resetSchedule(schedules, rentals);
                break;
            case 5:
                clearScreen();
                addSchedule(schedules, bikes);
                break;
            case 0:
                cout << "\nReturning to Main Menu...\n";
                break;
            default:
                cout << "Invalid selection!\n";
        }
    } while (choice != 0);
}

void printSchedule(const vector<Schedule>& schedules, const vector<Bicycle>& bikes) {
    cout << "                                          --Schedule--" << endl;
    cout << "             (1)     (2)     (3)     (4)     (5)     (6)     (7)     (8)     (9)     (10)    (11)    (12)  " << endl;
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;
    cout << "| Bike ID | 9.am- | 10.am-| 11.am-| 12.pm-| 1.pm- | 2.pm- | 3.pm- | 4.pm- | 5.pm- | 6.pm- | 7.pm- | 8.pm- |" << endl;
    cout << "|         | 10.am | 11.am | 12.pm | 1.pm  | 2.pm  | 3.pm  | 4.pm  | 5.pm  | 6.pm  | 7.pm  | 8.pm  | 9.pm  |" << endl;
    cout << "-----------------------------------------------------------------------------------------------------------" << endl;

        for (const auto&[bike_id, time_slot_status] : schedules) {
            cout << "| " << left << setw(8) << bike_id << "|";
            for (const char tss : time_slot_status) {
                cout << "   " << tss << "   |" ;
            }
            cout << "\n-----------------------------------------------------------------------------------------------------------" << endl;
        }

    cout << "X = Unavailable" << endl;
};

//====================
//logic later
//====================
DateYMD getToday() {
    DateYMD today{};

    time_t t = time(nullptr);
    tm* now = localtime(&t);

    today.year = now->tm_year + 1900;
    today.month = now->tm_mon + 1;
    today.day = now->tm_mday;

    return today;
};

//book time slot
int changeTimeSlot(std::vector<Schedule>& schedules, const std::vector<Bicycle>& bikes, int bike_id) {
    int id,timeSlot;
    bool found = false, isbooked = false, chs2 = false;

    do {
        chs2 = false;

        do {
            isbooked = false;

            //check valid bike id
            do {
                found = false;

                cout << "\nEnter bike ID: ";

                //check valid input
                if (!(cin >> id)) {
                    cout << "Invalid input! Please enter a number." << endl;
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }

                for (const auto& bike : bikes) {
                    if (bike.id == id) {found = true;}
                }

                if (!found) {cout << "Invalid ID! Please enter again." << endl;}
            }while (!found);

            //check valid time slot
            do {
                found = false;

                cout << "Enter time slot(1-12): ";
                if (!(cin >> timeSlot)) {
                    cout << "Invalid input! Please enter a number." << endl;
                    cin.clear();
                    cin.ignore(1000, '\n');
                    continue;
                }

                if (timeSlot < 0 || timeSlot > 12) {
                    cout << "Invalid timeSlot! Please enter again." << endl;
                }else {
                    found = true;
                }

            }while (!found);

            if (checkTimeSlot(schedules, id, timeSlot)) {
                for (auto& schedule : schedules) {
                    if (schedule.bike_id == id) {
                        schedule.time_slot_status[timeSlot - 1] = 'X';
                        isbooked = true;

                        cout << "Successfully booked time." << endl;

                        char option;
                        cout << "book another time slot?(y/n): ";
                        cin >> option;
                        if (option == 'y') {chs2 = true;}
                    }
                }
            }else{ cout << "TimeSlot has been booked! Please enter again." << endl; }
        }while (!isbooked);
    }while (chs2);

    return id;
};

//check valid time slot
bool checkTimeSlot(const std::vector<Schedule>& schedules, int targetID, int time_slot) {
    for (const auto& schedule : schedules) {
        if (schedule.bike_id == targetID) {
            if (schedule.time_slot_status[time_slot - 1] == 'X') {
                return false;
            }
        }
    }

    return true;
};

//delete booked time slot
void deleteTimeSlot(std::vector<Schedule>& schedules, const vector<Bicycle>& bikes) {
    int id,timeSlot;
    bool found = false;

    do {
        found = false;
        cout << "\nEnter bike ID: ";

        if (!(cin >> id)) {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        for (const auto& bike : bikes) {
            if (bike.id == id) {found = true;}
        }

        if (!found) {cout << "Invalid ID! Please enter again." << endl;}
    }while (!found);

    do {
        found = false;
        cout << "Enter time slot(1-12): ";

        if (!(cin >> timeSlot)) {
            cout << "Invalid input! Please enter a number." << endl;
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }

        if (timeSlot < 0 || timeSlot > 13) {cout << "Invalid timeSlot! Please enter again." << endl;}
        else{found = true;}
    }while (!found);

    if (!checkTimeSlot(schedules, id, timeSlot)) {
        for (auto& schedule : schedules) {
            if (schedule.bike_id == id) {
                schedule.time_slot_status[timeSlot - 1] = ' ';

                cout << "Successfully deleted time." << endl;
            }
        }
    }else {
        cout << "Time slot is no booked! Please enter again." << endl;
    }

};

//reset schedule to emtpy
void resetSchedule(vector<Schedule>& schedules, const vector<Rental>& rentals) {
    bool found = false, validInput = false;
    string choice;

    for (const auto& rental : rentals) {
        if (isToday(rental.date)) {
            found = true;
        }
    }

    if (!found) {
        do {
            validInput = false;
            cout << "Are you sure want to reset the schedules?(y/n): ";
            if (!(cin >> choice)) {
                cout << "Invalid input! Please enter a number." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }

            if (choice == "y" || choice == "Y") {
                for (auto& schedule : schedules) {
                    for (int i = 0;i < 12;i++) {
                        schedule.time_slot_status[i] = ' ';
                    }
                }
                cout << "Reset schedule success." << endl;

                validInput = true;
            }
            else if (choice == "n" || choice == "N") {validInput = true;}
            else {cout << "Invalid choice! Please enter again." << endl;}
        }while (!validInput);
    }else {
        do {
            validInput = false;
            cout << "Today's record are exist, are you sure want to reset the schedules?(y/n): ";
            if (!(cin >> choice)) {
                cout << "Invalid input! Please enter a number." << endl;
                cin.clear();
                cin.ignore(1000, '\n');
                continue;
            }

            if (choice == "y" || choice == "Y") {
                for (auto& schedule : schedules) {
                    for (int i = 0;i < 12;i++) {
                        schedule.time_slot_status[i] = ' ';
                    }
                }
                cout << "Reset schedule success." << endl;
                validInput = true;
            }
            else if (choice == "n" || choice == "N") {validInput = true;}
            else {cout << "Invalid choice! Please enter again." << endl;}
        }while (!validInput);
    }
};

//add new bike schedule
void addSchedule(vector<Schedule>& schedules, const vector<Bicycle>& bikes) {
    bool found = false;
    int count = 0;

    for (auto& bike : bikes) {
        found = false;
        for (auto& s : schedules) {
            if (s.bike_id == bike.id) {
                found = true;
            }
        }
        if (!found) {
            Schedule schedule = {bike.id};
            schedules.push_back(schedule);
            count++;
        }
    }

    if (count == 0) {
        cout << "No new bike add." << endl;
    }
    else {
        cout << "Added " << count << " new bike!" << endl;
    }
};

//check the date is today
bool isToday(const DateYMD& date) {
    auto [year, month, day] = getToday();
    bool found = false;

    if (date.year == year && date.month == month && date.day == day) {
        found = true;
    }

    return found;
};

//===============================
//file operations layer
//===============================
void saveSchedule(const std::vector<Schedule>& schedules) {
    ofstream outFile("../data/schedule.csv");

    if (!outFile) {
        cerr << "Can't open file!\n";
        return;
    }

    outFile << "Bike_ID|time_slot_status\n";
    for (const auto& schedule : schedules) {
        outFile << schedule.bike_id << "|";
        for (const auto& time_slot : schedule.time_slot_status) {
            outFile << time_slot << ",";
        }
        outFile << "\n";
    }
    outFile.close();
};

void loadSchedule(vector<Schedule>& schedules) {
    ifstream inFile("../data/schedule.csv");

    if (!inFile) {
        cerr << "Can't open file!\n";
        return;
    }

    string line;
    getline(inFile, line);
    while (getline(inFile, line)) {
        stringstream ss(line);
        string idStr, timeSlotStr;
        vector<char> arr;
        Schedule schedule;

        getline(ss, idStr, '|');
        getline(ss, timeSlotStr, '|');
        schedule.bike_id = stoi(idStr);

        stringstream ss2(timeSlotStr);
        string ts;
        int count = 0;

        while (getline(ss2, ts, ',')) {
            if (!ts.empty()) {
                schedule.time_slot_status[0 + count] = ts[0];
                count++;
            }
        }

    schedules.push_back(schedule);
    }
};
