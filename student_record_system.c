#include <stdio.h>

#define MAX_NUMBER_OF_STUDENTS 100  // Define the maximum number of students

// Define a structure for student information
typedef struct {
    char name[50];
    int studentRollNumber;
    float marks;
} Student;

// Function prototypes
void greetUser();
void addStudent(Student* students, int* count);
void display_all_students(Student* students, int count);
void editStudentInfo(Student* students, int count);
void deleteStudent(Student* students, int* count);
void studentResultStatus(Student student);
void saveDataToFile(Student* students, int count);
void loadDataFromFile(Student* students, int* count);
void searchStudent(Student* students, int count, int studentRollNumber);
void sortStudents(Student* students, int count);
float calculateAverageMark(Student* students, int count);

int main() {
    greetUser();

    Student students[MAX_NUMBER_OF_STUDENTS];  // Array to store students
    int totalNumberOfStudent = 0;  // Keeps track of the number of students

    int option;

    do {
        printf("\n1. Add Student\n2. Display All Students\n3. Modify Student\n4. Remove Student\n5. Save Records\n6. Load Records\n7. Search by Roll Number\n8. Sort by Marks\n9. Calculate Average Marks\n10. Exit\n");
        printf("Choose an option from the list of options above:\n");
        scanf("%d", &option);

        switch(option) {
            case 1:
                addStudent(students, &totalNumberOfStudent);
                break;
            case 2:
                display_all_students(students, totalNumberOfStudent);
                break;
            case 3:
                editStudentInfo(students, totalNumberOfStudent);
                break;
            case 4:
                deleteStudent(students, &totalNumberOfStudent);
                break;
            case 5:
                saveDataToFile(students, totalNumberOfStudent);
                break;
            case 6:
                loadDataFromFile(students, &totalNumberOfStudent);
                break;
            case 7: {
                int roll;
                printf("Enter roll number to search: ");
                scanf("%d", &roll);
                searchStudent(students, totalNumberOfStudent, roll);
                break;
            }
            case 8:
                sortStudents(students, totalNumberOfStudent);
                break;
            case 9:
                printf("Average Marks: %.2f\n", calculateAverageMark(students, totalNumberOfStudent));
                break;
            case 10:
                printf("Exiting...\n");
                break;
            default:
                printf("Invalid option. Try again.\n");
        }
    } while(option != 10);

    return 0;
}

// Function to greet the user
void greetUser() {
    char name[50];
    printf("Welcome to the Student Record System!\n");
    printf("Please enter your name: ");
    scanf("%s", name);
    printf("Hello, %s!\n", name);
}


// Function to add a new student record with unique roll number check and marks validation
void addStudent(Student* students, int* count) {
    if (*count >= MAX_NUMBER_OF_STUDENTS) {
        printf("Maximum student capacity reached.\n");
        return;
    }

    Student new_student;
    printf("Enter student name: ");
    scanf("%s", new_student.name);

    int is_unique;
    do {
        is_unique = 1;  // Assume roll number is unique
        printf("Enter roll number: ");
        scanf("%d", &new_student.studentRollNumber);

        // Check for unique roll number
        for (int i = 0; i < *count; i++) {
            if (students[i].studentRollNumber == new_student.studentRollNumber) {
                printf("Roll number %d is already taken. Please enter a different roll number.\n", new_student.studentRollNumber);
                is_unique = 0;
                break;
            }
        }
    } while (!is_unique);

    // Validate marks input
    do {
        printf("Enter marks (0 - 100): ");
        scanf("%f", &new_student.marks);
        if (new_student.marks < 0 || new_student.marks > 100) {
            printf("Invalid mark. Please enter a mark between 0 and 100.\n");
        }
    } while (new_student.marks < 0 || new_student.marks > 100);

    students[*count] = new_student;
    (*count)++;
    printf("Student added successfully!\n");
}

