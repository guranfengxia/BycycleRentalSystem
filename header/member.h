#pragma once
#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Member {
    int memberID;
    string name;
    string phoneNum;
    string email;
    string membershipType;
};

//global id declaration(auto memberID)
extern int nextMemberID;

//
int getValidIntInput(const std::string& prompt);
void addMember(std::vector<Member>& members);
void displayMembers(const std::vector<Member>& members);
void searchMember(const std::vector<Member>& members, int targetID);
void updateMember(std::vector<Member>& members, int targetID);
void deleteMember(std::vector<Member>& members, int targetID);
void memberMenu(std::vector<Member>& members);
void saveToFile(const vector<Member>& members, const string& filename = "members.txt");
void loadFromFile(vector<Member>& members, const string& filename = "members.txt");
bool memberExists(const vector<Member>& members, int memberID);