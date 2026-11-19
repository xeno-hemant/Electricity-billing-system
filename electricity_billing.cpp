/*
 * ===========================================================
 *   Electricity Billing System  (C++ / OOP Lab)
 * ===========================================================
 *   Team Members:
 *     1. Hemant Gurjar
 *     2. Hemant Khandelwal
 *     3. Himanshi Pancholi
 *     4. Harshpal Singh
 * ===========================================================
 *   Concepts used: abstraction, inheritance, polymorphism,
 *   encapsulation, STL containers, smart pointers, file I/O.
 * ===========================================================
 */

#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <vector>
#include <memory>
#include <string>
#include <climits>
#include <algorithm>
#include <ctime>
#include <cctype>
using namespace std;

const string CUSTOMER_FILE = "customers.txt";
const string BILL_FILE     = "bills.txt";
const double FUEL_SURCHARGE_RATE = 0.04;  // 4% of energy charge
const double ELECTRICITY_DUTY    = 0.06;  // 6% of (energy + fixed + surcharge)
const double LATE_FEE_RATE       = 0.02;  // 2% of bill total

/* ---------------------------- Utilities ---------------------------- */

string trim(const string &s) {
    size_t a = s.find_first_not_of(" \t\r\n");
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(" \t\r\n");
    return s.substr(a, b - a + 1);
}

string toLower(string s) {
    for (char &c : s) c = (char)tolower((unsigned char)c);
    return s;
}

vector<string> split(const string &line, char sep) {
    vector<string> parts;
    string item;
    stringstream ss(line);
    while (getline(ss, item, sep)) parts.push_back(item);
    return parts;
}

string readLine(const string &prompt) {
    cout << prompt;
    string s;
    if (!getline(cin, s)) { cout << "\nInput closed. Exiting.\n"; exit(0); }
    return trim(s);
}

string readText(const string &prompt) {
    string s;
    do {
        s = readLine(prompt);
        replace(s.begin(), s.end(), '|', '/');   // '|' is our file delimiter
        if (s.empty()) cout << "  Field cannot be empty.\n";
    } while (s.empty());
    return s;
}

int readInt(const string &prompt, int lo, int hi) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t pos;
            int v = stoi(s, &pos);
            if (pos == s.size() && v >= lo && v <= hi) return v;
        } catch (...) {}
        cout << "  Enter a whole number between " << lo << " and " << hi << ".\n";
    }
}

double readDouble(const string &prompt, double lo, double hi) {
    while (true) {
        string s = readLine(prompt);
        try {
            size_t pos;
            double v = stod(s, &pos);
            if (pos == s.size() && v >= lo && v <= hi) return v;
        } catch (...) {}
        cout << "  Enter a number between " << lo << " and " << hi << ".\n";
    }
}

string readPhone() {
    while (true) {
        string p = readLine("Phone (10 digits): ");
        if (p.size() == 10 && all_of(p.begin(), p.end(), ::isdigit)) return p;
        cout << "  Invalid phone number.\n";
    }
}

string readMonth() {
    while (true) {
        string m = readLine("Billing month (MM-YYYY): ");
        if (m.size() == 7 && m[2] == '-' &&
            isdigit((unsigned char)m[0]) && isdigit((unsigned char)m[1])) {
            int mm = stoi(m.substr(0, 2));
            bool yearOk = all_of(m.begin() + 3, m.end(), ::isdigit);
            if (mm >= 1 && mm <= 12 && yearOk) return m;
        }
        cout << "  Use format MM-YYYY, e.g. 04-2026.\n";
    }
}

string nowString() {
    time_t t = time(nullptr);
    char buf[40];
    strftime(buf, sizeof(buf), "%d-%b-%Y %H:%M", localtime(&t));
    return buf;
}

void line(char c = '-', int n = 62) { cout << string(n, c) << "\n"; }

/* --------------------------- Tariff slabs -------------------------- */

struct Slab {
    int    upTo;   // cumulative upper limit of this slab (in units)
    double rate;   // Rs. per unit inside this slab
};

/* ------------------- Customer hierarchy (polymorphism) ------------- */

class Customer {
protected:
    int    id;
    string name, address, phone;
    int    lastReading;   // most recent meter reading on record

public:
    Customer(int id, string name, string address, string phone, int lastReading = 0)
        : id(id), name(move(name)), address(move(address)),
          phone(move(phone)), lastReading(lastReading) {}
    virtual ~Customer() {}

    // Each category supplies its own tariff
    virtual int                 typeCode()    const = 0;
    virtual string              category()    const = 0;
    virtual double              fixedCharge() const = 0;
    virtual const vector<Slab> &slabs()       const = 0;

