#define _CRT_SECURE_NO_WARNINGS
#include	<iostream>
#include	<iomanip>
#include 	<fstream>
#include	<string>
#include	<cstring>
#include	<cstdio>
#include	<cstdlib>
#include	"List.h"
#include	"Student.h"
#include	"Exam.h"
#include	"Subject.h"

using namespace std;

bool CreateStuList(const char*, List*);
bool DeleteStudent(List*, char*);
bool PrintList(List, int);
bool InsertExamResult(const char*, List*);
bool PrintStatistic(List);
bool FilterStudent(List, List*, char*, int, int);
bool UpdateIDandPhone(List*);
bool FindPotentialFirstClass(List, List*, char*);
int menu();

//function 1
bool CreateStuList(const char* filename, List* list) {
	Student studentdata;
	string line;
	int count = 0;
	int line_cnt = 0;
	ifstream file;
	file.open(filename);
	if (!file) {
		cout << "Cannot open the file!! \n";
		return false;
	}

	while (getline(file, line))
	{
		if (line.empty())continue;//check empty line

		int pos = line.find('=');
		string data = line.substr(pos + 2);
		line_cnt++;

		if (line_cnt == 1)//read student id
		{
			strcpy(studentdata.id, data.c_str());
		}

		else if (line_cnt == 2)//read name
		{
			strcpy(studentdata.name, data.c_str());
		}

		else if (line_cnt == 3)//read course
		{
			strcpy(studentdata.course, data.c_str());
		}

		else if (line_cnt == 4)//read phone_no, when line_cnt is 4, means that a student data have been read out.
		{
			strcpy(studentdata.phone_no, data.c_str());

			//before insert checking duplicate
			bool duplicate = false;
			Student check;
			for (int i = 1; i <= list->size(); i++)
			{
				if (list->empty())break;

				list->get(i, check);
				if (studentdata.compareID(check))
				{
					duplicate = true;
				}
			}
			if (!duplicate)
				list->insert(studentdata);
			line_cnt = 0;
			count++;
		}
	}
	file.close();

	//print the result of student list;
	for (int i = 1; i <= list->size(); i++)
	{
		Student s;
		list->get(i, s);
		cout << s.id << endl;
		cout << s.name << endl;
		cout << s.course << endl;
		cout << s.phone_no << endl;
		cout << endl;
	}
	return true;
}


//function 2
bool DeleteStudent(List* list, char* id) {

	bool success = false;
	int deleteID;
	Student studentdata;
	for (int i = 1; i <= list->size(); i++)
	{
		list->get(i, studentdata);
		if (strcmp(studentdata.id, id) == 0) {
			deleteID = i;
			success = true;
		}
	}
	if (!success) {
		return false;
	}
	else {
		list->remove(deleteID);
		//print the result of student list;
		for (int i = 1; i <= list->size(); i++)
		{
			Student s;
			list->get(i, s);
			cout << s.id << endl;
			cout << s.name << endl;
			cout << s.course << endl;
			cout << s.phone_no << endl;
			cout << endl;
		}
		return true;
	}
}

//function 3
bool PrintList(List list, int source)
{
	if (list.empty())
		return false;

	ostream* out;
	ofstream fileOut;

	if (source == 2) { // Print to file
		fileOut.open("student_output.txt");
		if (!fileOut) {
			cout << "Cannot open output file!" << endl;
			return false;
		}
		out = &fileOut;
	}
	else { // Print to screen
		out = &cout;
	}

	int studentNum = 1;
	for (int i = 1; i <= list.size(); i++) {
		Student s;
		list.get(i, s);

		// Print student header
		*out << string(54, '*') << "STUDENT " << studentNum << string(54, '*') << "\n\n\n";

		// Print basic student info
		s.print(*out);


		// Print exam results if any
		if (s.exam_cnt > 0) {
			*out << "\n" << string(50, '-') << "PAST EXAM RESULT: " << string(50, '-') << " \n\n";

			for (int j = 0; j < s.exam_cnt; j++) {
				s.exam[j].print(*out);
			}
		}

		// Print footer
		*out << "\n" << string(54, '*') << "STUDENT " << studentNum << string(54, '*') << "\n\n";
		studentNum++;
	}

	if (source == 2) {
		fileOut.close();
		cout << "Student list has been written to student_output.txt" << endl;
	}

	return true;
}


