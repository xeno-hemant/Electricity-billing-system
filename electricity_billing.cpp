/*
 * ==========================================================
 *        ELECTRICITY BILLING SYSTEM  (C++ OOPs Lab Project)
 * ==========================================================
 *  Team Members:
 *    1. Hemant Gurjar
 *    2. Hemant Khandelwal
 *    3. Himanshi Pancholi
 *    4. Harshpal Singh
 * ==========================================================
 *  Topics used: class, object, array of objects, functions,
 *  loops, if-else, switch, file handling (ifstream/ofstream)
 * ==========================================================
 */

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

const int MAX = 100;                    // maximum number of consumers
const string FILE_NAME = "consumers.txt";

// ------------------------------------------------------------
// Helper functions: we save names in the file with '_' instead
// of spaces so that reading with >> is easy for beginners.
// ------------------------------------------------------------
string spaceToUnderscore(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] == ' ') s[i] = '_';
    }
    return s;
}

string underscoreToSpace(string s) {
    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] == '_') s[i] = ' ';
    }
    return s;
}

// ------------------------------------------------------------
// CLASS: Consumer  (one object = one electricity customer)
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
    double paidAmount;      // amount paid so far for latest bill

public:
    // Constructor: sets starting values
    Consumer() {
        id = 0;
        name = "";
        address = "";
        type = 1;
        previousReading = 0;
        currentReading = 0;
        units = 0;
        month = "None";
        billAmount = 0;
        paidAmount = 0;
    }

    // ---------- setters / getters ----------
    void setDetails(int i, string n, string a, int t, int startReading) {
        id = i;
        name = n;
        address = a;
        type = t;
        previousReading = startReading;
        currentReading = startReading;
    }

    int getId() { return id; }
    double getDue() { return billAmount - paidAmount; }
    double getBill() { return billAmount; }
    double getPaid() { return paidAmount; }
    int getPreviousReading() { return previousReading; }

    string getTypeName() {
        if (type == 1) return "Domestic";
        else return "Commercial";
    }

    // ---------- show one consumer in a table row ----------
    void displayRow() {
        cout << id << "\t" << underscoreToSpace(name) << "\t\t"
             << getTypeName() << "\t" << currentReading << "\t"
             << getDue() << endl;
    }

    // ---------- show full details ----------
    void displayFull() {
        cout << "\nConsumer ID   : " << id << endl;
        cout << "Name          : " << underscoreToSpace(name) << endl;
        cout << "Address       : " << underscoreToSpace(address) << endl;
        cout << "Type          : " << getTypeName() << endl;
        cout << "Last reading  : " << currentReading << endl;
        cout << "Latest bill   : Rs. " << billAmount << " (" << underscoreToSpace(month) << ")" << endl;
        cout << "Amount paid   : Rs. " << paidAmount << endl;
        cout << "Amount due    : Rs. " << getDue() << endl;
    }

    // ---------- calculate bill from new reading ----------
    // Returns false if the reading is invalid.
    bool calculateBill(int newReading, string billMonth) {
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

    // ---------- print the bill ----------
    void printBill() {
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

    // ---------- accept payment ----------
    bool makePayment(double amount) {
        if (amount <= 0 || amount > getDue()) {
            return false;
        }
        paidAmount = paidAmount + amount;
        return true;
    }

    // ---------- print receipt ----------
    void printReceipt(double amount) {
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

    // ---------- file handling ----------
    void writeToFile(ofstream &out) {
        out << id << " " << type << " " << previousReading << " "
            << currentReading << " " << units << " " << billAmount << " "
            << paidAmount << " " << name << " " << address << " "
            << month << endl;
    }

    bool readFromFile(ifstream &in) {
        if (in >> id >> type >> previousReading >> currentReading >> units
               >> billAmount >> paidAmount >> name >> address >> month) {
            return true;
        }
        return false;
    }
};

// ------------------------------------------------------------
// Global array of objects and counter
// ------------------------------------------------------------
Consumer consumers[MAX];
int total = 0;

// Find the position of a consumer in the array (-1 if not found)
int findConsumer(int id) {
    for (int i = 0; i < total; i++) {
        if (consumers[i].getId() == id) {
            return i;
        }
    }
    return -1;
}

// ------------------------------------------------------------
// File functions
// ------------------------------------------------------------
void saveToFile() {
    ofstream out(FILE_NAME.c_str());
    for (int i = 0; i < total; i++) {
        consumers[i].writeToFile(out);
    }
    out.close();
}

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
// Menu functions
// ------------------------------------------------------------
void addConsumer() {
    if (total >= MAX) {
        cout << "Memory full! Cannot add more consumers.\n";
        return;
    }

    int id, type, reading;
    string name, address;

    cout << "\n--- Add New Consumer ---\n";
    cout << "Enter consumer ID: ";
    cin >> id;

    if (findConsumer(id) != -1) {
        cout << "This ID already exists!\n";
        return;
    }

    cin.ignore();                       // clear the Enter key from the buffer
    cout << "Enter name: ";
    getline(cin, name);
    cout << "Enter address: ";
    getline(cin, address);

    cout << "Type (1 = Domestic, 2 = Commercial): ";
    cin >> type;
    while (type != 1 && type != 2) {
        cout << "Please enter 1 or 2: ";
        cin >> type;
    }

    cout << "Enter starting meter reading: ";
    cin >> reading;

    consumers[total].setDetails(id, spaceToUnderscore(name),
                                spaceToUnderscore(address), type, reading);
    total++;
    saveToFile();
    cout << "Consumer added successfully!\n";
}

void showAll() {
    cout << "\n--- All Consumers ---\n";
    if (total == 0) {
        cout << "No records found.\n";
        return;
    }
    cout << "ID\tName\t\tType\tReading\tDue(Rs.)\n";
    cout << "--------------------------------------------------\n";
    for (int i = 0; i < total; i++) {
        consumers[i].displayRow();
    }
}

void searchConsumer() {
    int id;
    cout << "Enter consumer ID to search: ";
    cin >> id;
    int pos = findConsumer(id);
    if (pos == -1) cout << "Consumer not found!\n";
    else consumers[pos].displayFull();
}

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

void viewBill() {
    int id;
    cout << "Enter consumer ID: ";
    cin >> id;
    int pos = findConsumer(id);
    if (pos == -1) cout << "Consumer not found!\n";
    else consumers[pos].printBill();
}

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

void deleteConsumer() {
    int id;
    cout << "Enter consumer ID to delete: ";
    cin >> id;
    int pos = findConsumer(id);
    if (pos == -1) {
        cout << "Consumer not found!\n";
        return;
    }
    if (consumers[pos].getDue() > 0) {
        cout << "Cannot delete: consumer has pending dues.\n";
        return;
    }
    // shift all later records one step to the left
    for (int i = pos; i < total - 1; i++) {
        consumers[i] = consumers[i + 1];
    }
    total--;
    saveToFile();
    cout << "Consumer deleted.\n";
}

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

// ------------------------------------------------------------
// MAIN
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
        cout << "\n1. Add consumer\n";
        cout << "2. Show all consumers\n";
        cout << "3. Search consumer\n";
        cout << "4. Generate bill\n";
        cout << "5. View bill\n";
        cout << "6. Pay bill\n";
        cout << "7. Pending dues\n";
        cout << "8. Delete consumer\n";
        cout << "9. Tariff chart\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (cin.fail()) {               // user typed a letter instead of a number
            cin.clear();
            cin.ignore(1000, '\n');
            choice = -1;
        }

        switch (choice) {
            case 1: addConsumer();      break;
            case 2: showAll();          break;
            case 3: searchConsumer();   break;
            case 4: generateBill();     break;
            case 5: viewBill();         break;
            case 6: payBill();          break;
            case 7: showDues();         break;
            case 8: deleteConsumer();   break;
            case 9: showTariff();       break;
            case 0: saveToFile(); cout << "Data saved. Goodbye!\n"; break;
            default: cout << "Invalid choice, try again.\n";
        }
    } while (choice != 0);

    return 0;
}
