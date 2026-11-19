# Electricity Billing System

A menu-driven **C++ console application** that handles customer registration, slab-based electricity billing, payment collection (full or partial), late fees, and reporting. Built as an **Object-Oriented Programming (OOPs) Lab** project.

## 👥 Team Members

| # | Name |
|---|------|
| 1 | Hemant Gurjar |
| 2 | Hemant Khandelwal |
| 3 | Himanshi Pancholi |
| 4 | Harshpal Singh |

## ✨ Features

- Register customers in three categories: **Domestic, Commercial, Industrial**
- Search customers by name or phone; list and remove customers
- Generate monthly bills from meter readings (validates readings, blocks duplicate months)
- **Slab-wise tariff** that differs per category, plus fixed charge, fuel surcharge and electricity duty
- Detailed itemised invoice
- Accept **full or partial payments**, optional **late fee**, printable payment receipt
- Outstanding dues list and overall summary report
- Built-in tariff chart
- **Data persistence** using text files (`customers.txt`, `bills.txt`) – data is kept between runs

## 🧠 OOP Concepts Used

| Concept | Where it is used |
|---|---|
| **Abstraction** | `Customer` is an abstract base class with pure virtual functions (`category()`, `fixedCharge()`, `slabs()`) |
| **Inheritance** | `DomesticCustomer`, `CommercialCustomer`, `IndustrialCustomer` derive from `Customer` |
| **Polymorphism** | The same `energyCharge()` call uses a different tariff depending on the customer's actual class (virtual dispatch) |
| **Encapsulation** | Customer data is `protected`; access is through getters/setters. `BillingSystem` hides all storage details |
| **Factory function** | `makeCustomer()` builds the right derived object from a type code (also used when loading files) |
| **STL & smart pointers** | `vector<unique_ptr<Customer>>`, `vector<Bill>`, `remove_if` |
| **File handling** | `ifstream` / `ofstream` with a `|`-delimited format |

## 💰 Tariff (Rs. per unit)

| Category | Slabs | Fixed charge |
|---|---|---|
| Domestic | 1–100: 3.50 · 101–200: 5.00 · 201–400: 7.00 · 400+: 9.00 | Rs. 40 |
| Commercial | 1–200: 6.50 · 201–500: 8.00 · 500+: 10.00 | Rs. 150 |
| Industrial | 1–1000: 7.50 · 1000+: 9.50 | Rs. 500 |

**Bill formula**

```
Energy charge   = slab-wise sum of (units in slab × rate)
Fuel surcharge  = 4% of energy charge
Duty            = 6% of (energy + fixed + surcharge)
Total           = energy + fixed + surcharge + duty
Late fee        = 2% of total (only if paid after due date)
```

*Example:* Domestic, 250 units → energy = 100×3.5 + 100×5 + 50×7 = Rs. 1200.

## 🚀 How to Compile & Run

```bash
g++ -std=c++11 electricity_billing.cpp -o billing
./billing          # Linux / macOS
billing.exe        # Windows
```

## 📋 Menu

```
 1. Register customer        7. View invoice
 2. List customers           8. Accept payment
 3. Search customer          9. Outstanding dues
 4. Remove customer         10. Summary report
 5. Generate bill           11. Tariff chart
 6. Customer bill history    0. Exit
```

## 🔄 Typical Workflow

1. Register a customer (option 1) – note the generated Customer ID.
2. Generate a bill with the new meter reading (option 5).
3. Accept payment against the Bill number (option 8).
4. Check dues or the summary report (options 9 / 10).

## 📁 Project Structure

```
├── electricity_billing.cpp   # complete source code
├── README.md                 # project documentation
├── customers.txt             # auto-created on first save
└── bills.txt                 # auto-created on first save
```

## 🔮 Future Improvements

- Admin login and password protection
- Meter-reading history graphs
- Monthly bill PDF export
- GUI front-end (Qt)

---
*Developed by Hemant Gurjar, Hemant Khandelwal, Himanshi Pancholi and Harshpal Singh – OOPs Lab Project.*
