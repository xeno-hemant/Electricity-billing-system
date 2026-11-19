/*
 * ==========================================================
 *  MODULE 1 : CONSUMER MANAGEMENT
 *  Developer : Hemant Gurjar
 * ==========================================================
 *  Work done in this module:
 *    - Consumer constructor and setting consumer details
 *    - Add a new consumer
 *    - Show all consumers
 *    - Search a consumer by ID
 *    - Delete a consumer
 *    - Helper functions (find consumer, space <-> underscore)
 * ==========================================================
 */

#include "common.h"

// Global array of objects (shared by all modules)
Consumer consumers[MAX];
int total = 0;

// ------------------------------------------------------------
// Helper functions: names are saved in the file with '_' instead
// of spaces so that reading with >> is easy.
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
// Consumer class functions that belong to Module 1
// ------------------------------------------------------------
Consumer::Consumer() {                  // constructor: default values
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

void Consumer::setDetails(int i, string n, string a, int t, int startReading) {
    id = i;
    name = n;
    address = a;
    type = t;
    previousReading = startReading;
    currentReading = startReading;
}

string Consumer::getTypeName() {
    if (type == 1) return "Domestic";
    else return "Commercial";
}

void Consumer::displayRow() {           // one line of the table
    cout << id << "\t" << underscoreToSpace(name) << "\t\t"
         << getTypeName() << "\t" << currentReading << "\t"
         << getDue() << endl;
}

void Consumer::displayFull() {          // full details
    cout << "\nConsumer ID   : " << id << endl;
    cout << "Name          : " << underscoreToSpace(name) << endl;
    cout << "Address       : " << underscoreToSpace(address) << endl;
    cout << "Type          : " << getTypeName() << endl;
    cout << "Last reading  : " << currentReading << endl;
    cout << "Latest bill   : Rs. " << billAmount << " ("
         << underscoreToSpace(month) << ")" << endl;
    cout << "Amount paid   : Rs. " << paidAmount << endl;
    cout << "Amount due    : Rs. " << getDue() << endl;
}

// ------------------------------------------------------------
// Find the position of a consumer in the array (-1 if not found)
// ------------------------------------------------------------
int findConsumer(int id) {
    for (int i = 0; i < total; i++) {
        if (consumers[i].getId() == id) {
            return i;
        }
    }
    return -1;
}

// ------------------------------------------------------------
// Menu option 1 : Add consumer
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

    cin.ignore();                       // clear Enter key from the buffer
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
    saveToFile();                       // saveToFile() is in Module 4
    cout << "Consumer added successfully!\n";
}

// ------------------------------------------------------------
// Menu option 2 : Show all consumers
// ------------------------------------------------------------
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

// ------------------------------------------------------------
// Menu option 3 : Search consumer
// ------------------------------------------------------------
void searchConsumer() {
    int id;
    cout << "Enter consumer ID to search: ";
    cin >> id;
    int pos = findConsumer(id);
    if (pos == -1) cout << "Consumer not found!\n";
    else consumers[pos].displayFull();
}

// ------------------------------------------------------------
// Menu option 8 : Delete consumer
// ------------------------------------------------------------
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