    // Slab-wise energy charge (common logic, category-specific data)
    double energyCharge(int units) const {
        double total = 0;
        int prev = 0;
        for (const Slab &s : slabs()) {
            if (units <= prev) break;
            int band = min(units, s.upTo) - prev;
            total += band * s.rate;
            prev = s.upTo;
        }
        return total;
    }

    int           getId()          const { return id; }
    const string &getName()        const { return name; }
    const string &getAddress()     const { return address; }
    const string &getPhone()       const { return phone; }
    int           getLastReading() const { return lastReading; }
    void          setLastReading(int r)  { lastReading = r; }

    string serialize() const {
        return to_string(typeCode()) + "|" + to_string(id) + "|" + name + "|" +
               address + "|" + phone + "|" + to_string(lastReading);
    }
};

class DomesticCustomer : public Customer {
public:
    using Customer::Customer;
    int    typeCode()    const override { return 1; }
    string category()    const override { return "Domestic"; }
    double fixedCharge() const override { return 40.0; }
    const vector<Slab> &slabs() const override {
        static const vector<Slab> t = {{100, 3.50}, {200, 5.00}, {400, 7.00}, {INT_MAX, 9.00}};
        return t;
    }
};

class CommercialCustomer : public Customer {
public:
    using Customer::Customer;
    int    typeCode()    const override { return 2; }
    string category()    const override { return "Commercial"; }
    double fixedCharge() const override { return 150.0; }
    const vector<Slab> &slabs() const override {
        static const vector<Slab> t = {{200, 6.50}, {500, 8.00}, {INT_MAX, 10.00}};
        return t;
    }
};

class IndustrialCustomer : public Customer {
public:
    using Customer::Customer;
    int    typeCode()    const override { return 3; }
    string category()    const override { return "Industrial"; }
    double fixedCharge() const override { return 500.0; }
    const vector<Slab> &slabs() const override {
        static const vector<Slab> t = {{1000, 7.50}, {INT_MAX, 9.50}};
        return t;
    }
};

unique_ptr<Customer> makeCustomer(int type, int id, const string &name,
                                  const string &addr, const string &phone, int reading) {
    switch (type) {
        case 1: return unique_ptr<Customer>(new DomesticCustomer(id, name, addr, phone, reading));
        case 2: return unique_ptr<Customer>(new CommercialCustomer(id, name, addr, phone, reading));
        case 3: return unique_ptr<Customer>(new IndustrialCustomer(id, name, addr, phone, reading));
    }
    return nullptr;
}

/* ------------------------------- Bill ------------------------------ */

struct Bill {
    int    billId = 0, custId = 0;
    string month;
    int    prevReading = 0, currReading = 0, units = 0;
    double energy = 0, fixed = 0, surcharge = 0, duty = 0, total = 0;
    double lateFee = 0, paid = 0;

    double due() const { return total + lateFee - paid; }
    bool   isSettled() const { return due() < 0.005; }

    string serialize() const {
        ostringstream o;
        o << billId << "|" << custId << "|" << month << "|" << prevReading << "|"
          << currReading << "|" << units << "|" << energy << "|" << fixed << "|"
          << surcharge << "|" << duty << "|" << total << "|" << lateFee << "|" << paid;
        return o.str();
    }

    static bool parse(const string &ln, Bill &b) {
        vector<string> f = split(ln, '|');
        if (f.size() < 13) return false;
        try {
            b.billId = stoi(f[0]);  b.custId = stoi(f[1]);  b.month = f[2];
            b.prevReading = stoi(f[3]);  b.currReading = stoi(f[4]);  b.units = stoi(f[5]);
            b.energy = stod(f[6]);  b.fixed = stod(f[7]);  b.surcharge = stod(f[8]);
            b.duty = stod(f[9]);  b.total = stod(f[10]);  b.lateFee = stod(f[11]);
            b.paid = stod(f[12]);
        } catch (...) { return false; }
        return true;
    }
};

/* --------------------------- Billing system ------------------------ */

class BillingSystem {
    vector<unique_ptr<Customer>> customers;
    vector<Bill> bills;
    int nextCustId = 1001, nextBillId = 5001;

    Customer *findCustomer(int id) {
        for (auto &c : customers) if (c->getId() == id) return c.get();
        return nullptr;
    }
    Bill *findBill(int id) {
        for (auto &b : bills) if (b.billId == id) return &b;
        return nullptr;
    }

