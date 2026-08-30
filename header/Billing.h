#ifndef BILLING_H
#define BILLING_H
#pragma once

#include "member.h"
#include <iostream>
#include <vector>
#include <string>

using namespace std;

struct Billing
{
    int bookingID;
    int memberID;
    int bikeID;
    string bikeType;
    double hourlyRate;
    int hours;
    double totalCost;
    double payment;
    double change;
    bool paid;
};
extern int nextBookingId;

Billing createBilling(
    int memberID,
    int bikeID,
    string bikeType,
    double hourlyRate,
    int hours);

double calculateRentalCost(double hourlyRate, int hours);

void processPayment(Billing& bill);

void generateReceipt(const Billing& bill);

double calculateTotalIncome(const vector<Billing>& bills);

void showReport(const vector<Billing>& bills, const vector<Member>& members);

void billingMenu(vector<Billing>& bills, const vector<Member>& members);

#endif