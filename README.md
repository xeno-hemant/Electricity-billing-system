# ⚡ Electricity Billing System

A beginner-friendly **C++ console project** that manages electricity consumers, calculates bills from meter readings, accepts payments and prints receipts. The project is divided into **4 modules**, one for each team member, and saves data in a text file.

## 👥 Team Members & Module Distribution

| Module | Developer | File | Responsibility |
|---|---|---|---|
| **Module 1** | Hemant Gurjar | `module1_consumer.cpp` | Consumer management - add, show, search, delete consumers |
| **Module 2** | Hemant Khandelwal | `module2_billing.cpp` | Billing - tariff slabs, bill calculation, generate / view bill, tariff chart |
| **Module 3** | Himanshi Pancholi | `module3_payment.cpp` | Payment - accept full/partial payment, receipt, pending dues |
| **Module 4** | Harshpal Singh | `module4_storage_main.cpp` | File handling (save/load), summary report, main menu |

`common.h` is the shared header: it holds the `Consumer` class declaration and the list of functions of every module.

## 🧩 How the Modules Work Together

```
                 module4_storage_main.cpp
                 (main menu – calls everyone)
                           |
      +--------------------+--------------------+
      |                    |                    |
 Module 1             Module 2             Module 3
 Consumers            Billing              Payment
      \                    |                    /
       +------- common.h (Consumer class) -----+
                           |
                 Module 4 : save / load file
```

The `Consumer` class is declared once in `common.h`. Each module writes the body of its own member functions using `Consumer::functionName()`, so every member works on its own file without touching the others.

## ✨ Features

- Add, view, search and delete consumers (Domestic or Commercial)
- Generate monthly bill from the current meter reading
- Slab-wise unit charges + fixed charge + 5% tax
- Pay bill (full or partial) and print payment receipt
- View pending dues of all consumers
- Summary report (total billed, collected, pending)
- Tariff chart
- Data saved in `consumers.txt` and loaded automatically on the next run

## 📚 Concepts Used

| Concept | Where |
|---|---|
| Class and objects | `Consumer` class |
| Constructor | `Consumer()` in Module 1 |
| Encapsulation (private data, public functions) | `common.h` |
| Scope resolution `::` | member functions defined in different modules |
| Array of objects | `Consumer consumers[MAX]` |
| Functions, loops, `if-else`, `switch` | all modules |
| Multi-file program, header file, `extern` | `common.h` + 4 `.cpp` files |
| File handling | `ifstream`, `ofstream` in Module 4 |

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

**Example:** Domestic, 250 units → Energy = 100×3.5 + 100×5 + 50×7 = 1200, Fixed = 50, Tax = 62.5 → **Bill = Rs. 1312.50**

## 🚀 How to Compile & Run

All `.cpp` files must be compiled together:

```bash
g++ *.cpp -o billing
./billing            # Linux / macOS
billing.exe          # Windows
```

(Or compile each module separately: `g++ module1_consumer.cpp module2_billing.cpp module3_payment.cpp module4_storage_main.cpp -o billing`)

## 📋 Menu

```
 1. Add consumer          (Module 1)
 2. Show all consumers    (Module 1)
 3. Search consumer       (Module 1)
 4. Generate bill         (Module 2)
 5. View bill             (Module 2)
 6. Pay bill              (Module 3)
 7. Pending dues          (Module 3)
 8. Delete consumer       (Module 1)
 9. Tariff chart          (Module 2)
10. Summary report        (Module 4)
 0. Exit
```

## 🔄 Sample Use

1. Choose **1** → add consumer ID `101`, starting reading `500`.
2. Choose **4** → ID `101`, month `April 2026`, new reading `750` → bill is printed.
3. Choose **6** → ID `101`, enter amount → receipt is printed.
4. Choose **10** → see the summary report.

## ⚠️ Notes

- Consumer IDs must be unique numbers.
- The next bill can be generated only after the previous bill is fully paid.
- Only the latest bill of each consumer is stored (kept simple on purpose).
- Maximum 100 consumers (`MAX` in `common.h`).
- In the data file, spaces in names/addresses are stored as `_`.

## 📁 Project Structure

```
ElectricityBillingSystem/
├── common.h                    # shared class + function declarations
├── module1_consumer.cpp        # Module 1 – Hemant Gurjar
├── module2_billing.cpp         # Module 2 – Hemant Khandelwal
├── module3_payment.cpp         # Module 3 – Himanshi Pancholi
├── module4_storage_main.cpp    # Module 4 – Harshpal Singh
├── README.md
└── consumers.txt               # created automatically
```

---
*OOPs Lab Project by Hemant Gurjar, Hemant Khandelwal, Himanshi Pancholi and Harshpal Singh.*
