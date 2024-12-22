#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

// Class representing a course with course number, title, and prerequisites
class Course {
public:
    string courseNumber;
    string courseTitle;
    vector<string> prerequisites;

    Course(string number, string title, vector<string> prereqs) {
        courseNumber = number;
        courseTitle = title;
        prerequisites = prereqs;
    }
};

// Node for Binary Search Tree
class Node {
public:
    Course course;
    Node* left;
    Node* right;

    Node(Course course) : course(course), left(nullptr), right(nullptr) {}
};

// Binary Search Tree for storing and managing Course objects
class CourseCatalog {
private:
    Node* root;

    // Helper function to insert a course in the BST
    Node* insert(Node* node, Course course) {
        if (node == nullptr) {
            return new Node(course);
        }

        if (course.courseNumber < node->course.courseNumber) {
            node->left = insert(node->left, course);
        } else {
            node->right = insert(node->right, course);
        }

        return node;
    }

    // Helper function to search for a course by courseNumber in the BST
    Node* search(Node* node, string courseNumber) {
        if (node == nullptr || node->course.courseNumber == courseNumber) {
            return node;
        }

        if (courseNumber < node->course.courseNumber) {
            return search(node->left, courseNumber);
        } else {
            return search(node->right, courseNumber);
        }
    }

    // Helper function for in-order traversal to print all courses
    void inOrderPrint(Node* node) {
        if (node == nullptr) {
            return;
        }

        inOrderPrint(node->left);
        cout << node->course.courseNumber << ", " << node->course.courseTitle << endl;
        inOrderPrint(node->right);
    }

public:
    // Constructor
    CourseCatalog() : root(nullptr) {}

    // Insert a course into the BST
    void insertCourse(Course course) {
        root = insert(root, course);
    }

    // Search for a course by courseNumber
    Course* getCourse(string courseNumber) {
        Node* node = search(root, courseNumber);
        if (node != nullptr) {
            return &(node->course);
        }
        return nullptr;
    }

    // Print all courses in alphanumeric order
    void printAllCourses() {
        cout << "Here is a sample schedule:" << endl;
        inOrderPrint(root);
    }

    // Print course information along with prerequisites
    void printCourseInfo(string courseNumber) {
        Course* course = getCourse(courseNumber);
        if (course == nullptr) {
            cout << "Course not found." << endl;
            return;
        }

        cout << course->courseNumber << ", " << course->courseTitle << endl;
        if (course->prerequisites.empty()) {
            cout << "No prerequisites for this course." << endl;
        } else {
            cout << "Prerequisites: ";
            for (size_t i = 0; i < course->prerequisites.size(); i++) {
                cout << course->prerequisites[i];
                if (i < course->prerequisites.size() - 1) {
                    cout << ", ";
                }
            }
            cout << endl;
        }
    }

    // Load courses from a file into the BST
    bool loadCourses(string filePath) {
        ifstream file(filePath);
        if (!file.is_open()) {
            cout << "Error: Could not open file" << endl;
            return false;
        }

        string line;
        while (getline(file, line)) {
            istringstream stream(line);
            string courseNumber, courseTitle, prereq;
            vector<string> prerequisites;

            // Parse course number and title
            getline(stream, courseNumber, ',');
            getline(stream, courseTitle, ',');

            // Parse prerequisites
            while (getline(stream, prereq, ',')) {
                prerequisites.push_back(prereq);
            }

            // Create and insert course
            Course course(courseNumber, courseTitle, prerequisites);
            insertCourse(course);
        }

        file.close();
        return true;
    }
};

// Display menu options
void displayMenu() {
    cout << "\n1. Load Data Structure." << endl;
    cout << "2. Print Course List." << endl;
    cout << "3. Print Course." << endl;
    cout << "9. Exit" << endl;
    cout << "What would you like to do? ";
}

// Main function to run the program
int main() {
    CourseCatalog catalog;
    int choice;
    string courseNumber;
    string fileName;
    bool dataLoaded = false;

    cout << "Welcome to the course planner." << endl;

    while (true) {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter file name to load data: ";
                cin >> fileName;
                if (catalog.loadCourses(fileName)) {
                    cout << "Data loaded successfully." << endl;
                    dataLoaded = true;
                } else {
                    cout << "Error: Could not load data from file." << endl;
                }
                break;
            case 2:
                if (dataLoaded) {
                    catalog.printAllCourses();
                } else {
                    cout << "Please load data first by selecting option 1." << endl;
                }
                break;
            case 3:
                if (dataLoaded) {
                    cout << "Enter course number: ";
                    cin >> courseNumber;
                    catalog.printCourseInfo(courseNumber);
                } else {
                    cout << "Please load data first by selecting option 1." << endl;
                }
                break;
            case 9:
                cout << "Thank you for using the course planner!" << endl;
                return 0;
            default:
                cout << choice << " is not a valid option." << endl;
        }
    }
}