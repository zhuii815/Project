#include <iostream>
#include <string>
#include <fstream>
#include <iomanip>
#include <cmath>
#include <cctype>
using namespace std;

struct Student {
    string studentId;
    string name;
    int form = 0;
    int marks[7];
};

const int MAX_STUDENTS = 100;
const string subjects[] = { "BM", "BI", "BC", "Math", "Sci", "Sejarah", "Geo" };
Student students[MAX_STUDENTS];
int studentCount = 0;

// Function prototypes
void saveToFile();
void readFile();
int getValidScore(const string& subject);
void addRecords();
double calculateAverage(int scores[], int n);
double calculateStandardDeviation(int scores[], int n, double average);
void displayStatistics(int form);
void displayResults();
void filter(int form, const string filterSubjects[], int subjectCount);
void search(const string& keyword);
void filterAndSearch();
void showMenu();
void displayStudentList();
void editStudent();
void deleteStudent();
void editAndDelete();

int main() {
    readFile();
    showMenu();
    return 0;
}

// Function to save student data to a file
void saveToFile() {
    ofstream myfile("student.txt");

    if (!myfile) {
        cout << "Error opening file for writing.\n";
        return;
    }

    for (int i = 0; i < studentCount; i++) {
        myfile << students[i].studentId << "\n";
        myfile << students[i].name << "\n";
        myfile << students[i].form << "\n";
        for (int j = 0; j < 7; j++) {
            myfile << students[i].marks[j] << " ";
        }
        myfile << "\n";
    }

    myfile.close();
    cout << "Data saved to student.txt successfully.\n";
}

// Function to read student data from a file
void readFile() {
    ifstream file("student.txt");

    if (file.fail()) {
        cout << "Error in opening file" << endl;
    }
    else {
        while (file >> students[studentCount].studentId) {
            file.ignore();
            getline(file, students[studentCount].name);
            file >> students[studentCount].form;
            for (int i = 0; i < 7; i++) {
                file >> students[studentCount].marks[i];
            }
            studentCount++;
        }
    }
    file.close();
}

// Function to get a valid score between 0 to 100
int getValidScore(const string& subject) {
    int score;
    while (true) {
        cout << subject << ": ";
        cin >> score;
        if (score >= 0 && score <= 100) {
            break;
        }
        else {
            cout << "Invalid number. Please enter a score between 0 and 100.\n";
        }
    }
    return score;
}

// Function to add student records
void addRecords() {
    char addMore;
    do {
        if (studentCount >= MAX_STUDENTS) {
            cout << "Cannot add more students. Maximum capacity reached.\n";
            return;
        }

        Student s;
        cout << "-------------------------------------------" << endl;
        cout << "Please enter student information and marks." << endl;
        cout << "-------------------------------------------" << endl;
        cout << "Student ID: ";
        cin >> s.studentId;
        cout << "Student Name: ";
        cin.ignore();
        getline(cin, s.name);
        cout << "Form: ";
        cin >> s.form;

        // Get valid scores for each subject
        for (int i = 0; i < 7; i++) {
            s.marks[i] = getValidScore(subjects[i]);
        }

        students[studentCount] = s;
        studentCount++;

        cout << "Do you want to key in another record? (Y/N): ";
        cin >> addMore;

    } while (addMore == 'Y' || addMore == 'y');

    saveToFile();
}

// Function to calculate average score
double calculateAverage(int scores[], int n) {
    double sum = 0;
    for (int i = 0; i < n; i++) {
        sum += scores[i];
    }
    return sum / n;
}

// Function to calculate standard deviation
double calculateStandardDeviation(int scores[], int n, double average) {
    double sumOfSquares = 0;
    for (int i = 0; i < n; i++) {
        double diff = scores[i] - average;
        sumOfSquares += diff * diff;
    }
    double variance = sumOfSquares / n;
    return sqrt(variance);
}

