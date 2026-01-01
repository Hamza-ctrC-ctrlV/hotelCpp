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
├── chambres.txt            # Persistent rooms storage
├── reservations.txt        # Persistent reservations storage
└── README.md               # Project documentation

---

## ▶ How to Run


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


