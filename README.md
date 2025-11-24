Hospital Management System – C Program

A simple console-based Hospital Management System developed in C.
This system helps manage patient records, including adding new patients, viewing all records, searching, updating, deleting, and generating hospital bills.

📌 Features
1. Add New Patient

Automatically generates a unique Patient ID

Stores name, age, gender, phone, address

Stores disease/complaint, doctor assigned

Calculates hospital bill based on stay duration

2. View All Patients

Displays all patient records stored in hospital.dat

Shows billing amount, personal details, and doctor information

3. Search Patient

Search by Patient ID

Displays full patient profile

4. Update Patient Record

Edit any existing details

Auto-recalculates bill

5. Delete Patient

Removes selected patient record safely

Creates a temporary file to avoid data loss

6. File Storage

Uses binary file (hospital.dat)

Ensures data remains saved after program exits

🛠️ Technologies Used

C Programming Language

File Handling (Binary)

Standard I/O Library

📂 File Structure
/HospitalManagement
│
├── hospital_system.c      # Main source code
├── hospital.dat           # Auto-created binary data file
└── README.md              # Project documentation

⚙️ How to Compile
Linux / macOS
gcc -o hospital_system hospital_system.c

Windows (MinGW)
gcc -o hospital_system.exe hospital_system.c

▶️ How to Run
Linux / macOS
./hospital_system

Windows
hospital_system.exe

📘 How Billing Works

A simple billing formula is used:

Total Bill = Base Fee + (Number of Days × Daily Charge)
Base Fee = 500
Daily Charge = 1500


You can modify these values inside the code as needed.

🚀 Future Enhancements

You may add:

Admin login system

Sorting patient records

Appointment management

Export to CSV or PDF

GUI using C++/GTK or Python interface 

![image alt](https://github.com/250301120332-oss/hospital-management/blob/372395f9e25dc3135d25b788e99c4d66d898bba5/output.jpg)