//function 4
bool InsertExamResult(const char* filename, List* list)
{
	ifstream file(filename);
	if (!file)
	{
		cout << "Cannot open the file!! \n";
		return false;
	}

	while (!file.eof())
	{
		char id[12];
		file >> id;
		if (strlen(id) == 0) break; // Skip empty lines

		// Find the student in the list
		bool studentFound = false;
		int studentPos = 0;
		Student studentData;

		for (int i = 1; i <= list->size(); i++)
		{
			Student temp;
			list->get(i, temp);
			if (strcmp(temp.id, id) == 0)
			{
				studentFound = true;
				studentPos = i;
				list->get(i, studentData);
				break;
			}
		}

		if (!studentFound)
		{
			cout << "Student with ID " << id << " not found in the list!\n";
			continue;
		}

		// Check if student has reached max exam count
		if (studentData.exam_cnt >= 10)
		{
			cout << "Student " << id << " has reached maximum exam records (10).\n";
			continue;
		}

		// Read exam data
		Exam newExam;
		file >> newExam.trimester >> newExam.year >> newExam.numOfSubjects;

		// Check if number of subjects exceeds maximum
		if (newExam.numOfSubjects > 6)
		{
			cout << "Student " << id << " has too many subjects (" << newExam.numOfSubjects
				<< "). Maximum is 6.\n";
			continue;
		}

		// Read subject data
		for (int i = 0; i < newExam.numOfSubjects; i++)
		{
			file >> newExam.sub[i].subject_code >> newExam.sub[i].subject_name
				>> newExam.sub[i].credit_hours >> newExam.sub[i].marks;
		}

		// Calculate GPA for this exam
		if (!newExam.calculateGPA())
		{
			cout << "Failed to calculate GPA for student " << id << endl;
			continue;
		}

		// Add the exam to student's record
		studentData.exam[studentData.exam_cnt] = newExam;
		studentData.exam_cnt++;

		// Update student's CGPA
		if (!studentData.calculateCurrentCGPA())
		{
			cout << "Failed to calculate current CGPA for student " << id << endl;
			continue;
		}

		// Update the student in the list
		list->set(studentPos, studentData);
	}

	file.close();
	return true;
}

//function 5
bool PrintStatistic(List list)
{
	if (list.empty())
	{
		cout << "List is empty!! \n";
		return false;
	}

	Student studentdata;
	double total_cgpa_jan = 0, total_cgpa_may = 0, total_cgpa_oct = 0;
	double total_credit_jan = 0, total_credit_may = 0, total_credit_oct = 0;
	double total_subject_jan = 0, total_subject_may = 0, total_subject_oct = 0;
	int jan_cnt = 0, may_cnt = 0, oct_cnt = 0;
	string trimester[3] = { "JAN","MAY","OCT" };
	for (int i = 1; i <= list.size(); i++)
	{
		list.get(i, studentdata);
		int count = studentdata.exam_cnt;
		for (int j = 0; j < count; j++)
		{
			if (studentdata.exam[j].trimester == 1)//January Trimester
			{
				if (studentdata.calculateCurrentCGPA())
				{
					total_cgpa_jan += studentdata.current_cgpa;
					total_credit_jan += studentdata.totalCreditsEarned;
					total_subject_jan += studentdata.exam[j].numOfSubjects;
					jan_cnt++;
				}
			}
			else if (studentdata.exam[j].trimester == 5)//January Trimester
			{
				if (studentdata.calculateCurrentCGPA())
				{
					total_cgpa_may += studentdata.current_cgpa;
					total_credit_may += studentdata.totalCreditsEarned;
					total_subject_may += studentdata.exam[j].numOfSubjects;
					may_cnt++;
				}
			}
			else if (studentdata.exam[j].trimester == 10)//January Trimester
			{
				if (studentdata.calculateCurrentCGPA())
				{
					total_cgpa_oct += studentdata.current_cgpa;
					total_credit_oct += studentdata.totalCreditsEarned;
					total_subject_oct += studentdata.exam[j].numOfSubjects;
					oct_cnt++;
				}
			}
		}
	}

	//calculate statistic for every semester
	double avg_cgpa;
	double avg_sub;
	double avg_credit;
	for (int i = 0; i < 3; i++)
	{
		cout << trimester[i] << " Trimester" << endl;
		cout << endl;
		if (i == 0)
		{
			avg_cgpa = total_cgpa_jan / jan_cnt;
			avg_sub = total_subject_jan / jan_cnt;
			avg_credit = total_credit_jan / jan_cnt;
			cout << "Average CGPA: " << avg_cgpa << endl;
			cout << "Average Subjects Taken Per Semester: " << avg_sub << endl;
			cout << "Average Credits Earned Per Semester: " << avg_credit << endl;
			cout << endl;
		}
		else if (i == 1) {
			avg_cgpa = total_cgpa_may / may_cnt;
			avg_sub = total_subject_may / may_cnt;
			avg_credit = total_credit_may / may_cnt;
			cout << "Average CGPA: " << avg_cgpa << endl;
			cout << "Average Subjects Taken Per Semester: " << avg_sub << endl;
			cout << "Average Credits Earned Per Semester: " << avg_credit << endl;
			cout << endl;
		}
		else if (i == 2) {
			avg_cgpa = total_cgpa_oct / oct_cnt;
			avg_sub = total_subject_oct / oct_cnt;
			avg_credit = total_credit_oct / oct_cnt;
			cout << "Average CGPA: " << avg_cgpa << endl;
			cout << "Average Subjects Taken Per Semester: " << avg_sub << endl;
			cout << "Average Credits Earned Per Semester: " << avg_credit << endl;
			cout << endl;
		}
	}
	return true;
}

