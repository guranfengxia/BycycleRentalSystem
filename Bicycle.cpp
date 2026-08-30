#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <ctime>
#include <filesystem>
#include <cctype>
#include "../header/Bicycle.h"
#include "../header/utils.h"

using namespace std;

bool isValidCustomerId(const string& customerId) {
    // Must be exactly 4 characters
    if (customerId.length() != 4) {
        return false;
    }

    // First character must be uppercase C
    if (customerId[0] != 'C') {
        return false;
    }

    // Remaining 3 characters must be digits
    for (int i = 1; i < 4; i++) {
        if (!isdigit(customerId[i])) {
            return false;
        }
    }

    return true;
}

void saveFleetToFile(const vector<Bicycle>& fleet) {
    ofstream outFile("../data/bikes.txt");
    if (!outFile) {
        cerr << "Error: Could not save fleet state!" << endl;
        return;
    }

    // Format: id|type|hourlyRate|isAvailable|rentedBy
    for (const auto& bike : fleet) {
        outFile << bike.id        << "|"
                << bike.type      << "|"
                << bike.hourlyRate << "|"
                << bike.isAvailable << "|"
                << (bike.rentedBy.empty() ? "None" : bike.rentedBy) << "\n";
    }
    outFile.close();
}

void loadFleetFromFile(vector<Bicycle>& fleet) {
    ifstream inFile("../data/bikes.txt");
    if (!inFile) {
        // Default fleet on first launch — also write to disk so file exists next time
        fleet = {
            {101, "City Bike",     5.00,  true, "None"},
            {102, "Mountain Bike", 8.50,  true, "None"},
            {103, "Electric Bike", 15.00, true, "None"},
            {104, "Tandem Bike",   12.00, true, "None"}
        };
        saveFleetToFile(fleet);   // ← FIX: create bikes.txt on first run
        return;
    }

    fleet.clear();
    string line;
    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        string idStr, type, rateStr, availStr, rentedBy;

        getline(ss, idStr,    '|');
        getline(ss, type,     '|');
        getline(ss, rateStr,  '|');
        getline(ss, availStr, '|');
        getline(ss, rentedBy, '|');

        Bicycle bike;
        bike.id          = stoi(idStr);
        bike.type        = type;
        bike.hourlyRate  = stod(rateStr);
        bike.isAvailable = (availStr == "1");
        bike.rentedBy    = rentedBy;
        fleet.push_back(bike);
    }
    inFile.close();
}

// Log booking record to data/bookings.txt
void saveBookingToFile(int bikeId, const string& bikeType, int hours, double totalCost) {
    filesystem::create_directories("data");
    ofstream outFile("data/bookings.txt", ios::app);
    if (!outFile) return;

    time_t now = time(0);
    char timeBuffer[80];
    struct tm timeInfo;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&timeInfo, &now);
#else
    localtime_r(&now, &timeInfo);
#endif
    strftime(timeBuffer, sizeof(timeBuffer), "%Y-%m-%d %H:%M:%S", &timeInfo);

    outFile << bikeId      << "|"
            << bikeType    << "|"
            << hours       << "|"
            << fixed << setprecision(2) << totalCost << "|"
            << timeBuffer  << "\n";
    outFile.close();
}

// Log rental/return record to data/rental_records.txt
void saveRentalRecord(const vector<Rental>& rentals) {
    ofstream outFile("../data/rental_records.txt");

    if (!outFile) {
        cerr << "Error: Could not save records state!" << endl;
        return;
    }

    for (const auto& r : rentals) {
        outFile << r.rentBy << "|" << r.bikeId << "|" << r.date.year << "," << r.date.month << "," << r.date.day << "|" << r.hours << "|" << r.isReturn << "|\n";
    }
    outFile.close();
};

