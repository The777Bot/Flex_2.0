#include<iostream>
#include"SFML/Graphics.hpp"
#include"Windows.h"
#include<fstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include<string>
using namespace std;
using namespace sf;
sf::Color yaleBlue(13, 77, 146);
sf::Color offWhiteColor(240, 240, 240);
sf::Color lightGray(211, 211, 211);

sf::Color darkerYaleBlue(8, 40, 79);
void handleAcademicOfficerAction(sf::RenderWindow& window, int& input);
void handleStudentAction(sf::RenderWindow& window, int& input);
class Validator {
public:
    static bool isValidRollNumber(int rollNumber) {
        return rollNumber > 0; // Add more validation as needed
    }

    static bool isValidAge(int age) {
        return age > 0 && age < 150; // Add more validation as needed
    }

    static bool isValidContact(const string& contact) {
        // Add contact validation as needed
        return true;
    }

    static bool isValidCourseCode(const string& courseCode) {
        // Course code should be alphanumeric and have a specific length range, e.g., 5 to 10 characters
        if (courseCode.length() >= 5 && courseCode.length() <= 10) {
            // Check if all characters in the course code are alphanumeric
            return all_of(courseCode.begin(), courseCode.end(), [](unsigned char c) { return isalnum(c); });
        }
        else {
            return false;
        }
    }
};

class Student {
private:
    string name;
    int rollNumber;
    int age;
    string contact;
    vector<string> courses;

public:
    Student(const string& n, int roll, int a, const string& c) : name(n), rollNumber(roll), age(a), contact(c) {}

    void displayStudentInfo() const {
        cout << "Name: " << name << "\tRoll Number: " << rollNumber << "\tAge: " << age << "\tContact: " << contact << endl;
        displayEnrolledCourses();
    }

    void displayEnrolledCourses() const 
    {
        if (!courses.empty()) {
            cout << "Enrolled Courses: ";
            for (const auto& course : courses)
            {
                cout << course << " ";
            }
            cout << endl;
        }
    }

    string getName() const {
        return name;
    }

    int getRollNumber() const {
        return rollNumber;
    }

    int getAge() const {
        return age;
    }

    const string& getContact() const {
        return contact;
    }

    const vector<string>& getEnrolledCourses() const {
        return courses;
    }

    // Member functions for enrollment, withdrawal, marking attendance, and assigning marks
    void enrollInCourse(const string& courseCode) {
        courses.push_back(courseCode);
    }

    void withdrawFromCourse(const string& courseCode) {
        auto it = remove(courses.begin(), courses.end(), courseCode);
        courses.erase(it, courses.end());
    }

    // Getters and setters as needed
};

class Course {
private:
    string code;
    string name;
    int capacity;
    vector<Student*> enrolledStudents;

public:
    Course(const string& c, const string& n, int cap) : code(c), name(n), capacity(cap) {}

    void displayCourseInfo() const {
        cout << "Code: " << code << "\tName: " << name << "\tCapacity: " << capacity << endl;
    }

    // Member functions for adding, removing, and displaying enrolled students
    void enrollStudent(Student* student) {
        if (enrolledStudents.size() < capacity) {
            enrolledStudents.push_back(student);
            student->enrollInCourse(code);
            cout << "Student enrolled in the course.\n";
        }
        else {
            cout << "Course is full. Cannot enroll more students.\n";
        }
    }

    void displayEnrolledStudents() const {
        cout << "Enrolled Students in " << name << ":\n";
        for (const auto& student : enrolledStudents) {
            student->displayStudentInfo();
        }
    }

    void withdrawStudent(Student* student) {
        auto it = remove(enrolledStudents.begin(), enrolledStudents.end(), student);
        enrolledStudents.erase(it, enrolledStudents.end());
        student->withdrawFromCourse(code);
        cout << "Student withdrawn from the course.\n";
    }

    // Getters and setters as needed
    const string& getCode() const {
        return code;
    }

    const string& getName() const {
        return name;
    }

    int getCapacity() const {
        return capacity;
    }

    const vector<Student*>& getEnrolledStudents() const {
        return enrolledStudents;
    }
};

class FileHandler {
private:
    string filename;
    string mode;

public:
    FileHandler(const string& fname, const string& m) : filename(fname), mode(m) {}