//function 6
bool FilterStudent(List list1, List* list2, char* course, int year, int totalcredit) {
	if (list1.empty()) {
		cout << "List is empty!! \n";
		return false;
	}

	// Clear the destination list
	while (!list2->empty()) {
		list2->remove(1);
	}

	bool found = false;
	for (int i = 1; i <= list1.size(); i++) {
		Student student;
		list1.get(i, student);

		// Check course match
		bool courseMatch = (strcmp(student.course, course) == 0);

		// Calculate enrollment year from ID (assuming format 20XX)
		int enrollmentYear = 2000 + (student.id[0] - '0') * 10 + (student.id[1] - '0');
		bool yearMatch = (enrollmentYear == year);

		// Check credits match
		bool creditMatch = (student.totalCreditsEarned >= totalcredit);

		if (courseMatch && yearMatch && creditMatch) {
			list2->insert(student);
			found = true;
		}
	}

	if (!found) {
		cout << "No students match the filter criteria!\n";
		return false;
	}

	return true;
}

//function 7
bool UpdateIDandPhone(List* list) {
	if (list->empty()) {
		cout << "List is empty! No updates performed.\n";
		return false;
	}

	for (int i = 1; i <= list->size(); i++) {
		Student student;
		list->get(i, student);

		// Update ID according to Table 1 (add course-specific prefix)
		char newID[12];
		strcpy(newID, "B"); // Start with 'B'

		// Add course code (first letter uppercase)
		newID[1] = toupper(student.course[0]);
		if (strlen(student.course) > 1) {
			newID[2] = toupper(student.course[1]);
			newID[3] = '\0';
		}
		else {
			newID[2] = '\0';
		}

		strcat(newID, student.id);
		strcpy(student.id, newID);

		// Update phone number according to Table 2
		char newPhone[10];
		if (student.phone_no[0] % 2 == 1) { // Odd first digit
			newPhone[0] = '0';
			newPhone[1] = '1';
			// Copy digits (skip dash if exists)
			int pos = 2;
			for (int j = 0; j < strlen(student.phone_no); j++) {
				if (isdigit(student.phone_no[j])) {
					newPhone[pos++] = student.phone_no[j];
				}
			}
			newPhone[pos] = '\0';
		}
		else { // Even first digit
			newPhone[0] = '0';
			newPhone[1] = '2';
			// Copy digits (skip dash if exists)
			int pos = 2;
			for (int j = 0; j < strlen(student.phone_no); j++) {
				if (isdigit(student.phone_no[j])) {
					newPhone[pos++] = student.phone_no[j];
				}
			}
			newPhone[pos] = '\0';
		}

		strcpy(student.phone_no, newPhone);

		// Update the student in the list
		list->set(i, student);
	}

	cout << "Successfully updated IDs and phone numbers for all students.\n";
	return true;
}

