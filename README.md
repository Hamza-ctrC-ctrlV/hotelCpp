#Hotel Management System (C++ OOP)

#Description

#Features


Account Locking: Clients are automatically blocked after 5 failed login attempts to prevent unauthorized access.

Dynamic Room Management: * Supports multiple room types (Single, Double, Suite, Deluxe, Presidential, Royal Suite).

Calculates real-time pricing based on a base rate plus a percentage modifier.

Reservation System: * Automated duration calculation between two dates.

Tracks room occupancy and links reservations to specific client IDs.

Persistent Storage: * Automated data saving and loading using text files (clients.txt, chambres.txt, staff.txt, reservations.txt).

Uses a custom pipe-separated (|) serialization format.

#Technologies
Language: C++

Library Focus: fstream (File I/O), vector (Dynamic collections), algorithm, and sstream (Data parsing).

#Architecture Overview
The project follows a modular hierarchy:

Personne (Abstract Base): Inherited by Client and Staff.

Chambre (Abstract Base): Inherited by specific room types with unique base prices.

Hotel (Manager Class): The "Controller" that handles all logic, file processing, and data vectors.

#How to Run

Bash
g++ main.cpp -o hotel
Execute:
Bash
./hotel


# Hotel Management System (C++)

> A comprehensive C++ console application built on Object-Oriented Programming principles. It manages the lifecycle of hotel operations including secure client authentication, dynamic room pricing, and staff-level administrative controls.


---

## 📌 Overview

This project is a **C++ hotel management system** designed to manage clients, staff, and rooms while enforcing authentication rules and data persistence.  
It is structured for **readability, maintainability, and academic evaluation**.

The system separates responsibilities clearly and documents intent at every level (files, classes, and functions).

---

## ✨ Features

- Client authentication using **CIN + password**
- **Account lock** after 5 failed login attempts
- Staff-only permissions to modify client data
- Persistent storage using human-readable text files
- Modular C++ design (classes, inheritance, polymorphism)
- Fully documented using industry-style comments

---

## 🗂 Project Structure

.
├── main.cpp                # Application entry point
├── clients.txt             # Persistent client storage
├── staff.txt               # Persistent staff storage
├── chambres.txt             # Persistent rooms storage
├── reservations.txt         # Persistent reservations storage
└── README.md               # Project documentation


## ▶ How to Run

### 1️⃣ Prerequisites

Make sure you have the following installed on your system:

- **GCC / G++** (version supporting C++17 or later)
- A terminal (Linux, macOS, or Windows with MinGW / WSL)

To verify your compiler version:

```bash
g++ --version

# Compile the Project
- g++ main.cpp -o hotel

# Run the Application
- ./hotel


