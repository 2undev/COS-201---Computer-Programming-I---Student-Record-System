#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define a structure for student information
typedef struct {
    char name[50];
    int studentRollNumber;
    float marks;
} Student;

// Function prototypes
void greetUser();
void addStudent(Student** students, int* count, int* capacity);
void display_all_students(Student* students, int count);
void editStudentInfo(Student* students, int count);
void deleteStudent(Student* students, int* count);
void studentResultStatus(Student student);
void saveDataToFile(Student* students, int count);
void loadDataFromFile(Student** students, int* count, int* capacity);
void searchStudent(Student* students, int count, int studentRollNumber);
void sortStudents(Student* students, int count);
float calculateAverageMark(Student* students, int count);

int main() {
    greetUser();

    Student *students = NULL;  // Pointer to store students dynamically
    int totalNumberOfStudent = 0;  // Tracks the number of students
    int capacity = 1;  // Initial capacity of the student array
    int option;

    // Allocate initial memory for one student
    students = (Student*)malloc(capacity * sizeof(Student));
    if (students == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    do {
        printf("\n1. Add Student\n2. Display All Students\n3. Modify Student\n4. Remove Student\n5. Save Records\n6. Load Records\n7. Search by Roll Number\n8. Sort by Marks\n9. Calculate Average Marks\n10. Exit\n");
        
        printf("Choose an option from the list of options above:\n");
        scanf("%d", &option);
        getchar();  // Consume the newline left by scanf

        switch(option) {
            case 1:
                addStudent(&students, &totalNumberOfStudent, &capacity);
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
                loadDataFromFile(&students, &totalNumberOfStudent, &capacity);
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

    // Free dynamically allocated memory for students
    free(students);

    return 0;
}

// Function to greet the user
void greetUser() {
    char name[50];
    printf("Welcome to the Student Record System!\n");
    printf("Please enter your full name: ");
    fgets(name, sizeof(name), stdin);
    printf("Hello, %s\n", name);
}

// Function to add a new student record
void addStudent(Student** students, int* count, int* capacity) {
    if (*count >= *capacity) {
        *capacity *= 2;  // Double the capacity
        *students = realloc(*students, *capacity * sizeof(Student));
        if (*students == NULL) {
            printf("Memory reallocation failed.\n");
            exit(1);
        }
    }

    Student new_student;
    printf("Enter student full name: ");
    fgets(new_student.name, sizeof(new_student.name), stdin);

    int is_unique;
    do {
        is_unique = 1;
        printf("Enter roll number: ");
        scanf("%d", &new_student.studentRollNumber);

        for (int i = 0; i < *count; i++) {
            if ((*students)[i].studentRollNumber == new_student.studentRollNumber) {
                printf("Roll number %d is already taken. Please enter a different roll number.\n", new_student.studentRollNumber);
                is_unique = 0;
                break;
            }
        }
    } while (!is_unique);

    do {
        printf("Enter marks (0 - 100): ");
        scanf("%f", &new_student.marks);
        if (new_student.marks < 0 || new_student.marks > 100) {
            printf("Invalid mark. Please enter a mark between 0 and 100.\n");
        }
    } while (new_student.marks < 0 || new_student.marks > 100);

    (*students)[*count] = new_student;
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
            int choice;
            printf("What would you like to edit?\n1. Name\n2. Marks\n3. Both\n");
            printf("Enter your choice: ");
            scanf("%d", &choice);
            getchar();

            if (choice == 1 || choice == 3) {
                printf("Enter new name: ");
                fgets(students[i].name, sizeof(students[i].name), stdin);
            }

            if (choice == 2 || choice == 3) {
                float new_marks;
                do {
                    printf("Enter new marks (0 - 100): ");
                    scanf("%f", &new_marks);
                    if (new_marks < 0 || new_marks > 100) {
                        printf("Marks must be between 0 and 100. Please try again.\n");
                    }
                } while (new_marks < 0 || new_marks > 100);
                students[i].marks = new_marks;
            }

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

// Function to save student records to a file
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
    printf("Records saved to file successfully.\n");
}

// Function to load student records from a file
void loadDataFromFile(Student** students, int* count, int* capacity) {
    FILE *file = fopen("student_records.txt", "r");
    if (file == NULL) {
        printf("Error opening file for reading.\n");
        return;
    }

    *count = 0;
    char buffer[100];
    while (fgets(buffer, sizeof(buffer), file)) {
        if (*count >= *capacity) {
            *capacity *= 2;
            *students = realloc(*students, *capacity * sizeof(Student));
            if (*students == NULL) {
                printf("Memory reallocation failed.\n");
                fclose(file);
                return;
            }
        }

        sscanf(buffer, "Name: %[^\n]\n", (*students)[*count].name);
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "Roll number: %d\n", &(*students)[*count].studentRollNumber);
        fgets(buffer, sizeof(buffer), file);
        sscanf(buffer, "Student mark: %f\n", &(*students)[*count].marks);
        *count += 1;

        fgets(buffer, sizeof(buffer), file);  // Skip the "Status" line
    }

    fclose(file);
    printf("Records loaded from file successfully.\n");
}

// Function to search for a student by roll number
void searchStudent(Student* students, int count, int studentRollNumber) {
    for (int i = 0; i < count; i++) {
        if (students[i].studentRollNumber == studentRollNumber) {
            printf("Name: %s\n", students[i].name);
            printf("Roll Number: %d\n", students[i].studentRollNumber);
            printf("Marks: %.2f\n", students[i].marks);
            studentResultStatus(students[i]);
            return;
        }
    }

    printf("Student with roll number %d not found.\n", studentRollNumber);
}

// Function to sort students by marks
void sortStudents(Student* students, int count) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (students[i].marks < students[j].marks) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }
    printf("Students sorted by marks in descending order.\n");
}

// Function to calculate the average mark of all students
float calculateAverageMark(Student* students, int count) {
    if (count == 0) {
        printf("No students to calculate average.\n");
        return 0.0;
    }

    float total = 0.0;
    for (int i = 0; i < count; i++) {
        total += students[i].marks;
    }

    return total / count;
}
