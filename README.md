# ⚡ Electricity Billing System

A beginner-friendly **C++ console project** that manages electricity consumers, calculates bills from meter readings, accepts payments and prints receipts. It uses a simple menu-driven interface and saves data in a text file.

## 👥 Team Members

1. Hemant Gurjar
2. Hemant Khandelwal
3. Himanshi Pancholi
4. Harshpal Singh

## ✨ Features

- Add, view, search and delete consumers (Domestic or Commercial)
- Generate monthly bill from the current meter reading
- Slab-wise unit charges + fixed charge + 5% tax
- Pay bill (full or partial) and print payment receipt
- View pending dues of all consumers
- Tariff chart
- Data is saved in `consumers.txt` and loaded automatically on the next run

## 📚 Concepts Used

| Concept | Where |
|---|---|
| Class and objects | `Consumer` class stores one customer's data and bill |
| Constructor | `Consumer()` sets default values |
| Private data + public functions (encapsulation) | All data members are `private` |
| Array of objects | `Consumer consumers[MAX]` |
| Functions | `addConsumer()`, `generateBill()`, `payBill()` etc. |
| Loops | `for`, `do-while` (menu) |
| Conditions | `if-else`, `switch` |
| File handling | `ifstream` and `ofstream` |

## 💰 Tariff

| Category | Units | Rate (Rs./unit) | Fixed charge |
|---|---|---|---|
| Domestic | 1 – 100 | 3.50 | Rs. 50 |
| | 101 – 200 | 5.00 | |
| | Above 200 | 7.00 | |
| Commercial | 1 – 100 | 6.00 | Rs. 150 |
| | 101 – 300 | 8.00 | |
| | Above 300 | 10.00 | |

```
Total Bill = (Energy charge + Fixed charge) + 5% tax
```

**Example:** Domestic, 250 units
Energy = 100×3.5 + 100×5 + 50×7 = 1200, Fixed = 50, Tax = 62.5 → **Bill = Rs. 1312.50**

## 🚀 How to Run

```bash
g++ electricity_billing.cpp -o billing
./billing            # Linux / macOS
billing.exe          # Windows
```

## 📋 Menu

```
1. Add consumer          6. Pay bill
2. Show all consumers    7. Pending dues
3. Search consumer       8. Delete consumer
4. Generate bill         9. Tariff chart
5. View bill             0. Exit
```

## 🔄 Sample Use

1. Choose **1** and add a consumer with ID `101` and starting reading `500`.
2. Choose **4**, enter ID `101`, month `April 2026`, new reading `750` → bill is printed.
3. Choose **6**, enter ID `101` and the amount to pay → receipt is printed.

## ⚠️ Notes

- Consumer IDs must be unique numbers.
- The next bill can only be generated after the previous bill is fully paid.
- Only the latest bill of each consumer is stored (kept simple on purpose).
- Maximum 100 consumers (`MAX` constant, can be changed).

## 📁 Files

```
├── electricity_billing.cpp   # source code
├── README.md                 # documentation
└── consumers.txt             # created automatically when you save data
```

---
*OOPs Lab Project by Hemant Gurjar, Hemant Khandelwal, Himanshi Pancholi and Harshpal Singh.*
