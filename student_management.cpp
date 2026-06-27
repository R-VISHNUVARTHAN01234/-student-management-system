#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
using namespace std;

// ─────────────────────────────────────────
//  Student Structure
// ─────────────────────────────────────────
struct Student {
    int    id;
    string name;
    int    age;
    string department;
    float  cgpa;
};

// ─────────────────────────────────────────
//  Constants
// ─────────────────────────────────────────
const string FILENAME = "students.txt";

// ─────────────────────────────────────────
//  Utility: Print a separator line
// ─────────────────────────────────────────
void printLine() {
    cout << string(60, '-') << endl;
}

// ─────────────────────────────────────────
//  Display a single student record
// ─────────────────────────────────────────
void displayStudent(const Student& s) {
    cout << left
         << setw(6)  << s.id
         << setw(20) << s.name
         << setw(5)  << s.age
         << setw(15) << s.department
         << fixed << setprecision(2) << s.cgpa
         << endl;
}

// ─────────────────────────────────────────
//  1. Add a new student
// ─────────────────────────────────────────
void addStudent() {
    Student s;
    cout << "\n--- Add New Student ---\n";
    cout << "Enter Student ID     : "; cin >> s.id;
    cin.ignore();
    cout << "Enter Name           : "; getline(cin, s.name);
    cout << "Enter Age            : "; cin >> s.age;
    cin.ignore();
    cout << "Enter Department     : "; getline(cin, s.department);
    cout << "Enter CGPA           : "; cin >> s.cgpa;

    // Check for duplicate ID
    ifstream fin(FILENAME);
    Student tmp;
    while (fin >> tmp.id) {
        fin.ignore();
        getline(fin, tmp.name);
        fin >> tmp.age;
        fin.ignore();
        getline(fin, tmp.department);
        fin >> tmp.cgpa;
        fin.ignore();
        if (tmp.id == s.id) {
            cout << "\n[ERROR] Student with ID " << s.id << " already exists!\n";
            fin.close();
            return;
        }
    }
    fin.close();

    ofstream fout(FILENAME, ios::app);
    if (!fout) { cout << "[ERROR] Cannot open file!\n"; return; }

    fout << s.id        << "\n"
         << s.name       << "\n"
         << s.age        << "\n"
         << s.department << "\n"
         << s.cgpa       << "\n";
    fout.close();
    cout << "\n[SUCCESS] Student added successfully!\n";
}

// ─────────────────────────────────────────
//  2. Display all students
// ─────────────────────────────────────────
void displayAll() {
    ifstream fin(FILENAME);
    if (!fin) { cout << "\n[INFO] No records found.\n"; return; }

    cout << "\n--- All Students ---\n";
    printLine();
    cout << left
         << setw(6)  << "ID"
         << setw(20) << "Name"
         << setw(5)  << "Age"
         << setw(15) << "Dept"
         << "CGPA" << endl;
    printLine();

    Student s;
    int count = 0;
    while (fin >> s.id) {
        fin.ignore();
        getline(fin, s.name);
        fin >> s.age;
        fin.ignore();
        getline(fin, s.department);
        fin >> s.cgpa;
        fin.ignore();
        displayStudent(s);
        count++;
    }
    fin.close();
    printLine();
    cout << "Total students: " << count << endl;
}

// ─────────────────────────────────────────
//  3. Search by ID
// ─────────────────────────────────────────
void searchStudent() {
    int searchId;
    cout << "\n--- Search Student ---\n";
    cout << "Enter Student ID to search: "; cin >> searchId;

    ifstream fin(FILENAME);
    if (!fin) { cout << "\n[INFO] No records found.\n"; return; }

    Student s;
    bool found = false;
    while (fin >> s.id) {
        fin.ignore();
        getline(fin, s.name);
        fin >> s.age;
        fin.ignore();
        getline(fin, s.department);
        fin >> s.cgpa;
        fin.ignore();
        if (s.id == searchId) {
            cout << "\n[FOUND]\n";
            printLine();
            cout << "ID         : " << s.id         << "\n"
                 << "Name       : " << s.name        << "\n"
                 << "Age        : " << s.age         << "\n"
                 << "Department : " << s.department  << "\n"
                 << "CGPA       : " << fixed << setprecision(2) << s.cgpa << "\n";
            printLine();
            found = true;
            break;
        }
    }
    fin.close();
    if (!found) cout << "\n[INFO] Student with ID " << searchId << " not found.\n";
}