    void printInvoice(const Bill &b, const Customer &c) const {
        line('=');
        cout << "              POWERGRID ELECTRICITY INVOICE\n";
        line('=');
        cout << "Bill No   : " << b.billId << "          Month : " << b.month << "\n";
        cout << "Customer  : " << c.getName() << " (ID " << c.getId() << ")\n";
        cout << "Category  : " << c.category() << "      Phone : " << c.getPhone() << "\n";
        cout << "Address   : " << c.getAddress() << "\n";
        line();
        cout << "Previous reading : " << b.prevReading << "\n";
        cout << "Current reading  : " << b.currReading << "\n";
        cout << "Units consumed   : " << b.units << "\n";
        line();
        cout << fixed << setprecision(2);
        cout << left << setw(36) << "Energy charge (slab-wise)" << right << setw(12) << b.energy << "\n";
        cout << left << setw(36) << "Fixed charge"              << right << setw(12) << b.fixed << "\n";
        cout << left << setw(36) << "Fuel surcharge (4%)"       << right << setw(12) << b.surcharge << "\n";
        cout << left << setw(36) << "Electricity duty (6%)"     << right << setw(12) << b.duty << "\n";
        if (b.lateFee > 0)
            cout << left << setw(36) << "Late payment fee (2%)" << right << setw(12) << b.lateFee << "\n";
        line();
        cout << left << setw(36) << "TOTAL PAYABLE" << right << setw(12) << (b.total + b.lateFee) << "\n";
        cout << left << setw(36) << "Paid so far"   << right << setw(12) << b.paid << "\n";
        cout << left << setw(36) << "BALANCE DUE"   << right << setw(12) << b.due() << "\n";
        line('=');
        cout << "Status: " << (b.isSettled() ? "PAID" : "UNPAID") << "\n";
    }

public:
    /* ---------- persistence ---------- */
    void load() {
        ifstream cf(CUSTOMER_FILE);
        string ln;
        while (getline(cf, ln)) {
            vector<string> f = split(ln, '|');
            if (f.size() < 6) continue;
            try {
                auto c = makeCustomer(stoi(f[0]), stoi(f[1]), f[2], f[3], f[4], stoi(f[5]));
                if (c) { nextCustId = max(nextCustId, c->getId() + 1); customers.push_back(move(c)); }
            } catch (...) {}
        }
        ifstream bf(BILL_FILE);
        while (getline(bf, ln)) {
            Bill b;
            if (Bill::parse(ln, b)) { nextBillId = max(nextBillId, b.billId + 1); bills.push_back(b); }
        }
    }

    void save() const {
        ofstream cf(CUSTOMER_FILE);
        for (const auto &c : customers) cf << c->serialize() << "\n";
        ofstream bf(BILL_FILE);
        bf << setprecision(10);
        for (const auto &b : bills) bf << b.serialize() << "\n";
    }

    /* ---------- customer operations ---------- */
    void addCustomer() {
        cout << "\n--- Register New Customer ---\n";
        string name = readText("Full name: ");
        string addr = readText("Address: ");
        string phone = readPhone();
        cout << "Category: 1) Domestic  2) Commercial  3) Industrial\n";
        int type = readInt("Choose (1-3): ", 1, 3);
        int reading = readInt("Initial meter reading: ", 0, 99999999);
        int id = nextCustId++;
        customers.push_back(makeCustomer(type, id, name, addr, phone, reading));
        save();
        cout << "Customer registered. Assigned ID: " << id << "\n";
    }

    void listCustomers() const {
        cout << "\n--- All Customers ---\n";
        if (customers.empty()) { cout << "No customers registered yet.\n"; return; }
        line();
        cout << left << setw(7) << "ID" << setw(22) << "Name" << setw(12) << "Category"
             << setw(12) << "Phone" << "Reading\n";
        line();
        for (const auto &c : customers)
            cout << left << setw(7) << c->getId() << setw(22) << c->getName().substr(0, 20)
                 << setw(12) << c->category() << setw(12) << c->getPhone()
                 << c->getLastReading() << "\n";
        line();
    }

    void searchCustomer() const {
        string key = toLower(readLine("Search by name or phone: "));
        bool found = false;
        for (const auto &c : customers) {
            if (toLower(c->getName()).find(key) != string::npos ||
                c->getPhone().find(key) != string::npos) {
                cout << "  [" << c->getId() << "] " << c->getName() << " | "
                     << c->category() << " | " << c->getAddress() << " | " << c->getPhone() << "\n";
                found = true;
            }
        }
        if (!found) cout << "No matching customer.\n";
    }

