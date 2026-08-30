#include <iostream>
#include "../header/Billing.h"
#include <iomanip>

using namespace std;
int nextBookingId = 1;

double calculateRentalCost(double hourlyRate, int hours)
{
    return hourlyRate * hours;
}

void processPayment(Billing& bill){
    cout<<"\nTotal Cost: RM"<<bill.totalCost<<endl;

    do{
        cout<<"Enter payment amount: RM";
        if(!(cin>> bill.payment)){
            cout<<"Invalid Input!Please enter a number.\n";
            cin.clear();
            cin.ignore(1000,'\n');
            continue;
        }

        if(bill.payment<bill.totalCost){
            cout<<"Insufficient payment! Please enter a higher amount.\n";
        }
    }while((bill.payment<bill.totalCost));

    bill.change=bill.payment-bill.totalCost;
    bill.paid=true;
    cout<<"Payment Successful!"<<endl;
    cout<<"Change: RM"<<bill.change<<endl;
    


}

void generateReceipt(const Billing& bill){
      cout << "======================================";
    cout<<"\nBICYCLE RENTAL SYSTEM"<<endl;
      cout << "======================================" << endl;

    cout<<setw(10)<<"Booking ID  :"<<bill.bookingID<<endl;
    cout<<setw(10)<<"Member ID   :"<<bill.memberID<<endl;
    cout<<setw(10)<<"Bike ID     :"<<bill.bikeID<<endl;
    cout<<setw(10)<<"Bike Type   :"<<bill.bikeType<<endl;
    cout<<setw(10)<<"Hourly Rate :"<<bill.hourlyRate<<endl;
    cout<<setw(10)<<"Duration    :"<<bill.hours<<endl;
      cout << "-------------------------------------" << endl;
    cout<<setw(10)<<"Total Cost :"<<bill.totalCost<<endl;
    cout<<setw(10)<<"Payment :"<<bill.payment<<endl;
    cout<<setw(10)<<"Change :"<<bill.change<<endl;

      cout << "======================================" << endl;
    cout<<"\n"<<setw(10)<<"Thank You!"<<endl;
}

void billingMenu(vector<Billing>& bills, const vector<Member>& members){
    int choice;
    do{
         cout << "\n======================================" << endl;
        cout<<"        "<<"BILLING AND RECORD!";
          cout << "\n======================================" << endl;

        cout<<"1.Process Payment"<<endl;
        cout<<"2.View Billing Record"<<endl;
        cout<<"3.View Report"<<endl;
        cout<<"4.Back to Main Menu"<<endl;
          cout << "======================================" << endl;

        cout<<"Enter Choice:";
        cin>>choice;

        switch(choice){
            case 1:
            {
                if(bills.empty()){
                    cout<<"\nNo billing record available"<<endl;
                    break;
                }
                bool unpaidFound = false;

                cout << "\n--- Unpaid Billing Records ---" << endl;

                for (const auto& bill : bills) {
                    if (!bill.paid) {
                        unpaidFound = true;                
                        cout<<"Booking ID:"<<bill.bookingID<<endl;
                        cout<<"Member ID :"<<bill.memberID<<endl;
                        cout<<"Bike      :"<<bill.bikeType<<endl;
                        cout<<"Duration  :"<<bill.hours<<endl;

                        cout << "-----------------------------------" << endl;
                    }
                }
                if (!unpaidFound) {
                    cout << "\nAll bookings have been paid." << endl;
                    break;
                }
                int bookingID;
                cout << "\nEnter Booking ID to make payment: ";
                cin >> bookingID;

                 Billing* bill = nullptr;

                for (auto& b : bills) {
                    if (b.bookingID == bookingID && !b.paid) {
                    bill = &b;
                    break;
                    }
                }       

                if (bill == nullptr) {
                    cout << "Unpaid Booking ID not found!" << endl;
                    break;
                }

                cout<<"\n---Process Payment ---"<<endl;
                cout<<"Booking ID:"<<bill->bookingID<<endl;
                cout<<"Member ID :"<<bill->memberID<<endl;
                cout<<"Bike      :"<<bill->bikeType<<endl;
                cout<<"Duration  :"<<bill->hours<<endl;


                processPayment(*bill);
                generateReceipt(*bill);
                break;
            }
            case 2:{
                cout<<"\n---Billing Records ---"<<endl;
                if(bills.empty()){
                    cout<<"\nNo billing record found"<<endl;
                    break;
                }else{
                    for(const auto& bill :bills){
                        cout<<"\n"<<setfill('-')<<setw(20)<<endl;
                        cout<<"Booking ID:"<<bill.bookingID<<endl;
                        cout<<"Member ID :"<<bill.memberID<<endl;
                        cout<<"Bike      :"<<bill.bikeType<<endl;
                        cout<<"Duration  :"<<bill.hours<<endl;
                        cout<<"Total Cost:RM"<<fixed<<setprecision(2)<<bill.totalCost<<endl;
                    }
                      cout << "-----------------------------------" << endl;
                }
                break;
            }
            case 3:
            showReport(bills,members);
            break;

            case 4:
             cout<<"Returning......."<<endl;
            break;

            default:
                cout<<"Invalid Choice! Please try again."<<endl;
        }
    }while(choice!=4);
}


Billing createBilling(
    int memberID,
    int bikeId,
    string bikeType,
    double hourlyRate,
    int hours)
{
    Billing bill;
    bill.bookingID = nextBookingId++;
    bill.memberID = memberID;
    bill.bikeID = bikeId;
    bill.bikeType = bikeType;
    bill.hourlyRate = hourlyRate;
    bill.hours = hours;

    bill.totalCost = calculateRentalCost(
        bill.hourlyRate,
        bill.hours
    );
    bill.payment=0;
    bill.change=0;
    bill.paid=false;

    return bill;
}

double calculateTotalIncome(const vector<Billing>& bills){
    double totalIncome=0;

    for(const auto&bill:bills){
        if(bill.paid){
        totalIncome+=bill.totalCost;
        }
    }
    return totalIncome;
    
}

void showReport(const vector<Billing>&bills,const vector<Member>& members){
    double totalIncome =calculateTotalIncome(bills);

    cout << "\n======================================" << endl;
    cout<<"             Rental Report"<<endl;
    cout << "======================================" << endl;

    cout <<"Total Income     :RM"<<fixed<<setprecision(2)<<totalIncome<<endl;
    cout<<"Number of Customer: "<<members.size()<<endl;

    if (bills.empty()){
        cout<<"Most Rental Bike :No rental record"<<endl;
    }
    else{
        vector<int>bikeIDs;
        vector<int>rentalCounts;

        for(const auto& bill:bills){
            bool found =false;

            for(int i=0; i<bikeIDs.size();i++)
            {
                if (bikeIDs[i]==bill.bikeID){
                    rentalCounts[i]+=1;
                    found =true;
                    break;
                }
            }
            if(!found){
                bikeIDs.push_back(bill.bikeID);
                rentalCounts.push_back(1);
            }
        }
        int maxIndex=0;
        for(int i=1;i<rentalCounts.size();i++)
        {
             if(rentalCounts[i] > rentalCounts[maxIndex]){
                maxIndex=i;
            }
        }
        cout<<"Most Rental Bike  :"<<bikeIDs[maxIndex]<<" ("<<rentalCounts[maxIndex]<<"times)"<<endl;
    }
}