// ─────────────────────────────────────────
//  4. Update student record
// ─────────────────────────────────────────
void updateStudent() {
    int updateId;
    cout << "\n--- Update Student ---\n";
    cout << "Enter Student ID to update: "; cin >> updateId;

    ifstream fin(FILENAME);
    if (!fin) { cout << "\n[INFO] No records found.\n"; return; }

    // Read all records into a temporary file, replace matched one
    ofstream fout("temp.txt");
    Student s;
    bool found = false;

    while (fin >> s.id) {
        fin.ignore();
        getline(fin, s.name);
        fin >> s.age;
        fin.ignore();
        getline(fin, s.department);
        fin >> s.cgpa;
        fin.ignore();

        if (s.id == updateId) {
            found = true;
            cout << "\nCurrent record:\n";
            displayStudent(s);
            cout << "\nEnter new details:\n";
            cin.ignore();
            cout << "New Name       : "; getline(cin, s.name);
            cout << "New Age        : "; cin >> s.age;
            cin.ignore();
            cout << "New Department : "; getline(cin, s.department);
            cout << "New CGPA       : "; cin >> s.cgpa;
        }
        fout << s.id        << "\n"
             << s.name       << "\n"
             << s.age        << "\n"
             << s.department << "\n"
             << s.cgpa       << "\n";
    }
    fin.close();
    fout.close();

    remove(FILENAME.c_str());
    rename("temp.txt", FILENAME.c_str());

    if (found)  cout << "\n[SUCCESS] Record updated!\n";
    else        cout << "\n[INFO] Student ID " << updateId << " not found.\n";
}

// ─────────────────────────────────────────
//  5. Delete student record
// ─────────────────────────────────────────
void deleteStudent() {
    int deleteId;
    cout << "\n--- Delete Student ---\n";
    cout << "Enter Student ID to delete: "; cin >> deleteId;

    ifstream fin(FILENAME);
    if (!fin) { cout << "\n[INFO] No records found.\n"; return; }

    ofstream fout("temp.txt");
    Student s;
    bool found = false;

    while (fin >> s.id) {
        fin.ignore();
        getline(fin, s.name);
        fin >> s.age;
        fin.ignore();
        getline(fin, s.department);
        fin >> s.cgpa;
        fin.ignore();

        if (s.id == deleteId) {
            found = true;   // skip writing this record
            cout << "\n[INFO] Deleting: "; displayStudent(s);
        } else {
            fout << s.id        << "\n"
                 << s.name       << "\n"
                 << s.age        << "\n"
                 << s.department << "\n"
                 << s.cgpa       << "\n";
        }
    }
    fin.close();
    fout.close();

    remove(FILENAME.c_str());
    rename("temp.txt", FILENAME.c_str());

    if (found)  cout << "[SUCCESS] Student deleted!\n";
    else        cout << "\n[INFO] Student ID " << deleteId << " not found.\n";
}

// ─────────────────────────────────────────
//  6. Show statistics
// ─────────────────────────────────────────
void showStats() {
    ifstream fin(FILENAME);
    if (!fin) { cout << "\n[INFO] No records found.\n"; return; }

    Student s;
    int count = 0;
    float total = 0, maxCgpa = -1, minCgpa = 11;
    string topStudent, lowStudent;

    while (fin >> s.id) {
        fin.ignore();
        getline(fin, s.name);
        fin >> s.age;
        fin.ignore();
        getline(fin, s.department);
        fin >> s.cgpa;
        fin.ignore();

        count++;
        total += s.cgpa;
        if (s.cgpa > maxCgpa) { maxCgpa = s.cgpa; topStudent = s.name; }
        if (s.cgpa < minCgpa) { minCgpa = s.cgpa; lowStudent = s.name; }
    }
    fin.close();

    if (count == 0) { cout << "\n[INFO] No records to analyse.\n"; return; }

    cout << "\n--- Statistics ---\n";
    printLine();
    cout << fixed << setprecision(2);
    cout << "Total Students  : " << count               << "\n"
         << "Average CGPA    : " << (total / count)     << "\n"
         << "Highest CGPA    : " << maxCgpa << "  (" << topStudent << ")\n"
         << "Lowest  CGPA    : " << minCgpa << "  (" << lowStudent << ")\n";
    printLine();
}

// ─────────────────────────────────────────
//  Main Menu
// ─────────────────────────────────────────
void showMenu() {
    cout << "\n";
    printLine();
    cout << "       STUDENT MANAGEMENT SYSTEM\n";
    printLine();
    cout << "  1. Add Student\n"
         << "  2. Display All Students\n"
         << "  3. Search Student by ID\n"
         << "  4. Update Student Record\n"
         << "  5. Delete Student Record\n"
         << "  6. Show Statistics\n"
         << "  7. Exit\n";
    printLine();
    cout << "  Enter your choice: ";
}

// ─────────────────────────────────────────
//  Entry Point
// ─────────────────────────────────────────
int main() {
    int choice;
    do {
        showMenu();
        cin >> choice;
        switch (choice) {
            case 1: addStudent();     break;
            case 2: displayAll();     break;
            case 3: searchStudent();  break;
            case 4: updateStudent();  break;
            case 5: deleteStudent();  break;
            case 6: showStats();      break;
            case 7: cout << "\nGoodbye!\n"; break;
            default: cout << "\n[ERROR] Invalid choice. Try again.\n";
        }
    } while (choice != 7);

    return 0;
}
