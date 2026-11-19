/*
 * ==========================================================
 *  MODULE 3 : PAYMENT & DUES
 *  Developer : Himanshi Pancholi
 * ==========================================================
 *  Work done in this module:
 *    - Accept full or partial payment
 *    - Print payment receipt
 *    - Show pending dues of all consumers
 * ==========================================================
 */

#include "common.h"

// ------------------------------------------------------------
// Add the payment to the bill. Returns false if amount is wrong.
// ------------------------------------------------------------
bool Consumer::makePayment(double amount) {
    if (amount <= 0 || amount > getDue()) {
        return false;
    }
    paidAmount = paidAmount + amount;
    return true;
}

// ------------------------------------------------------------
// Print the receipt
// ------------------------------------------------------------
void Consumer::printReceipt(double amount) {
    cout << "\n********** PAYMENT RECEIPT **********" << endl;
    cout << "Consumer ID : " << id << endl;
    cout << "Name        : " << underscoreToSpace(name) << endl;
    cout << "Month       : " << underscoreToSpace(month) << endl;
    cout << "Amount paid : Rs. " << amount << endl;
    cout << "Balance due : Rs. " << getDue() << endl;
    if (getDue() == 0) cout << "Status      : FULLY PAID" << endl;
    else cout << "Status      : PARTIALLY PAID" << endl;
    cout << "*************************************" << endl;
}

// ------------------------------------------------------------
// Menu option 6 : Pay bill
// ------------------------------------------------------------
void payBill() {
    int id;
    double amount;

    cout << "\n--- Pay Bill ---\n";
    cout << "Enter consumer ID: ";
    cin >> id;
    int pos = findConsumer(id);
    if (pos == -1) {
        cout << "Consumer not found!\n";
        return;
    }
    if (consumers[pos].getDue() == 0) {
        cout << "No dues for this consumer.\n";
        return;
    }

    cout << "Amount due: Rs. " << consumers[pos].getDue() << endl;
    cout << "Enter amount to pay: Rs. ";
    cin >> amount;

    if (consumers[pos].makePayment(amount)) {
        saveToFile();
        consumers[pos].printReceipt(amount);
    } else {
        cout << "Invalid amount! It must be more than 0 and not more than the due.\n";
    }
}

// ------------------------------------------------------------
// Menu option 7 : Pending dues
// ------------------------------------------------------------
void showDues() {
    cout << "\n--- Pending Dues ---\n";
    double sum = 0;
    int count = 0;
    for (int i = 0; i < total; i++) {
        if (consumers[i].getDue() > 0) {
            consumers[i].displayRow();
            sum = sum + consumers[i].getDue();
            count++;
        }
    }
    if (count == 0) cout << "No pending dues.\n";
    else cout << "Total pending amount: Rs. " << sum << endl;
}
