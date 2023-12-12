#include<iostream>
#include"SFML/Graphics.hpp"
#include"Windows.h"
#include<fstream>
#include <vector>
#include <algorithm>
#include <cctype>
#include<string>
#include <unordered_map>
using namespace std;
using namespace sf;


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
    unordered_map<string, int> marks;
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
    // Getters and setters as needed
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

    void assignMarks(const string& courseCode, int marksValue) {
        // Assign marks for a specific course
        marks[courseCode] = marksValue;
    }
    void displayMarks() const {
        // Display marks for each enrolled course
        cout << "Marks for " << name << ":\n";
        for (const auto& entry : marks) {
            cout << entry.first << ": " << entry.second << " ";
        }
        cout << endl;
    }
    
};

class Course {
private:
    string code;
    string name;
    int capacity;
    vector<Student*> enrolledStudents;
    map<string, int> marks;
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
    void displayEnrolledStudentsMarks() const {
        cout << "Enrolled Students in " << name << " with Marks:\n";
        for (const auto& student : enrolledStudents) {
            cout << "Name: " << student->getName() << "\tRoll Number: " << student->getRollNumber();
            auto it = marks.find(student->getName());
            if (it != marks.end()) {
                cout << "\tMarks: " << it->second;
            }
            cout << endl;
        }
    }
    void assignMarks(const string& courseName, int marks) {
        // Assuming marks is a member of the Student class, you should have a data structure to store marks for each course
        this->marks[courseName] = marks;
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
            for (const auto& studentPtr : course.getEnrolledStudents()) {
                // Display marks for the student
                // For example, you might have a function in the Student class like studentPtr->displayMarks();
                studentPtr->displayMarks();
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
                student->assignMarks(course.getName(), marks);
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

                    studentIt->withdrawFromCourse(studentIt->getEnrolledCourses()[choice]);
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
        cout << endl;


        cout << "Enter student roll number: ";
        cin >> rollNumber;
        cout << endl;
        // Add validation for rollNumber using isValidRollNumber

        cout << "Enter student age: ";
        cin >> age;
        cout << endl;
        // Add validation for age using isValidAge

        cout << "Enter student contact: ";
        cin.ignore(); // Clear buffer
        getline(cin, contact);
        cout << endl;
        // Add validation for contact using isValidContact

        Student newStudent(name, rollNumber, age, contact);
        students.push_back(newStudent);

        cout << "Student enrolled successfully.\n";
        cout << endl;
    }

public:
    System(const string& filename) : fileHandler(filename, "data.txt") {}

    void loadDataFromFile() {
        fileHandler.readFile(students, courses);
    }

    void saveDataToFile() const {
        fileHandler.writeFile(students, courses);
    }
    bool checkButton(const int& x, const int& y)
    {
        // Assuming button size is 20x20 pixels (adjust as needed)
        float buttonWidth = 40.f;
        float buttonHeight = 40.f;

        // Button coordinates
        float enrollX = 120.f;
        float enrollY = 200.f;

        float regisX = 120.f;
        float regisY = 250.f;

        float attendanceX = 120.f;
        float attendanceY = 300.f;

        float marksX = 120.f;
        float marksY = 350.f;

        float withdrawX = 120.f;
        float withdrawY = 400.f;

        float exitX = 120.f;
        float exitY = 450.f;

        if (y >= enrollY && y <= enrollY + buttonHeight)
        {
            if (x >= enrollX && x <= enrollX + buttonWidth)
            {
                enrollStudent();
                return true;
            }
        }
        else if (y >= regisY && y <= regisY + buttonHeight)
        {
            if (x >= regisX && x <= regisX + buttonWidth)
            {
                courseRegistrationMenu();
                return true;
            }
        }
        else if (y >= attendanceY && y <= attendanceY + buttonHeight)
        {
            if (x >= attendanceX && x <= attendanceX + buttonWidth)
            {
                attendanceMenu(); 
                return true;
            }
        }
        else if (y >= marksY && y <= marksY + buttonHeight)
        {
            if (x >= marksX && x <= marksX + buttonWidth)
            {
                marksMenu();
                return true;
            }
        }
        else if (y >= withdrawY && y <= withdrawY + buttonHeight)
        {
            if (x >= withdrawX && x <= withdrawX + buttonWidth)
            {
                courseWithdrawMenu();
                return true;
            }
        }
        else if (y >= exitY && y <= exitY + buttonHeight)
        {
            if (x >= exitX && x <= exitX + buttonWidth)
            {
                exit;
            }
        }

        return false;
    }

    void run()
    {
        loadDataFromFile();
        sf::RenderWindow window(sf::VideoMode(830, 600), "FLEX_2.0", sf::Style::Close);
        // Define two rectangles to cover the window's left and right halves
        // Define two rectangles to cover the window's upper and lower halves
        sf::RectangleShape upperRect(sf::Vector2f(window.getSize().x, window.getSize().y / 8));
        sf::RectangleShape lowerRect(sf::Vector2f(window.getSize().x, 7 * window.getSize().y / 8));

        // Set positions for the rectangles
        upperRect.setPosition(0, 0);
        lowerRect.setPosition(0, window.getSize().y / 8);

        // Set colors for the rectangles
        upperRect.setFillColor(sf::Color::White);
        lowerRect.setFillColor(sf::Color::Blue);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
            }

            window.clear();

            // Draw the rectangles
            window.draw(upperRect);
            window.draw(lowerRect);

       /* while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed)
                    window.close();
            }

            window.clear(sf::Color::Cyan);*/

            // Display your user interface using SFML, e.g., buttons, text, etc.
            // For simplicity, I'm displaying a basic message.
            sf::Font font;
            if (!font.loadFromFile("Fonts/verdanab.ttf")) {
                cerr << "Error: Font not found.\n";
                return;
            }
            sf::Font font2;
            if (!font2.loadFromFile("Fonts/Pacifico.ttf")) {
                cerr << "Error: Font not found.\n";
                return;
            }

            sf::Text text;
            text.setFont(font2);
            text.setString("WELCOME  TO  STUDENT  MANAGMENT  SYSTEM ");
            text.setCharacterSize(28);
            text.setFillColor(sf::Color::Blue);
            text.setPosition(55.f, 30.f);
            window.draw(text);


            sf::Text text2;
            text2.setFont(font);
            text2.setString("MAIN MENU");
            text2.setCharacterSize(24);
            text2.setFillColor(sf::Color::White);
            text2.setPosition(330.f, 130.f);
            window.draw(text2);


            ///////////////////////////



            sf::Text text3;
            text3.setFont(font);
            text3.setString("1. ENROLL A STUDENT");
            text3.setCharacterSize(18);
            text3.setFillColor(sf::Color::Cyan);
            text3.setPosition(120.f, 200.f);
            window.draw(text);
            window.draw(text3);

            ///////////////////////////////


            sf::Text text4;
            text4.setFont(font);
            text4.setString("2. COURSE REGISTERATION");
            text4.setCharacterSize(18);
            text4.setFillColor(sf::Color::Cyan);
            text4.setPosition(120.f, 250.f);
            window.draw(text);
            window.draw(text4);

            ///////////////////////////////////


            sf::Text text5;
            text5.setFont(font);
            text5.setString("3. ATTENDENCE");
            text5.setCharacterSize(18);
            text5.setFillColor(sf::Color::Cyan);
            text5.setPosition(120.f, 300.f);
            window.draw(text);
            window.draw(text5);

            //////////////////////////////////////


            sf::Text text6;
            text6.setFont(font);
            text6.setString("4. MARKS");
            text6.setCharacterSize(18);
            text6.setFillColor(sf::Color::Cyan);
            text6.setPosition(120.f, 350.f);
            window.draw(text);
            window.draw(text6);


            ///////////////////////////////////////

            sf::Text text7;
            text7.setFont(font);
            text7.setString("5. COURSE WITHDRAW");
            text7.setCharacterSize(18);
            text7.setFillColor(sf::Color::Cyan);
            text7.setPosition(120.f, 400.f);
            window.draw(text);
            window.draw(text7);

            /////////////////////////////////////

            sf::Text text8;
            text8.setFont(font);
            text8.setString("6. EXIT");
            text8.setCharacterSize(18);
            text8.setFillColor(sf::Color::Cyan);
            text8.setPosition(120.f, 450.f);
            window.draw(text);
            window.draw(text8);

            //////////////////////////////////////
            // 
            // 
            sf::Text text9;
            text9.setFont(font);
            text9.setString(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::");
            text9.setCharacterSize(40);
            text9.setFillColor(sf::Color::Yellow);
            text9.setPosition(0.f, 500.f);
            window.draw(text);
            window.draw(text9);
            ///////////////////////////////////////////////
            
            sf::Text text10;
            text10.setFont(font);
            text10.setString("::::::::::::::::::::");
            text10.setCharacterSize(40);
            text10.setFillColor(sf::Color::Yellow);
            text10.setPosition(0.f, 530.f);
            window.draw(text);
            window.draw(text10);


            sf::Text text13;
            text13.setFont(font2);
            text13.setString("@FLEX 2.0");
            text13.setCharacterSize(30);
            text13.setFillColor(sf::Color::White);
            text13.setPosition(325.f, 540.f);
            window.draw(text);
            window.draw(text13);

            sf::Text text12;
            text12.setFont(font);
            text12.setString(":::::::::::::::::::::::");
            text12.setCharacterSize(40);
            text12.setFillColor(sf::Color::Yellow);
            text12.setPosition(490.f, 530.f);
            window.draw(text);
            window.draw(text12);

            ////////////////////////////////////////

            sf::Text text11;
            text11.setFont(font);
            text11.setString("::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::");
            text11.setCharacterSize(40);
            text11.setFillColor(sf::Color::Yellow);
            text11.setPosition(0.f, 560.f);
            window.draw(text);
            window.draw(text11);




            //mouse buttons
            sf::Event event2;
            while (window.pollEvent(event2)) {
                if (event2.type == sf::Event::Closed)
                    window.close();

                if (event2.type == sf::Event::MouseButtonPressed) {
                    if (event2.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        cout << "Mouse Clicked at (" << mousePos.x << ", " << mousePos.y << ")\n";
                        if (checkButton(mousePos.x, mousePos.y)) {
                            // Handle button click logic here if needed
                        }
                    }
                }
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
                cout << "        Main Menu\n";
                cout << endl;
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
        }
    };


    //heres 
    int main() {


        System system("data.txt");
        //system.loadDataFromFile();


       // System system("data.txt");
        system.run();
        system.saveDataToFile();
        return 0;
    }