//function 8
bool FindPotentialFirstClass(List list1, List* list2, char* course) {
	// Check preconditions
	if (list1.empty()) {
		cout << "Error: Source student list is empty!\n";
		return false;
	}

	if (!list2->empty()) {
		cout << "Error: Destination list must be empty!\n";
		return false;
	}

	bool foundQualifiedStudents = false;

	// Process each student in the source list
	for (int i = 1; i <= list1.size(); i++) {
		Student student;
		list1.get(i, student);

		// Skip if not in the specified course
		if (strcmp(student.course, course) != 0) {
			continue;
		}

		// Skip if less than 3 exams taken
		if (student.exam_cnt < 3) {
			continue;
		}

		int highGPAExams = 0;
		bool meetsCriteria = true;

		// Check each exam result
		for (int j = 0; j < student.exam_cnt; j++) {
			Exam exam = student.exam[j];

			// Disqualify if any exam has GPA < 3.5
			if (exam.gpa < 3.5) {
				meetsCriteria = false;
				break;
			}

			// Count qualifying exams (GPA >= 3.75 AND >= 12 credit hours)
			if (exam.gpa >= 3.75 && exam.numOfSubjects >= 12) {
				highGPAExams++;
			}
		}

		// If meets all criteria (at least 3 high GPA exams and no disqualifications)
		if (meetsCriteria && highGPAExams >= 3) {
			list2->insert(student);
			foundQualifiedStudents = true;
		}
	}

	// Handle case when no qualified students found
	if (!foundQualifiedStudents) {
		cout << "There is no student in " << course
			<< " that has potential to get first class\n";
		return false;
	}

	return true;
}

//function 9
int menu() {
	int choice;
	cout << "\n===== MENU =====\n";
	cout << "1. Create student list\n";
	cout << "2. Delete Student\n";
	cout << "3. Print student list\n";
	cout << "4. Insert exam result\n";
	cout << "5. Print Exam Statistic\n";
	cout << "6. Filter Student\n";
	cout << "7. Update Student's ID and Phone\n";
	cout << "8. Find Potential First Class Student\n";
	cout << "9. Exit\n";
	cout << "Enter your choice (1-9): ";
	cin >> choice;
	return choice;
}


void makeUpperCase(char* str) {
	for (int i = 0; str[i]; i++) {
		str[i] = toupper(str[i]);
	}
}


int main() {

	List* studentList = new List;
	List filteredList;
	List firstClassList;
	int choice;
	const char filename1[12] = "student.txt";
	const char filename2[12] = "exam.txt";
	char id[12], course[3];
	int year, totalcredit, source;

	do {

		choice = menu();
		switch (choice) {
		case 1:
			CreateStuList(filename1, studentList);
			if (!studentList->empty())
			{
				cout << "Successfully Create Student List!! \n\n";
			}
			else {
				cout << "Failed to create student list!!\n\n";
			}
			system("Pause");
			system("cls");
			break;

		case 2:
			cout << "Enter student ID to delete: ";
			cin >> id;
			if (DeleteStudent(studentList, id)) {
				cout << "Successfully Delete Student Data!! \n\n";
			}
			else {
				cout << "THE STUDENT CANNOT FOUND IN THE LIST!! \n\n";
			}
			system("Pause");
			system("cls");
			break;

		case 3:
			cout << "Print to (1) screen or (2) file: ";
			cin >> source;
			PrintList(*studentList, source);
			system("Pause");
			system("cls");
			break;

		case 4:
			if (InsertExamResult(filename2, studentList)) {
				cout << "Exam results inserted successfully!" << endl;
			}
			else {
				cout << "Failed to insert exam results!" << endl;
			}
			system("Pause");
			system("cls");
			break;

		case 5:
			PrintStatistic(*studentList);
			system("Pause");
			system("cls");
			break;

		case 6:
			cout << "Enter course to filter (CS,IA,IB,CN): ";
			char course[3];
			cin >> course;
			makeUpperCase(course);

			cout << "Enter year enrolled: ";
			int year;
			cin >> year;

			cout << "Enter minimum total credits: ";
			int totalcredit;
			cin >> totalcredit;

			if (FilterStudent(*studentList, &filteredList, course, year, totalcredit)) {
				cout << "Filtered students:" << endl;
				PrintList(filteredList, 1);
			}
			filteredList = List();
			system("Pause");
			system("cls");
			break;

		case 7:
			if (UpdateIDandPhone(studentList)) {
				cout << "ID and phone numbers updated successfully!" << endl;
				PrintList(*studentList, 1);
			}
			else {
				cout << "Update failed!" << endl;
			}
			system("Pause");
			system("cls");
			break;

		case 8:
			cout << "Enter course to check for first class (CS,IA,IB,CN): ";
			cin >> course;
			makeUpperCase(course);

			if (FindPotentialFirstClass(*studentList, &firstClassList, course)) {
				cout << "Potential first class students:" << endl;
				PrintList(firstClassList, 1);
			}
			firstClassList = List();
			system("Pause");
			system("cls");
			break;

		case 9:
			cout << "Exiting program..." << endl;
			system("Pause");
			system("cls");
			break;

		default:
			cout << "Invalid choice! Please try again." << endl;
			system("Pause");
			system("cls");
		}
	} while (choice != 9);

	
	system("pause");
	return 0;
}