// Function to display statistics for a specific form
void displayStatistics(int form) {
    const int SUBJECT_COUNT = 7;
    int scores[MAX_STUDENTS];

    for (int subj = 0; subj < SUBJECT_COUNT; subj++) {
        int count = 0;
        int highestScore = -1;
        string topStudent;

        // Display results for the selected form or all forms
        for (int i = 0; i < studentCount; i++) {
            if (students[i].form == form || form == 0) {
                int score = students[i].marks[subj];
                scores[count++] = score;
                if (score > highestScore) {
                    highestScore = score;
                    topStudent = students[i].name;
                }
            }
        }

        if (count > 0) {
            double average = calculateAverage(scores, count);
            double stdDev = calculateStandardDeviation(scores, count, average);

            cout << fixed << setprecision(2) << endl;
            cout << subjects[subj] << "\n";
            cout << "Average: " << average << "\n";
            cout << "Standard deviation: " << stdDev << "\n";
            cout << "Highest Score: " << highestScore << "\n";
            cout << "Student Name: " << topStudent << "\n";
            cout << "--------------------------\n";
        }
    }
}

// Function to display results for a selected form
void displayResults() {
    if (studentCount == 0) {
        cout << "No records available.\n";
        return;
    }

    string choice;
    int formChoice = -1;

    // Validation loop to ensure valid form choice
    while (true) {
        cout << "Please choose the form that you wish to view [All/1/2/3]: ";
        cin >> choice;

        if (choice == "All" || choice == "all") {
            formChoice = 0;  // 0 will represent "All"
            break;
        }
        else if (choice == "1" || choice == "2" || choice == "3") {
            formChoice = stoi(choice);
            break;
        }
        else {
            cout << "Invalid form choice! Please enter a valid form [All/1/2/3].\n";
        }
    }

    // Display the results for the selected form
    cout << "Form " << (formChoice == 0 ? "All" : to_string(formChoice)) << ":\n";
    displayStatistics(formChoice);

    cout << "\nPress Enter to return to the main menu...";
    cin.ignore();
    cin.get();
}

//Function of filter for selected record
void filter(int form, const string filterSubjects[], int subjectCount) {
    for (int i = 0; i < studentCount; i++) {
        if (students[i].form == form) {
            cout << "Student ID: " << students[i].studentId << endl;
            cout << "Student name: " << students[i].name << endl;
            cout << "Form: " << students[i].form << endl;

            for (int j = 0; j < subjectCount; j++) {
                bool subjectFound = false; //Check the subject is valid or not
                for (int k = 0; k < 7; k++) {
                    if (subjects[k] == filterSubjects[j]) {
                        cout << subjects[k] << ": " << students[i].marks[k] << endl;
                        subjectFound = true;
                        break;
                    }
                } if (!subjectFound) {
                    cout << "Invalid subject: " << filterSubjects[j] << endl;
                }
            }
            cout << "\n";
        }
    }
}

//Fucntion of search for selected record
void search(const string& keyword) {
    bool found = false; //To check if result was found

    for (int i = 0; i < studentCount; i++) {
        if (students[i].name.find(keyword) != string::npos) {
            //.find(keyword) function search for the name that user input
            //string::npos represent the value when the keyword was not found

            found = true;
            cout << "Search Results:" << endl;
            cout << "Student ID: " << students[i].studentId << endl;
            cout << "Student name: " << students[i].name << endl;
            cout << "Form: " << students[i].form << endl;

            for (int j = 0; j < 7; j++) {
                cout << subjects[j] << ": " << students[i].marks[j] << endl;
            }
            cout << "\n";
        }
    }

    if (!found) {
        //If the keyword name was not found from the file
        cout << "Search Results: " << endl;
        cout << "Not found! Please try again." << endl;
    }
}