    void openFile() {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file: " << filename << endl;
            exit(EXIT_FAILURE);
        }
        file.close();
    }

    void closeFile() {
        // Closing the file is not necessary in this case since ifstream automatically closes when it goes out of scope.
    }

    void readFile(vector<Student>& students, vector<Course>& courses) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file: " << filename << endl;
            exit(EXIT_FAILURE);
        }

        // Assuming file structure is such that student data is followed by course data
        // Read student data
        int numStudents;
        file >> numStudents;

        for (int i = 0; i < numStudents; ++i) {
            string name, contact;
            int rollNumber, age;
            file >> name >> rollNumber >> age >> contact;
            students.emplace_back(name, rollNumber, age, contact);
        }

        // Read course data
        int numCourses;
        file >> numCourses;

        for (int i = 0; i < numCourses; ++i) {
            string code, name;
            int capacity;
            file >> code >> name >> capacity;
            courses.emplace_back(code, name, capacity);
        }

        file.close();
    }

    // Implement writing to file logic
    void writeFile(const vector<Student>& students, const vector<Course>& courses) const {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Unable to open file for writing: " << filename << endl;
            exit(EXIT_FAILURE);
        }

        // Write student data
        file << students.size() << endl;
        for (const auto& student : students) {
            file << student.getName() << " " << student.getRollNumber() << " " << student.getAge() << " " << student.getContact() << endl;
        }

        // Write course data
        file << courses.size() << endl;
        for (const auto& course : courses) {
            file << course.getCode() << " " << course.getName() << " " << course.getCapacity() << endl;
        }

        file.close();
    }
};

class System {
private:
    vector<Student> students;
    vector<Course> courses;
    FileHandler fileHandler;