    void deleteCustomer() {
        int id = readInt("Customer ID to remove: ", 1, INT_MAX);
        Customer *c = findCustomer(id);
        if (!c) { cout << "Customer not found.\n"; return; }
        for (const auto &b : bills)
            if (b.custId == id && !b.isSettled()) {
                cout << "Cannot remove: customer has unpaid bills.\n";
                return;
            }
        string ans = toLower(readLine("Remove " + c->getName() + " and their bill history? (y/n): "));
        if (ans != "y") { cout << "Cancelled.\n"; return; }
        bills.erase(remove_if(bills.begin(), bills.end(),
                    [id](const Bill &b) { return b.custId == id; }), bills.end());
        customers.erase(remove_if(customers.begin(), customers.end(),
                    [id](const unique_ptr<Customer> &p) { return p->getId() == id; }), customers.end());
        save();
        cout << "Customer removed.\n";
    }

    /* ---------- billing operations ---------- */
    void generateBill() {
        cout << "\n--- Generate Bill ---\n";
        int id = readInt("Customer ID: ", 1, INT_MAX);
        Customer *c = findCustomer(id);
        if (!c) { cout << "Customer not found.\n"; return; }

        string month = readMonth();
        for (const auto &b : bills)
            if (b.custId == id && b.month == month) {
                cout << "A bill for " << month << " already exists (Bill " << b.billId << ").\n";
                return;
            }

        cout << "Previous reading on record: " << c->getLastReading() << "\n";
        int curr = readInt("Current meter reading: ", 0, 99999999);
        if (curr < c->getLastReading()) {
            cout << "Current reading cannot be lower than the previous one.\n";
            return;
        }

        Bill b;
        b.billId = nextBillId++;
        b.custId = id;
        b.month = month;
        b.prevReading = c->getLastReading();
        b.currReading = curr;
        b.units = curr - b.prevReading;
        b.energy = c->energyCharge(b.units);
        b.fixed = c->fixedCharge();
        b.surcharge = b.energy * FUEL_SURCHARGE_RATE;
        b.duty = (b.energy + b.fixed + b.surcharge) * ELECTRICITY_DUTY;
        b.total = b.energy + b.fixed + b.surcharge + b.duty;

        c->setLastReading(curr);
        bills.push_back(b);
        save();
        printInvoice(b, *c);
    }

    void viewCustomerBills() {
        int id = readInt("Customer ID: ", 1, INT_MAX);
        Customer *c = findCustomer(id);
        if (!c) { cout << "Customer not found.\n"; return; }
        cout << "\nBill history for " << c->getName() << "\n";
        line();
        cout << left << setw(8) << "Bill" << setw(10) << "Month" << setw(8) << "Units"
             << setw(13) << "Total" << setw(13) << "Due" << "Status\n";
        line();
        bool any = false;
        cout << fixed << setprecision(2);
        for (const auto &b : bills) {
            if (b.custId != id) continue;
            any = true;
            cout << left << setw(8) << b.billId << setw(10) << b.month << setw(8) << b.units
                 << setw(13) << (b.total + b.lateFee) << setw(13) << b.due()
                 << (b.isSettled() ? "PAID" : "UNPAID") << "\n";
        }
        if (!any) cout << "No bills generated yet.\n";
        line();
    }

    void showInvoice() {
        int bid = readInt("Bill number: ", 1, INT_MAX);
        Bill *b = findBill(bid);
        if (!b) { cout << "Bill not found.\n"; return; }
        Customer *c = findCustomer(b->custId);
        if (c) printInvoice(*b, *c);
    }

    void payBill() {
        cout << "\n--- Accept Payment ---\n";
        int bid = readInt("Bill number: ", 1, INT_MAX);
        Bill *b = findBill(bid);
        if (!b) { cout << "Bill not found.\n"; return; }
        if (b->isSettled()) { cout << "This bill is already fully paid.\n"; return; }

        if (b->lateFee == 0 &&
            toLower(readLine("Is the payment after the due date? (y/n): ")) == "y") {
            b->lateFee = b->total * LATE_FEE_RATE;
            cout << fixed << setprecision(2) << "Late fee of Rs. " << b->lateFee << " added.\n";
        }

        cout << fixed << setprecision(2) << "Amount due: Rs. " << b->due() << "\n";
        double amt = readDouble("Amount received: Rs. ", 0.01, b->due() + 0.005);
        double applied = min(amt, b->due());
        b->paid += applied;
        save();

        Customer *c = findCustomer(b->custId);
        cout << "\n";
        line('*');
        cout << "                  PAYMENT RECEIPT\n";
        line('*');
        cout << "Date      : " << nowString() << "\n";
        cout << "Bill No   : " << b->billId << "  (" << b->month << ")\n";
        if (c) cout << "Customer  : " << c->getName() << " (ID " << c->getId() << ")\n";
        cout << "Received  : Rs. " << applied << "\n";
        cout << "Balance   : Rs. " << max(0.0, b->due()) << "\n";
        cout << "Status    : " << (b->isSettled() ? "FULLY PAID" : "PARTIALLY PAID") << "\n";
        line('*');
    }