//Function of filter and search for selected record in menu
void filterAndSearch() {
    int choices; //For user to enter 1 or 2
    char cont; //For user to enter (Y-Yes) to continue or (N-No) to exit
    string input;

    do {
        cout << "---------------------------" << endl;
        cout << "Please select your choice: " << endl;
        cout << "    1. Filter" << endl;
        cout << "    2. Search" << endl;
        cout << "---------------------------" << endl;
        cout << "\nChoice: ";

        if (cin >> choices && (choices == 1 || choices == 2)) {
            cin.ignore();

            if (choices == 1) {
                int form;
                cout << "Please key in the information that you want to filter." << endl;
                cout << "Form: ";

                if (cin >> form && (form >= 1 && form <= 3)) {
                    cin.ignore();

                    cout << "Subject(s): ";
                    getline(cin, input);

                    string filterSubjects[7];
                    int subjectCount = 0;
                    string subject;

                    for (int i = 0; i < input.size(); i++) {
                        if (input[i] != ' ') { //If its not space, the character will store into the 'subject'
                            subject += input[i];
                        }
                        else {
                            filterSubjects[subjectCount++] = subject; //If its have space, it will ends the subject name and
                            subject.clear();                          //store into 'subject' then into 'filterSubject' for later.
                        }
                    }
                    filterSubjects[subjectCount++] = subject; //Final subject
                    cout << "==============" << endl;
                    cout << "Filtered list:" << endl;
                    cout << "==============" << endl;
                    filter(form, filterSubjects, subjectCount);
                }
                else {
                    cin.clear(); //Clear the error flag in case the form is not between 1 and 3 inclusive
                    cin.ignore();
                    cout << "Invalid form! Please enter form (1-3)." << endl;
                }

            }
            else if (choices == 2) {
                cout << "Please key in the keyword you want to search: ";
                getline(cin, input);
                search(input);
            }
        }
        else {
            cin.clear(); //Clear the error flag in case the input is not 1 or 2
            cin.ignore();
            cout << "Invalid input! Please enter either the number 1 or 2." << endl;
        }

        do {
            cout << "------------------------------------------------------" << endl;
            cout << "Do you wish to continue filter and search? (Y-yes/N-no)" << endl;
            cout << "------------------------------------------------------" << endl;
            cout << "Choice: ";
            cin >> cont;
            cin.ignore();
            cont = tolower(cont);

            if (cont != 'y' && cont != 'n') {
                cout << "Invalid input! Please enter 'Y' or 'N'." << endl;
            }
        } while (cont != 'y' && cont != 'n');

        cout << "\n";
    } while (cont == 'y'); //The program will continue when 'Y' or 'y' was entered

}

// Function to display the current list of students
void displayStudentList() {
    cout << "-----------------------------------------------------------------------\n";
    cout << left << setw(15) << "Student ID" << setw(20) << "Name" << setw(8) << "Form" << "Subjects\n";
    cout << "-----------------------------------------------------------------------\n";

    for (int i = 0; i < studentCount; i++) {
        cout << left << setw(15) << students[i].studentId
            << setw(20) << students[i].name
            << setw(8) << students[i].form;

        // Display marks for each subject
        for (int j = 0; j < 7; j++) {
            cout << students[i].marks[j];
            if (j < 6)
                cout << ", ";
        }
        cout << "\n";
    }
    cout << "-----------------------------------------------------------------------\n";
}

// Function to modify existing student records
void editStudent() {
    displayStudentList();

    string studentId;
    cout << "\nPlease key in the student ID that you want to edit: ";
    cin >> studentId;

    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].studentId == studentId) {
            found = true;
            char cont;
            do {
                int editChoice;
                cout << "\nPlease select which option that you want to edit:\n";
                cout << "1. Name\n"; //Edit name
                cout << "2. Form\n"; //Edit form
                cout << "3. Marks\n"; //Edit marks
                cout << "Please key in the option that you want to edit: ";
                cin >> editChoice;

                if (editChoice == 1) {
                    cout << "Enter new name: ";
                    cin.ignore();
                    getline(cin, students[i].name);
                }
                else if (editChoice == 2) {
                    cout << "Please update the form: ";
                    cin >> students[i].form;
                }
                else if (editChoice == 3) {
                    string subject;
                    cout << "Please enter the subject that you want to change the mark (BM/BI/BC/Math/Sci/Sejarah/Geo): ";
                    cin >> subject;

                    bool validSubject = false;
                    for (int j = 0; j < 7; j++) {
                        if (subjects[j] == subject) {
                            cout << "Please update the mark for " << subjects[j] << ": ";
                            cin >> students[i].marks[j];
                            validSubject = true;
                            break;
                        }
                    }
                    if (!validSubject) {
                        cout << "Invalid subject entered!\n";
                    }
                }
                else {
                    cout << "Invalid choice! Try again.\n";
                }

                cout << "\nDo you still have anything to edit for this student? (Y/N): ";
                cin >> cont;
                cont = tolower(cont);

            } while (cont == 'y');

            // After finishing edits for this student, save changes
            saveToFile();
            cout << "\nSuccessfully updated!\n";

            // Show updated list
            displayStudentList();

            break;
        }
    }

    if (!found) {
        cout << "Student ID not found!\n";
    }

    // Ask if the user wants to edit another student
    char editMore;
    cout << "\nDo you want to edit record for other students? (Y/N): ";
    cin >> editMore;

    if (tolower(editMore) == 'y') {
        editStudent();
    }
}