// Function to display all student records
void display_all_students(Student* students, int count) {
    if (count == 0) {
        printf("No students to display.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        printf("\nStudent %d:\n", i + 1);
        printf("Name: %s\n", students[i].name);
        printf("Roll Number: %d\n", students[i].studentRollNumber);
        printf("Marks: %.2f\n", students[i].marks);
        studentResultStatus(students[i]);
    }
}

// Function to check if a student has passed or failed
void studentResultStatus(Student student) {
    if (student.marks >= 40) {
        printf("Status: Passed\n");
    } else {
        printf("Status: Failed\n");
    }
}

// Function to modify a student's record by roll number
void editStudentInfo(Student* students, int count) {
    int studentRollNumber;
    printf("Enter roll number of student to modify: ");
    scanf("%d", &studentRollNumber);

    for (int i = 0; i < count; i++) {
        if (students[i].studentRollNumber == studentRollNumber) {
            printf("Enter new name: ");
            scanf("%s", students[i].name);
            printf("Enter new marks: ");
            scanf("%f", &students[i].marks);
            printf("Student record updated successfully!\n");
            return;
        }
    }

    printf("Student with roll number %d not found.\n", studentRollNumber);
}

// Function to remove a student by roll number
void deleteStudent(Student* students, int* count) {
    int studentRollNumber;
    printf("Enter roll number of student to remove: ");
    scanf("%d", &studentRollNumber);

    for (int i = 0; i < *count; i++) {
        if (students[i].studentRollNumber == studentRollNumber) {
            for (int j = i; j < *count - 1; j++) {
                students[j] = students[j + 1];
            }
            (*count)--;
            printf("Student removed successfully!\n");
            return;
        }
    }

    printf("Student with roll number %d not found.\n", studentRollNumber);
}

// Function to save student records to a file with formatted output
void saveDataToFile(Student* students, int count) {
    FILE *file = fopen("student_records.txt", "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "Name: %s\n", students[i].name);
        fprintf(file, "Roll number: %d\n", students[i].studentRollNumber);
        fprintf(file, "Student mark: %.2f\n", students[i].marks);
        fprintf(file, "Status: %s\n\n", (students[i].marks >= 40) ? "Pass" : "Fail");
    }

    fclose(file);
    printf("Records saved to file successfully in the new format.\n");
}

// Function to load student records from a file
void loadDataFromFile(Student* students, int* count) {
    FILE *file = fopen("student_records.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    *count = 0;
    while (fscanf(file, "%s %d %f", students[*count].name, &students[*count].studentRollNumber, &students[*count].marks) != EOF) {
        (*count)++;
    }

    fclose(file);
    printf("Records loaded from file successfully.\n");
}

// Function to search for a student by roll number
void searchStudent(Student* students, int count, int studentRollNumber) {
    for (int i = 0; i < count; i++) {
        if (students[i].studentRollNumber == studentRollNumber) {
            printf("\nStudent found:\n");
            printf("Name: %s\n", students[i].name);
            printf("Roll Number: %d\n", students[i].studentRollNumber);
            printf("Marks: %.2f\n", students[i].marks);
            studentResultStatus(students[i]);
            return;
        }
    }

    printf("Student with roll number %d not found.\n", studentRollNumber);
}

// Function to sort students by marks in ascending or descending order
void sortStudents(Student* students, int count) {
    int order;
    printf("Choose sorting order:\n1. Ascending\n2. Descending\n");
    printf("Enter 1 or 2: ");
    scanf("%d", &order);

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            int shouldSwap = 0;
            if (order == 1 && students[i].marks > students[j].marks) {  // Ascending order
                shouldSwap = 1;
            } else if (order == 2 && students[i].marks < students[j].marks) {  // Descending order
                shouldSwap = 1;
            }

            if (shouldSwap) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }

    if (order == 1) {
        printf("Students sorted by marks in ascending order.\n");
    } else if (order == 2) {
        printf("Students sorted by marks in descending order.\n");
    } else {
        printf("Invalid option. Sorting not performed.\n");
    }
}

// Function to calculate the average marks
float calculateAverageMark(Student* students, int count) {
    if (count == 0) {
        printf("No students to calculate average.\n");
        return 0;
    }

    float total = 0;
    for (int i = 0; i < count; i++) {
        total += students[i].marks;
    }

    return total / count;
}

