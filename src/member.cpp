#include "../header/member.h"
#include "../header/utils.h"
#include <limits>
#include <fstream>
#include <sstream>
using namespace std;

//global variable for assign memberID
int nextMemberID = 1001;

int getValidIntInput(const string& prompt) {
    int val;
    while (true) {
        cout << prompt;
        if (cin >> val) return val;
        cout << "Invalid input! Enter a valid number.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

//add member function
void addMember(vector<Member>& members) {
    Member newMember;
    newMember.memberID = nextMemberID++;

    cout << "\n--- Add New Member ---\n";
    cout << "Enter Name: ";
    getline(cin >> ws, newMember.name);
    cout << "Enter Phone: ";
    getline(cin, newMember.phoneNum);
    cout << "Enter Email: ";
    getline(cin, newMember.email);

    int choice;
    do {
        choice = getValidIntInput("Membership Type (1: Regular, 2: Premium): ");
        if (choice == 1) { newMember.membershipType = "Regular"; break; }
        if (choice == 2) { newMember.membershipType = "Premium"; break; }
    } while (true);

    members.push_back(newMember);
    cout << "Member registered with ID: C" << newMember.memberID << "\n";
}

//display member function
void displayMembers(const vector<Member>& members) {
    if (members.empty()) {
        cout << "No members found.\n";
        return;
    }
    for (const auto& m : members) {
        cout << "ID: " << m.memberID << " | Name: " << m.name
            << " | Phone: " << m.phoneNum << " | Type: " << m.membershipType << "\n";
    }
}

//searching function
void searchMember(const vector<Member>& members, int targetID) {
    for (const auto& m : members) {
        if (m.memberID == targetID) {
            cout << "Found: " << m.name << " (" << m.membershipType << ")\n";
            return;
        }
    }
    cout << "Member ID " << targetID << " not found.\n";
}

//update member function
void updateMember(vector<Member>& members, int targetID) {
    for (auto& m : members) {
        if (m.memberID == targetID) {
            cout << "Enter New Name: ";
            getline(cin >> ws, m.name);
            cout << "Enter new phone number:";
            getline(cin, m.phoneNum);
            cout << "Enter new email:";
            getline(cin, m.email);
            cout << "Enter new membership type:";
            getline(cin, m.membershipType);
            cout << "Updated successfully.\n";
            return;
        }
    }
    cout << "Member ID " << targetID << " not found.\n";
}

//delete member fucntion
void deleteMember(vector<Member>& members, int targetID) {
    for (auto it = members.begin(); it != members.end(); ++it) {
        if (it->memberID == targetID) {
            members.erase(it);
            cout << "Deleted successfully.\n";
            return;
        }
    }
    cout << "Member ID " << targetID << " not found.\n";
}

void saveToFile(const vector<Member>& members, const string& filename) {
    ofstream outFile(filename);
    if (!outFile) {
        cout << "Error opening file for saving!\n";
        return;
    }
    for (const auto& m : members) {
        outFile << m.memberID << "|" << m.name << "|" << m.phoneNum
            << "|" << m.email << "|" << m.membershipType << "\n";
    }
    outFile.close();
    cout << "Data saved successfully to " << filename << "\n";
}

//load members from text file into vector u
void loadFromFile(vector<Member>& members, const string& filename) {
    ifstream inFile(filename);
    if (!inFile) return;

    members.clear();
    string line;
    int maxID = 1000;

    while (getline(inFile, line)) {
        if (line.empty()) continue;
        stringstream ss(line);
        Member m;
        string tempID;

        getline(ss, tempID, '|');
        getline(ss, m.name, '|');
        getline(ss, m.phoneNum, '|');
        getline(ss, m.email, '|');
        getline(ss, m.membershipType, '|');

        m.memberID = stoi(tempID);
        if (m.memberID > maxID) maxID = m.memberID;

        members.push_back(m);
    }
    inFile.close();

    nextMemberID = maxID + 1;
}

void memberMenu(vector<Member>& members) {
    int choice = 0;
    do {
        clearScreen();
        cout << "\n=========================================\n";
        cout << "        MEMBER MANAGEMENT SYSTEM         \n";
        cout << "=========================================\n";
        cout << "1. Add New Member\n";
        cout << "2. Display All Members\n";
        cout << "3. Search Member by ID\n";
        cout << "4. Update Member Details\n";
        cout << "5. Delete Member\n";
        cout << "6. Back to Main Menu\n";

        choice = getValidIntInput("Enter choice (1-6): ");

        switch (choice) {
        case 1:
            clearScreen();
            addMember(members);
            saveToFile(members);
            break;
        case 2:
           clearScreen();
            displayMembers(members);
            break;
        case 3:
            clearScreen();
            searchMember(members, getValidIntInput("Enter Member ID: "));
            break;
        case 4:
            clearScreen();
            updateMember(members, getValidIntInput("Enter Member ID: "));
            saveToFile(members);
            break;
        case 5:
            clearScreen();
            deleteMember(members, getValidIntInput("Enter Member ID: "));
            saveToFile(members);
            break;
        case 6:
            saveToFile(members);
            cout << "\nReturning to Main Menu...\n";
            break;
        default:
            cout << "Invalid selection!\n";
        }
    } while (choice != 6);
}

bool memberExists(const vector<Member>& members,int memberID){
    for(const auto& member:members){
        if(member.memberID==memberID){
            return true;
        }
    }
    return false;
}