// Function to delete a student record
void deleteStudent() {
    displayStudentList();

    string studentId;

    cout << "\nPlease key in the student ID that you want to delete the entire record: ";
    cin >> studentId;

    bool found = false;
    for (int i = 0; i < studentCount; i++) {
        if (students[i].studentId == studentId) {
            found = true;

            // Shift remaining students up
            for (int j = i; j < studentCount - 1; j++) {
                students[j] = students[j + 1];
            }
            studentCount--;
            saveToFile();

            cout << "\nSuccessfully Deleted!\n";

            displayStudentList();
            break;
        }
    }

    if (!found) {
        cout << "Student ID not found!\n";
    }

    // Ask if the user wants to delete another student
    char deleteMore;
    cout << "\nDo you want to delete record for other students? (Y/N): ";
    cin >> deleteMore;

    if (tolower(deleteMore) == 'y') {
        deleteStudent();
    }
}

//Function to edit and delete for main menu
void editAndDelete() {
    int modifyChoice;
    char cont;

    do {
        cout << "-----------------------------" << endl;
        cout << "Please select your choice: " << endl;
        cout << "    1. Edit" << endl;
        cout << "    2. Delete" << endl;
        cout << "-----------------------------" << endl;
        cout << "\nChoice: ";

        if (cin >> modifyChoice && (modifyChoice == 1 || modifyChoice == 2)) {
            cin.ignore();

            if (modifyChoice == 1) {
                editStudent();
            }
            else if (modifyChoice == 2) {
                deleteStudent();
            }
        }
        else {
            cin.clear();
            cin.ignore();
            cout << "Invalid choice! Please try again." << endl;
        }

        do {
            cout << "------------------------------------------------------" << endl;
            cout << "Do you wish to continue edit and delete? (Y-yes/N-no)" << endl;
            cout << "------------------------------------------------------" << endl;
            cout << "Choice: ";
            cin >> cont;
            cin.ignore();
            cont = tolower(cont);

            if (cont != 'y' && cont != 'n') {
                cout << "Invalid input! Please enter 'Y' or 'N'." << endl;
            }
        } while (cont != 'y' && cont != 'n');

        cout << "\n";
    } while (cont == 'y');
}

// Function to display the main menu and handle user input
void showMenu() {
    int choice;
    do {
        system("cls");
        cout << "************************" << endl;
        cout << "*       Welcome        *" << endl;
        cout << "*         to           *" << endl;
        cout << "*        JKJT          *" << endl;
        cout << "************************" << endl;
        cout << "Main Menu Page:\n";
        cout << "Please select your choice:\n";
        cout << "1. Insert\n";
        cout << "2. Filter/Search\n";
        cout << "3. Edit/Delete\n";
        cout << "4. View Results\n";
        cout << "5. Exit\n";
        cout << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore();
            cout << "Invalid input! Please enter a number between 1 to 5.\n";
            continue;
        }

        switch (choice) {
        case 1:
            addRecords();
            break;
        case 2:
            filterAndSearch();
            break;
        case 3:
            editAndDelete();
            break;
        case 4:
            displayResults();
            break;
        case 5:
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice! Please try again.\n";
            continue;
        }
    } while (choice != 5);
}
