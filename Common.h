/*
 * common.h  -  SHARED HEADER (used by all 4 modules)
 * ---------------------------------------------------------
 * Contains: the Consumer class declaration, global data and
 * the list of functions that belong to each module.
 *
 * Team: Hemant Gurjar | Hemant Khandelwal |
 *       Himanshi Pancholi | Harshpal Singh
 */

#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX = 100;                       // maximum number of consumers
const string FILE_NAME = "consumers.txt";  // data file

// ------------------------------------------------------------
// CLASS: Consumer  (one object = one electricity customer)
// The body of each member function is written in the module
// that is responsible for it (shown in the comments below).
// ------------------------------------------------------------
class Consumer {
private:
    int id;
    string name;
    string address;
    int type;               // 1 = Domestic, 2 = Commercial
    int previousReading;
    int currentReading;
    int units;
    string month;           // month of the latest bill
    double billAmount;      // total of latest bill
    double paidAmount;      // amount paid so far

public:
    // ---- Module 1 (module1_consumer.cpp) ----
    Consumer();
    void setDetails(int i, string n, string a, int t, int startReading);
    string getTypeName();
    void displayRow();
    void displayFull();

    // ---- Module 2 (module2_billing.cpp) ----
    bool calculateBill(int newReading, string billMonth);
    void printBill();

    // ---- Module 3 (module3_payment.cpp) ----
    bool makePayment(double amount);
    void printReceipt(double amount);

    // ---- Module 4 (module4_storage_main.cpp) ----
    void writeToFile(ofstream &out);
    bool readFromFile(ifstream &in);

    // ---- Small getters (used by all modules) ----
    int getId()              { return id; }
    double getDue()          { return billAmount - paidAmount; }
    double getBill()         { return billAmount; }
    double getPaid()         { return paidAmount; }
    int getPreviousReading() { return previousReading; }
    int getUnits()           { return units; }
};

// ------------------------------------------------------------
// Global data (created in module 1, shared by everyone)
// ------------------------------------------------------------
extern Consumer consumers[MAX];
extern int total;

// ------------------------------------------------------------
// MODULE 1 : Consumer Management      (Hemant Gurjar)
// ------------------------------------------------------------
string spaceToUnderscore(string s);
string underscoreToSpace(string s);
int  findConsumer(int id);
void addConsumer();
void showAll();
void searchConsumer();
void deleteConsumer();

// ------------------------------------------------------------
// MODULE 2 : Billing                  (Hemant Khandelwal)
// ------------------------------------------------------------
void generateBill();
void viewBill();
void showTariff();

// ------------------------------------------------------------
// MODULE 3 : Payment & Dues           (Himanshi Pancholi)
// ------------------------------------------------------------
void payBill();
void showDues();

// ------------------------------------------------------------
// MODULE 4 : File Storage, Report & Main Menu  (Harshpal Singh)
// ------------------------------------------------------------
void saveToFile();
void loadFromFile();
void showSummary();

#endif