    void outstandingDues() const {
        cout << "\n--- Outstanding Dues ---\n";
        double sum = 0;
        bool any = false;
        cout << fixed << setprecision(2);
        for (const auto &b : bills) {
            if (b.isSettled()) continue;
            any = true;
            string nm = "?";
            for (const auto &c : customers) if (c->getId() == b.custId) nm = c->getName();
            cout << "Bill " << b.billId << " | " << setw(18) << left << nm.substr(0, 18)
                 << " | " << b.month << " | Rs. " << b.due() << "\n";
            sum += b.due();
        }
        if (!any) cout << "No pending dues. \n";
        else cout << "Total outstanding: Rs. " << sum << "\n";
    }

    void summaryReport() const {
        int cat[4] = {0, 0, 0, 0};
        for (const auto &c : customers) cat[c->typeCode()]++;
        long long units = 0;
        double billed = 0, collected = 0, pending = 0;
        for (const auto &b : bills) {
            units += b.units;
            billed += b.total + b.lateFee;
            collected += b.paid;
            pending += max(0.0, b.due());
        }
        cout << "\n=========== SYSTEM SUMMARY ===========\n";
        cout << "Customers : " << customers.size() << "  (Domestic " << cat[1]
             << ", Commercial " << cat[2] << ", Industrial " << cat[3] << ")\n";
        cout << "Bills     : " << bills.size() << "\n";
        cout << "Units sold: " << units << "\n";
        cout << fixed << setprecision(2);
        cout << "Billed    : Rs. " << billed << "\n";
        cout << "Collected : Rs. " << collected << "\n";
        cout << "Pending   : Rs. " << pending << "\n";
        cout << "======================================\n";
    }

    void showTariff() const {
        cout << "\n--- Tariff Chart (Rs. per unit) ---\n";
        DomesticCustomer d(0, "", "", ""); CommercialCustomer c(0, "", "", ""); IndustrialCustomer i(0, "", "", "");
        const Customer *all[3] = {&d, &c, &i};
        cout << fixed << setprecision(2);
        for (const Customer *p : all) {
            cout << p->category() << " (fixed Rs. " << p->fixedCharge() << "):\n";
            int prev = 0;
            for (const Slab &s : p->slabs()) {
                if (s.upTo == INT_MAX) cout << "   above " << prev << " units : " << s.rate << "\n";
                else cout << "   " << prev + 1 << " - " << s.upTo << " units : " << s.rate << "\n";
                prev = s.upTo;
            }
        }
        cout << "Fuel surcharge 4% | Electricity duty 6% | Late fee 2%\n";
    }
};

/* -------------------------------- main ----------------------------- */

void banner() {
    line('=');
    cout << "      POWERGRID - ELECTRICITY BILLING SYSTEM\n";
    cout << "  Team: Hemant Gurjar, Hemant Khandelwal,\n";
    cout << "        Himanshi Pancholi, Harshpal Singh\n";
    line('=');
}

int main() {
    BillingSystem sys;
    sys.load();
    banner();

    while (true) {
        cout << "\n 1. Register customer        7. View invoice\n"
                " 2. List customers           8. Accept payment\n"
                " 3. Search customer          9. Outstanding dues\n"
                " 4. Remove customer         10. Summary report\n"
                " 5. Generate bill           11. Tariff chart\n"
                " 6. Customer bill history    0. Exit\n";
        int ch = readInt("Enter choice: ", 0, 11);
        switch (ch) {
            case 1:  sys.addCustomer();       break;
            case 2:  sys.listCustomers();     break;
            case 3:  sys.searchCustomer();    break;
            case 4:  sys.deleteCustomer();    break;
            case 5:  sys.generateBill();      break;
            case 6:  sys.viewCustomerBills(); break;
            case 7:  sys.showInvoice();       break;
            case 8:  sys.payBill();           break;
            case 9:  sys.outstandingDues();   break;
            case 10: sys.summaryReport();     break;
            case 11: sys.showTariff();        break;
            case 0:
                sys.save();
                cout << "Data saved. Thank you!\n";
                return 0;
        }
    }
}