void loadRentalRecord(vector<Rental>& rentals) {
    ifstream inFile("../data/booking_records.txt");
    if (!inFile) {
        cerr << "Error: Could not load records state!" << endl;
        return;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        string rentBy, bikeIdStr, date, hourStr, isReturnStr;
        getline(ss, rentBy, '|');
        getline(ss, bikeIdStr, '|');
        getline(ss, date, '|');
        getline(ss, hourStr, '|');
        getline(ss, isReturnStr, '|');

        Rental rental;
        rental.rentBy = rentBy;
        rental.bikeId = stoi(bikeIdStr);
        rental.hours = stoi(hourStr);
        rental.isReturn = isReturnStr == "true";

        stringstream ss2(date);
        string ts;
        vector<int> temp_date;

        while (getline(ss2, ts, ',')) {
            if (!ts.empty()) {
                temp_date.push_back(stoi(ts));
            }
        }

        rental.date.year = temp_date[0];
        rental.date.month = temp_date[1];
        rental.date.day = temp_date[2];

        rentals.push_back(rental);
    }
    inFile.close();
};
// ─────────────────────────────────────────
//  DISPLAY
// ─────────────────────────────────────────

void displayAvailableBikes(const vector<Bicycle>& fleet) {
    cout << "\n--- Bicycle Fleet Status ---\n";
    cout << left << setw(8)  << "ID"
                 << setw(18) << "Type"
                 << setw(12) << "Rate/Hour"
                 << "Status" << "\n";
    cout << "--------------------------------------------\n";

    for (const auto& bike : fleet) {
        cout << left << setw(8)  << bike.id
                     << setw(18) << bike.type
                     << "$" << setw(11) << fixed << setprecision(2) << bike.hourlyRate
                     << (bike.isAvailable ? "Available" : "Rented Out") << "\n";
    }
}

// ─────────────────────────────────────────
//  OPERATIONS  (each is its own function)
// ─────────────────────────────────────────

// Option 1 – view all bikes
void viewAllBikes(const vector<Bicycle>& fleet) {
    displayAvailableBikes(fleet);
}

// Option 2 – rent a bike
void rentBike(vector<Bicycle>& fleet, vector<Rental>& rentals, vector<Schedule>& schedules) {
    displayAvailableBikes(fleet);

    string customerId;
    int bikeId, hours;
    Rental rental;

    // Validate Customer ID format
    do {
        cout << "\nEnter Customer ID (e.g. C100): ";
        cin >> customerId;

        if (!isValidCustomerId(customerId)) {
            cout << "Invalid Customer ID format!\n";
            cout << "Please use format C100, C101, C102...\n";
        }

    } while (!isValidCustomerId(customerId));
    rental.rentBy = customerId;

    printSchedule(schedules,fleet);
    bikeId = changeTimeSlot(schedules, fleet);
    rental.bikeId = bikeId;

    bool found = false;

    for (auto& bike : fleet) {
        if (bike.id == bikeId) {
            found = true;

            if (!bike.isAvailable) {
                cout << "Sorry, this bike is currently rented out.\n";
            }
            else {
                cout << "Enter rental duration (hours): ";
                cin >> hours;
                rental.hours = hours;

                rental.date = getToday();

                double total = bike.hourlyRate * hours;

                // Store current customer renting this bike
                bike.isAvailable = false;
                bike.rentedBy = customerId;

                cout << "\nRental Confirmed!\n";
                cout << "Customer ID : " << customerId << "\n";
                cout << "Bike        : " << bike.type << "\n";
                cout << "Hours       : " << hours << "\n";
                cout << "Total       : $"
                     << fixed << setprecision(2)
                     << total << "\n";

                rental.isReturn = false;
                rentals.push_back(rental);

                // Save booking information
                saveBookingToFile(
                    bike.id,
                    bike.type,
                    hours,
                    total
                );

                // Save rental record
                // false = not returned
                saveRentalRecord(rentals);

                // Save current bicycle status
                saveFleetToFile(fleet);
            }

            break;
        }
    }

    if (!found) {
        cout << "Invalid Bike ID!\n";
    }
}

