/*
 * ==========================================================
 *  MODULE 4 : FILE STORAGE, REPORT & MAIN MENU
 *  Developer : Harshpal Singh
 * ==========================================================
 *  Work done in this module:
 *    - Save data to file / load data from file
 *    - Summary report (totals)
 *    - Main menu that connects all the modules
 * ==========================================================
 */

#include "common.h"

// ------------------------------------------------------------
// Write one consumer as one line of the file
// ------------------------------------------------------------
void Consumer::writeToFile(ofstream &out) {
    out << id << " " << type << " " << previousReading << " "
        << currentReading << " " << units << " " << billAmount << " "
        << paidAmount << " " << name << " " << address << " "
        << month << endl;
}

// ------------------------------------------------------------
// Read one consumer from the file (false when file ends)
// ------------------------------------------------------------
bool Consumer::readFromFile(ifstream &in) {
    if (in >> id >> type >> previousReading >> currentReading >> units
           >> billAmount >> paidAmount >> name >> address >> month) {
        return true;
    }
    return false;
}

// ------------------------------------------------------------
// Save all consumers to the file
// ------------------------------------------------------------
void saveToFile() {
    ofstream out(FILE_NAME.c_str());
    for (int i = 0; i < total; i++) {
        consumers[i].writeToFile(out);
    }
    out.close();
}

// ------------------------------------------------------------
// Load all consumers from the file (when program starts)
// ------------------------------------------------------------
void loadFromFile() {
    ifstream in(FILE_NAME.c_str());
    if (!in) return;                    // file does not exist yet
    total = 0;
    while (total < MAX && consumers[total].readFromFile(in)) {
        total++;
    }
    in.close();
}

// ------------------------------------------------------------
// Menu option 10 : Summary report
// ------------------------------------------------------------
void showSummary() {
    int domestic = 0, commercial = 0;
    long totalUnits = 0;
    double billed = 0, collected = 0, pending = 0;

    for (int i = 0; i < total; i++) {
        if (consumers[i].getTypeName() == "Domestic") domestic++;
        else commercial++;
        totalUnits = totalUnits + consumers[i].getUnits();
        billed = billed + consumers[i].getBill();
        collected = collected + consumers[i].getPaid();
        pending = pending + consumers[i].getDue();
    }

    cout << "\n========== SUMMARY REPORT ==========\n";
    cout << "Total consumers  : " << total << endl;
    cout << "  Domestic       : " << domestic << endl;
    cout << "  Commercial     : " << commercial << endl;
    cout << "Units (latest)   : " << totalUnits << endl;
    cout << "Total billed     : Rs. " << billed << endl;
    cout << "Amount collected : Rs. " << collected << endl;
    cout << "Amount pending   : Rs. " << pending << endl;
    cout << "====================================\n";
}

// ------------------------------------------------------------
// MAIN : shows the menu and calls the functions of all modules
// ------------------------------------------------------------
int main() {
    int choice;
    loadFromFile();

    cout << "==========================================\n";
    cout << "    ELECTRICITY BILLING SYSTEM\n";
    cout << "  Hemant Gurjar | Hemant Khandelwal\n";
    cout << "  Himanshi Pancholi | Harshpal Singh\n";
    cout << "==========================================\n";

    do {
        cout << "\n 1. Add consumer          (Module 1)\n";
        cout << " 2. Show all consumers    (Module 1)\n";
        cout << " 3. Search consumer       (Module 1)\n";
        cout << " 4. Generate bill         (Module 2)\n";
        cout << " 5. View bill             (Module 2)\n";
        cout << " 6. Pay bill              (Module 3)\n";
        cout << " 7. Pending dues          (Module 3)\n";
        cout << " 8. Delete consumer       (Module 1)\n";
        cout << " 9. Tariff chart          (Module 2)\n";
        cout << "10. Summary report        (Module 4)\n";
        cout << " 0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {               // user typed a letter instead of a number
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
        }

        switch (choice) {
            case 1:  addConsumer();      break;
            case 2:  showAll();          break;
            case 3:  searchConsumer();   break;
            case 4:  generateBill();     break;
            case 5:  viewBill();         break;
            case 6:  payBill();          break;
            case 7:  showDues();         break;
            case 8:  deleteConsumer();   break;
            case 9:  showTariff();       break;
            case 10: showSummary();      break;
            case 0:  saveToFile(); cout << "Data saved. Goodbye!\n"; break;
            default: cout << "Invalid choice, try again.\n";
        }
    } while (choice != 0);

    return 0;
}
