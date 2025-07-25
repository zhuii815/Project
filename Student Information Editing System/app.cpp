#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include "BST.h"
#include "Student.h"
#include <string>

using namespace std;

bool readFile(const char* filename, BST* t1);
int menu();

int main() {
    BST t1, t2;
    int choice;
    bool success;
    const char* filename = "student.txt";

    do {
        system("cls");

        choice = menu();

        switch (choice) {
        case 1: {
            // Read data to BST
            success = readFile(filename, &t1);
            if (success) {
                cout << "Data successfully read from file and stored in BST." << endl;
            }
            else {
                cout << "Failed to read data from file." << endl;
            }
            break;
        }
        case 2: {
            // Print deepest nodes
            cout << "Deepest nodes in the tree: ";
            success = t1.deepestNodes();
            if (!success) {
                cout << "Tree is empty." << endl;
            }
            break;
        }
        case 3: {
            // Display students
            int order, source;
            cout << "Enter order (1 for ascending, 2 for descending): ";
            cin >> order;
            cout << "Enter source (1 for screen, 2 for file): ";
            cin >> source;
            success = t1.display(order, source);
            if (!success) {
                cout << "Tree is empty." << endl;
            }
            break;
        }
        case 4: {
            // Clone Subtree
            if (t1.empty()) {
                cout << "Cannot clone from an empty tree." << endl;
                break;
            }
            int id;
            cout << "Enter student ID to clone subtree from: ";
            cin >> id;

            // Create a dummy student with the ID to search for
            Student target;
            target.id = id;

            // Clone the subtree
            success = t2.CloneSubtree(t1, target);
            if (success) {
                cout << "\nOriginal tree (pre-order):" << endl;
                t1.preOrderPrint();
                cout << "\nCloned subtree (pre-order):" << endl;
                t2.preOrderPrint();
            }
            break;
        }
        case 5: {
            // Print Level Nodes
            success = t1.printLevelNodes();
            if (!success) {
                cout << "Tree is empty." << endl;
            }
            break;
        }
        case 6: {
            // Print Path
            success = t1.printPath();
            if (!success) {
                cout << "Tree is empty." << endl;
            }
            break;
        }
        case 7: {
            // Exit
            cout << "Exiting program..." << endl;
            break;
        }
        default: {
            cout << "Invalid choice. Please try again." << endl;
        }
        }
        system("pause");
    } while (choice != 7);

    return 0;
}

bool readFile(const char* filename, BST* t1) {
    ifstream inFile(filename);
    if (!inFile.is_open()) {
        cout << "Unable to open file: " << filename << endl;
        return false;
    }

    int recordCount = 0;
    string line;

    while (getline(inFile, line)) {
        // Skip empty lines
        if (line.empty()) continue;

        Student s;

        // Read ID line (e.g., "Student Id = 28")
        size_t idPos = line.find("Student Id = ");
        if (idPos == string::npos) continue; // skip if format doesn't match
        s.id = stoi(line.substr(idPos + 13));

        // Read Name line
        getline(inFile, line);
        size_t namePos = line.find("Name = ");
        if (namePos == string::npos) continue;
        strncpy(s.name, line.substr(namePos + 7).c_str(), sizeof(s.name) - 1);
        s.name[sizeof(s.name) - 1] = '\0';

        // Read Address line
        getline(inFile, line);
        size_t addrPos = line.find("Address = ");
        if (addrPos == string::npos) continue;
        strncpy(s.address, line.substr(addrPos + 10).c_str(), sizeof(s.address) - 1);
        s.address[sizeof(s.address) - 1] = '\0';

        // Read DOB line
        getline(inFile, line);
        size_t dobPos = line.find("DOB = ");
        if (dobPos == string::npos) continue;
        strncpy(s.DOB, line.substr(dobPos + 6).c_str(), sizeof(s.DOB) - 1);
        s.DOB[sizeof(s.DOB) - 1] = '\0';

        // Read Phone Number line
        getline(inFile, line);
        size_t phonePos = line.find("Phone Number = ");
        if (phonePos == string::npos) continue;
        strncpy(s.phone_no, line.substr(phonePos + 14).c_str(), sizeof(s.phone_no) - 1);
        s.phone_no[sizeof(s.phone_no) - 1] = '\0';

        // Read Course line
        getline(inFile, line);
        size_t coursePos = line.find("Course = ");
        if (coursePos == string::npos) continue;
        strncpy(s.course, line.substr(coursePos + 9).c_str(), sizeof(s.course) - 1);
        s.course[sizeof(s.course) - 1] = '\0';

        // Read CGPA line
        getline(inFile, line);
        size_t cgpaPos = line.find("CGPA = ");
        if (cgpaPos == string::npos) continue;
        s.cgpa = stod(line.substr(cgpaPos + 7));

        // Skip empty line between records if exists
        while (inFile.peek() == '\n') inFile.ignore();

        // Check for duplicate ID
        if (t1->findDuplicateID(s.id)) {
            cout << "Duplicate ID found: " << s.id << ". Skipping this record." << endl;
            continue;
        }

        // Insert into BST
        if (t1->insert(s)) {
            recordCount++;
        }
        else {
            cout << "Failed to insert record with ID: " << s.id << endl;
        }
    }

    inFile.close();
    cout << "Successfully read " << recordCount << " student records." << endl;
    return true;
}

int menu() {	//getting input for choices of function from the user
	int choice;

	do {
        cout << "============================================\n";
        cout << " Binary Search Tree Operations Menu \n";
        cout << "============================================\n";
        cout << "(1) Read data to BST\n";
		cout << "(2) Print deepest nodes\n";
		cout << "(3) Display student\n";
		cout << "(4) Clone Subtree\n";
		cout << "(5) Print Level Nodes\n";
		cout << "(6) Print Path\n";
		cout << "(7) Exit\n";

		cout << "Enter your choice: ";
		cin >> choice;
		cin.ignore();

		if (choice < 1 || choice > 7) {
			cout << "\n\nInvalid option.Please enter a number between 1 and 7.\n";
			system("pause");
			system("cls");
		}
	} while (choice < 1 || choice > 7);//loop this function if the input is invalid

	return choice;
}