// Option 3 – return a bike
void returnBike(vector<Bicycle>& fleet, vector<Rental>& rentals) {
    int bikeId;

    cout << "\nEnter Bike ID to return: ";
    cin >> bikeId;

    bool found = false;

    for (auto& bike : fleet) {
        if (bike.id == bikeId) {
            found = true;

            if (bike.isAvailable) {
                cout << "This bike was not rented out.\n";
            }
            else {
                // Get the customer ID before clearing rentedBy
                string customerId = bike.rentedBy;

                bike.isAvailable = true;
                bike.rentedBy = "None";

                for (auto& rental : rentals) {
                    if (rental.bikeId == bikeId && rental.rentBy == customerId) {
                        rental.isReturn = true;
                    }
                }
                // Save return record
                saveRentalRecord(rentals);

                saveFleetToFile(fleet);

                cout << "Bike ID "
                     << bikeId
                     << " successfully returned!\n";
            }

            break;
        }
    }

    if (!found) {
        cout << "Invalid Bike ID!\n";
    }
}

// Option 4 – add a new bike
void addBike(vector<Bicycle>& fleet) {
    Bicycle newBike;

    // Auto-generate next ID based on highest existing ID
    int maxId = 100;
    for (const auto& b : fleet) {
        if (b.id > maxId) maxId = b.id;
    }
    newBike.id = maxId + 1;

    cin.ignore();
    cout << "\n--- Add New Bicycle ---\n";
    cout << "Enter bike type: ";
    getline(cin, newBike.type);

    cout << "Enter hourly rate: $";
    cin  >> newBike.hourlyRate;

    newBike.isAvailable = true;

    fleet.push_back(newBike);
    saveFleetToFile(fleet);

    cout << "Bicycle added with ID: " << newBike.id << "\n";
}

// Option 5 – remove a bike
void removeBike(vector<Bicycle>& fleet) {
    int bikeId;
    cout << "\nEnter Bike ID to remove: ";
    cin  >> bikeId;

    for (auto it = fleet.begin(); it != fleet.end(); ++it) {
        if (it->id == bikeId) {
            if (!it->isAvailable) {
                cout << "Cannot remove a bike that is currently rented out!\n";
                return;
            }
            fleet.erase(it);
            saveFleetToFile(fleet);
            cout << "Bike ID " << bikeId << " removed successfully.\n";
            return;
        }
    }
    cout << "Invalid Bike ID!\n";
}

// ─────────────────────────────────────────
//  MENU
// ─────────────────────────────────────────

void bikeRentalMenu(vector<Bicycle>& fleet, vector<Rental>& rentals, vector<Schedule>& schedules) {
    loadFleetFromFile(fleet);

    int choice;
    do {
        clearScreen();
        cout << "\n=========================================\n";
        cout << "      BICYCLE RENTAL MANAGEMENT          \n";
        cout << "=========================================\n";
        cout << "1. View All Bikes & Status\n";
        cout << "2. Rent a Bike\n";
        cout << "3. Return a Bike\n";
        cout << "4. Add New Bike\n";
        cout << "5. Remove a Bike\n";
        cout << "6. Back to Main Menu\n";

        cout << "Enter choice (1-6): ";
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input!\n";
            continue;
        }

        switch (choice) {
            case 1:
                clearScreen();
                viewAllBikes(fleet);
                break;
            case 2:
                clearScreen();
                rentBike(fleet, rentals, schedules);
                break;
            case 3:
                clearScreen();
                returnBike(fleet, rentals);
                break;
            case 4:
                clearScreen();
                addBike(fleet);
                break;
            case 5:
                clearScreen();
                removeBike(fleet);
                break;
            case 6:
                saveFleetToFile(fleet);
                cout << "\nReturning to Main Menu...\n";
                break;
            default:
                cout << "Invalid selection!\n";
        }
    } while (choice != 6);
}