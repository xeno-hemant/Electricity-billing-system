/*
 * ==========================================================
 *  MODULE 2 : BILLING
 *  Developer : Hemant Khandelwal
 * ==========================================================
 *  Work done in this module:
 *    - Tariff (slab rates) and bill calculation
 *    - Generate bill from the new meter reading
 *    - Print / view the bill
 *    - Show tariff chart
 * ==========================================================
 */

#include "common.h"

// ------------------------------------------------------------
// Calculate the bill. Returns false if the reading is invalid.
// ------------------------------------------------------------
bool Consumer::calculateBill(int newReading, string billMonth) {
    if (newReading < currentReading) {
        return false;                       // reading cannot go down
    }

    previousReading = currentReading;       // old current becomes previous
    currentReading = newReading;
    units = currentReading - previousReading;
    month = billMonth;

    double energy = 0;
    double fixedCharge = 0;

    if (type == 1) {                        // Domestic tariff
        fixedCharge = 50;
        if (units <= 100)
            energy = units * 3.50;
        else if (units <= 200)
            energy = 100 * 3.50 + (units - 100) * 5.00;
        else
            energy = 100 * 3.50 + 100 * 5.00 + (units - 200) * 7.00;
    } else {                                // Commercial tariff
        fixedCharge = 150;
        if (units <= 100)
            energy = units * 6.00;
        else if (units <= 300)
            energy = 100 * 6.00 + (units - 100) * 8.00;
        else
            energy = 100 * 6.00 + 200 * 8.00 + (units - 300) * 10.00;
    }

    double tax = (energy + fixedCharge) * 0.05;     // 5% tax
    billAmount = energy + fixedCharge + tax;
    paidAmount = 0;                                 // new bill, nothing paid yet
    return true;
}

// ------------------------------------------------------------
// Print the bill on the screen
// ------------------------------------------------------------
void Consumer::printBill() {
    double energy, fixedCharge;
    if (type == 1) fixedCharge = 50; else fixedCharge = 150;
    double tax = billAmount - billAmount / 1.05;
    energy = billAmount - tax - fixedCharge;

    cout << "\n==========================================" << endl;
    cout << "         ELECTRICITY BILL" << endl;
    cout << "==========================================" << endl;
    cout << "Consumer ID      : " << id << endl;
    cout << "Name             : " << underscoreToSpace(name) << endl;
    cout << "Address          : " << underscoreToSpace(address) << endl;
    cout << "Category         : " << getTypeName() << endl;
    cout << "Month            : " << underscoreToSpace(month) << endl;
    cout << "------------------------------------------" << endl;
    cout << "Previous reading : " << previousReading << endl;
    cout << "Current reading  : " << currentReading << endl;
    cout << "Units consumed   : " << units << endl;
    cout << "------------------------------------------" << endl;
    cout << "Energy charges   : Rs. " << energy << endl;
    cout << "Fixed charges    : Rs. " << fixedCharge << endl;
    cout << "Tax (5%)         : Rs. " << tax << endl;
    cout << "------------------------------------------" << endl;
    cout << "TOTAL BILL       : Rs. " << billAmount << endl;
    cout << "Paid             : Rs. " << paidAmount << endl;
    cout << "DUE              : Rs. " << getDue() << endl;
    cout << "==========================================" << endl;
}

// ------------------------------------------------------------
// Menu option 4 : Generate bill
// ------------------------------------------------------------
void generateBill() {
    int id, reading;
    string month;

    cout << "\n--- Generate Bill ---\n";
    cout << "Enter consumer ID: ";
    cin >> id;
    int pos = findConsumer(id);
    if (pos == -1) {
        cout << "Consumer not found!\n";
        return;
    }

    if (consumers[pos].getDue() > 0) {
        cout << "Previous bill is still unpaid (Rs. " << consumers[pos].getDue()
             << "). Please clear it first.\n";
        return;
    }

    cin.ignore();
    cout << "Enter month (e.g. April 2026): ";
    getline(cin, month);
    cout << "Current meter reading (last was "
         << consumers[pos].getPreviousReading() << " or more): ";
    cin >> reading;

    if (consumers[pos].calculateBill(reading, spaceToUnderscore(month))) {
        saveToFile();
        consumers[pos].printBill();
    } else {
        cout << "Error: new reading is less than the old reading!\n";
    }
}

// ------------------------------------------------------------
// Menu option 5 : View bill
// ------------------------------------------------------------
void viewBill() {
    int id;
    cout << "Enter consumer ID: ";
    cin >> id;
    int pos = findConsumer(id);
    if (pos == -1) cout << "Consumer not found!\n";
    else consumers[pos].printBill();
}

// ------------------------------------------------------------
// Menu option 9 : Tariff chart
// ------------------------------------------------------------
void showTariff() {
    cout << "\n--- Tariff Chart ---\n";
    cout << "DOMESTIC (fixed charge Rs. 50)\n";
    cout << "  First 100 units  : Rs. 3.50 per unit\n";
    cout << "  101 to 200 units : Rs. 5.00 per unit\n";
    cout << "  Above 200 units  : Rs. 7.00 per unit\n";
    cout << "COMMERCIAL (fixed charge Rs. 150)\n";
    cout << "  First 100 units  : Rs. 6.00 per unit\n";
    cout << "  101 to 300 units : Rs. 8.00 per unit\n";
    cout << "  Above 300 units  : Rs. 10.00 per unit\n";
    cout << "Tax: 5% on (energy + fixed charges)\n";
}