    void courseRegistrationMenu() {
        int choice;
        do {
            cout << "Course Registration Menu\n";
            cout << "1- Display Available Courses\n";
            cout << "2- Register for a Course\n";
            cout << "3- Back\n";
            cout << "Press 1 to 3 to select an option: ";
            cin >> choice;

            switch (choice) {
            case 1:
                displayAvailableCourses();
                break;
            case 2:
                registerForCourse();
                break;
            case 3:
                cout << "Going back to the main menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 3);
    }

    void displayAvailableCourses() const {
        cout << "Available Courses:\n";
        for (const auto& course : courses) {
            course.displayCourseInfo();
        }
    }

    void registerForCourse() {
        int studentRollNumber;
        cout << "Enter your roll number: ";
        cin >> studentRollNumber;

        auto studentIt = find_if(students.begin(), students.end(),
            [studentRollNumber](const Student& s) { return s.getRollNumber() == studentRollNumber; });

        if (studentIt != students.end()) {
            cout << "Enter the course code you want to register for: ";
            string courseCode;
            cin >> courseCode;

            auto courseIt = find_if(courses.begin(), courses.end(),
                [courseCode](const Course& c) { return c.getCode() == courseCode; });

            if (courseIt != courses.end()) {
                courseIt->enrollStudent(&(*studentIt));
            }
            else {
                cout << "Course not found.\n";
            }
        }
        else {
            cout << "Student not found.\n";
        }
    }

    // Similar logic for attendanceMenu, marksMenu, and courseWithdrawMenu
    // ...
    void attendanceMenu() {
        int choice;
        do {
            cout << "Attendance Menu\n";
            cout << "1- Display Attendance (Subject wise)\n";
            cout << "2- Mark Attendance\n";
            cout << "3- Back\n";
            cout << "Press 1 to 3 to select an option: ";
            cin >> choice;

            switch (choice) {
            case 1:
                displaySubjectWiseAttendance();
                break;
            case 2:
                markAttendance();
                break;
            case 3:
                cout << "Going back to the main menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 3);
    }

    void displaySubjectWiseAttendance() const {
        // Assuming you have courses and want to display attendance for each course
        for (const auto& course : courses) {
            cout << "Attendance for " << course.getName() << ":\n";
            course.displayEnrolledStudents(); // Display enrolled students in the course
            // Implement logic to display attendance for each student in the course
        }
    }

    void markAttendance() {
        // Assuming you have courses and want to mark attendance for each course
        for (auto& course : courses) {
            cout << "Mark Attendance for " << course.getName() << ":\n";
            course.displayEnrolledStudents(); // Display enrolled students in the course

            // Implement logic to mark attendance for each student in the course
            for (auto& student : course.getEnrolledStudents()) {
                string status;
                cout << "Enter attendance status for " << student->getName() << " (Present/Absent): ";
                cin >> status;

                // Add error checking for status if needed
                // Set attendance status for the student
                // For example, you might have a function in the Student class like student.setAttendance(course.getName(), status);
            }
        }
    }

    void marksMenu() {
        int choice;
        do {
            cout << "Marks Menu\n";
            cout << "1- Display Marks (Subject wise)\n";
            cout << "2- Assign Marks\n";
            cout << "3- Back\n";
            cout << "Press 1 to 3 to select an option: ";
            cin >> choice;

            switch (choice) {
            case 1:
                displaySubjectWiseMarks();
                break;
            case 2:
                assignMarks();
                break;
            case 3:
                cout << "Going back to the main menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 3);
    }

    void displaySubjectWiseMarks() const {
        // Implement logic to display subject-wise marks
        // Assuming you have courses and want to display marks for each course
        for (const auto& course : courses) {
            cout << "Marks for " << course.getName() << ":\n";
            course.displayEnrolledStudents(); // Display enrolled students in the course

            // Implement logic to display marks for each student in the course
            for (const auto& student : course.getEnrolledStudents()) {
                // Display marks for the student
                // For example, you might have a function in the Student class like student.displayMarks(course.getName());
            }
        }
    }

    void assignMarks() {
        // Implement logic to assign marks

        // Assuming you have courses and want to assign marks for each course
        for (auto& course : courses) {
            cout << "Assign Marks for " << course.getName() << ":\n";
            course.displayEnrolledStudents(); // Display enrolled students in the course

            // Implement logic to assign marks for each student in the course
            for (auto& student : course.getEnrolledStudents()) {
                int marks;
                cout << "Enter marks for " << student->getName() << ": ";
                cin >> marks;

                // Add error checking for marks if needed
                // Assign marks to the student
                // For example, you might have a function in the Student class like student.assignMarks(course.getName(), marks);
            }
        }
    }

    void courseWithdrawMenu() {
        int choice;
        do {
            cout << "Course Withdraw Menu\n";
            cout << "1- Enrolled Courses\n";
            cout << "2- Drop a Course\n";
            cout << "3- Back\n";
            cout << "Press 1 to 3 to select an option: ";
            cin >> choice;

            switch (choice) {
            case 1:
                displayEnrolledCourses();
                break;
            case 2:
                dropCourse();
                break;
            case 3:
                cout << "Going back to the main menu.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 3);
    }

    void displayEnrolledCourses() const {
        // Implement logic to display enrolled courses
        // Assuming you have students and want to display enrolled courses for each student
        for (const auto& student : students) {
            student.displayStudentInfo();
            student.displayEnrolledCourses(); // Display enrolled courses for the student
        }
    }

    void dropCourse() {
        // Implement logic to drop a course
        int studentRollNumber;
        cout << "Enter your roll number: ";
        cin >> studentRollNumber;

        auto studentIt = std::find_if(students.begin(), students.end(),
            [studentRollNumber](const Student& s) { return s.getRollNumber() == studentRollNumber; });

        if (studentIt != students.end()) {
            int courseIndex = 1;
            cout << "Enrolled Courses:\n";
            for (const auto& course : studentIt->getEnrolledCourses()) {
                cout << courseIndex << "- " << course << endl;
                courseIndex++;
            }

            if (!studentIt->getEnrolledCourses().empty()) {
                int choice;
                cout << "Enter the number corresponding to the course you want to drop: ";
                cin >> choice;

                // Add error checking for choice if needed
                if (choice >= 1 && choice <= studentIt->getEnrolledCourses().size()) {
                  //  studentIt->withdrawFromCourse(static_cast<si>(choice - 1));

                    studentIt->withdrawFromCourse(studentIt->getEnrolledCourses()[choice ]);
                }
                else {
                    cout << "Invalid choice. No course dropped.\n";
                }
            }
            else {
                cout << "No enrolled courses to drop.\n";
            }
        }
        else {
            cout << "Student not found.\n";
        }
    }

    void enrollStudent() {
        // Implement logic for enrolling a student
        string name, contact;
        int rollNumber, age;

        cout << "Enter student name: ";
        cin.ignore(); // Clear buffer
        getline(cin, name);

        cout << "Enter student roll number: ";
        cin >> rollNumber;

        // Add validation for rollNumber using isValidRollNumber

        cout << "Enter student age: ";
        cin >> age;

        // Add validation for age using isValidAge

        cout << "Enter student contact: ";
        cin.ignore(); // Clear buffer
        getline(cin, contact);

        // Add validation for contact using isValidContact

        Student newStudent(name, rollNumber, age, contact);
        students.push_back(newStudent);

        cout << "Student enrolled successfully.\n";
    }

public:
    System(const string& filename) : fileHandler(filename, "data.txt") {}

    void loadDataFromFile() {
        fileHandler.readFile(students, courses);
    }

    void saveDataToFile() const {
        fileHandler.writeFile(students, courses);
    }
    bool checkSettingMenu(const int& x, const int& y)
    {
        if (y >= 313 && y <= 605)
        {
            if (x >= 320 && x <= 340)//for  first button x coordinates
            {
                enrollStudent();
                //break;
            }
            else if (x >= 280 && x <= 340)// for sceond button 
            {
                courseRegistrationMenu();
            }
            else if (x >= 350 && x <= 380)//for third button
            {
                attendanceMenu();
            }
            else {//if y coordiantes not wihtin bound return false
                return false;
            }
        }
    }
    void run()
    {
        loadDataFromFile();
        sf::RenderWindow window(sf::VideoMode(830, 600), "FLEX_2.0");
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color::Cyan);

            // Display your user interface using SFML, e.g., buttons, text, etc.
            // For simplicity, I'm displaying a basic message.
            sf::Font font;
            if (!font.loadFromFile("Fonts/verdanab.ttf")) {
                cerr << "Error: Font not found.\n";
                return;
            }

            sf::Text text;
            text.setFont(font);
            text.setString("Welcome to Student Management System");
            text.setCharacterSize(26);
            text.setFillColor(sf::Color::White);
            text.setPosition(100.f, 50.f);
            window.draw(text);


            sf::Text text2;
            text2.setFont(font);
            text2.setString("MAIN MENU");
            text2.setCharacterSize(21);
            text2.setFillColor(sf::Color::Black);
            text2.setPosition(330.f, 150.f);
            window.draw(text2);


            ///////////////////////////



            sf::Text text3;
            text3.setFont(font);
            text3.setString("ENROLL A STUDENT");
            text3.setCharacterSize(20);
            text3.setFillColor(sf::Color::Black);
            text3.setPosition(320.f, 200.f);
            window.draw(text);
            window.draw(text3);

            ///////////////////////////////


            sf::Text text4;
            text4.setFont(font);
            text4.setString("COURSE REGISTERATION");
            text4.setCharacterSize(20);
            text4.setFillColor(sf::Color::Black);
            text4.setPosition(280.f, 250.f);
            window.draw(text);
            window.draw(text4);

            ///////////////////////////////////


            sf::Text text5;
            text5.setFont(font);
            text5.setString("ATTENDENCE");
            text5.setCharacterSize(20);
            text5.setFillColor(sf::Color::Black);
            text5.setPosition(350.f, 300.f);
            window.draw(text);
            window.draw(text5);

            //////////////////////////////////////


            sf::Text text6;
            text6.setFont(font);
            text6.setString(" MARKS");
            text6.setCharacterSize(20);
            text6.setFillColor(sf::Color::Black);
            text6.setPosition(376.f, 350.f);
            window.draw(text);
            window.draw(text6);

            ///////////////////////////////////////

            sf::Text text7;
            text7.setFont(font);
            text7.setString(" COURSE WITHDRAW");
            text7.setCharacterSize(20);
            text7.setFillColor(sf::Color::Black);
            text7.setPosition(305.f, 400.f);
            window.draw(text);
            window.draw(text7);

            /////////////////////////////////////

            sf::Text text8;
            text8.setFont(font);
            text8.setString(" EXIT");
            text8.setCharacterSize(20);
            text8.setFillColor(sf::Color::Black);
            text8.setPosition(376.f, 450.f);
            window.draw(text);
            window.draw(text8);

            //////////////////////////////////////
            //mouse buttons
            if (Mouse::isButtonPressed(Mouse::Left))
            {
                Vector2i mousepos = Mouse::getPosition(window);
                Sleep(200);//delays program for 0.2 second to avoid taking too much cpu time
                checkSettingMenu(mousepos.x, mousepos.y);

                
            }
            window.display();
            // window.close();
             // Your existing logic (input handling, processing, etc.) remains in the System class.


        /* sf::RenderWindow addMenuWindow(sf::VideoMode(700, 500), "FLEX_2.0");

         sf::Font font;
         font.loadFromFile("Fonts/verdanab.ttf");
         sf::Text inputPrompt("Enter Course Information", font, 45);
         inputPrompt.setPosition(40.0f, 40.0f);
         inputPrompt.setOutlineColor(sf::Color::Black);
         inputPrompt.setOutlineThickness(3);
         sf::Text resultText("", font, 20);
         resultText.setPosition(340.f, 160.f);
         resultText.setFillColor(sf::Color::Black);
         sf::String userInput;*/

         //addMenuWindow.setPosition(40.0f, 40.0f);
         // Load existing data from file
        int choice;
        do {
            // Display main menu and handle user input
            cout << "Main Menu\n";
            cout << "1- Enroll a student\n";
            cout << "2- Course Registration\n";
            cout << "3- Attendance\n";
            cout << "4- Marks\n";
            cout << "5- Course Withdraw\n";
            cout << "6- Exit\n";
            cout << "Press 1 to 6 to select an option: ";
            cin >> choice;

            switch (choice) {
            case 1:
                enrollStudent();
                break;
            case 2:
                courseRegistrationMenu();
                break;
            case 3:
                attendanceMenu();
                break;
            case 4:
                marksMenu();
                break;
            case 5:
                courseWithdrawMenu();
                break;
            case 6:
                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
            }
        } while (choice != 6);
        saveDataToFile();
    }
};


//heres 
int main() {
   

    System system("data.txt");
    //system.loadDataFromFile();

   

    system.saveDataToFile();

   // System system("data.txt");
    system.run();
    return 0;
}

