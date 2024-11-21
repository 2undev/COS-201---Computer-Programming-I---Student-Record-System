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
void addStudent(Student** students, int* totalNumberOfStudent, int* capacity);
void display_all_students(Student* students, int totalNumberOfStudent);
void editStudentInfo(Student* students, int totalNumberOfStudent);
void deleteStudent(Student* students, int* totalNumberOfStudent);
void studentResultStatus(Student student);
void saveDataToFile(Student* students, int totalNumberOfStudent);
void loadDataFromFile(Student** students, int* totalNumberOfStudent, int* capacity);
void searchStudent(Student* students, int totalNumberOfStudent, int studentRollNumber);
void sortStudents(Student* students, int totalNumberOfStudent);
float calculateAverageMark(Student* students, int totalNumberOfStudent);




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
void addStudent(Student** students, int* totalNumberOfStudent, int* capacity) {
    if (*totalNumberOfStudent >= *capacity) {
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

    // Remove the trailing newline character from the name string
    size_t length = strlen(new_student.name);
    if (length > 0 && new_student.name[length - 1] == '\n') {
        new_student.name[length - 1] = '\0';
    }

    int is_unique;
    do {
        is_unique = 1;
        printf("Enter roll number (must be greater than 0): ");
        scanf("%d", &new_student.studentRollNumber);

        if (new_student.studentRollNumber <= 0) {
            printf("Roll number cannot be zero or negative. Please enter a valid roll number.\n");
            is_unique = 0;
            continue;
        }

        for (int i = 0; i < *totalNumberOfStudent; i++) {
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

    (*students)[*totalNumberOfStudent] = new_student;
    (*totalNumberOfStudent)++;
    printf("Student added successfully!\n");
}





// Function to display all student records
void display_all_students(Student* students, int totalNumberOfStudent) {
    if (totalNumberOfStudent == 0) {
        printf("No students to display.\n");
        return;
    }

    for (int i = 0; i < totalNumberOfStudent; i++) {
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
void editStudentInfo(Student* students, int totalNumberOfStudent) {
    int studentRollNumber;
    printf("Enter roll number of student to modify: ");
    scanf("%d", &studentRollNumber);

    for (int i = 0; i < totalNumberOfStudent; i++) {
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





// Function to remove a student by roll number with confirmation
void deleteStudent(Student* students, int* totalNumberOfStudent) {
    int studentRollNumber;
    printf("Enter roll number of student to remove: ");
    scanf("%d", &studentRollNumber);

    for (int i = 0; i < *totalNumberOfStudent; i++) {
        if (students[i].studentRollNumber == studentRollNumber) {
            printf("\nStudent to be deleted:\n");
            printf("Name: %s\n", students[i].name);
            printf("Roll Number: %d\n", students[i].studentRollNumber);
            printf("Marks: %.2f\n", students[i].marks);

            char confirm;
            printf("Are you sure you want to delete this student? (y/n): ");
            getchar();
            scanf("%c", &confirm);

            if (confirm == 'y' || confirm == 'Y') {
                for (int j = i; j < *totalNumberOfStudent - 1; j++) {
                    students[j] = students[j + 1];
                }
                (*totalNumberOfStudent)--;
                printf("Student deleted successfully.\n");
            } else {
                printf("Operation cancelled. Student not deleted.\n");
            }
            return;
        }
    }

    printf("Student with roll number %d not found.\n", studentRollNumber);
}






// Function to save student records to a file
void saveDataToFile(Student* students, int totalNumberOfStudent) {
    FILE *file = fopen("student_records.txt", "w");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    for (int i = 0; i < totalNumberOfStudent; i++) {
        fprintf(file, "%s,%d,%.2f\n", students[i].name, students[i].studentRollNumber, students[i].marks);
    }

    fclose(file);
    printf("Data saved successfully.\n");
}





// Function to load student records from a file
void loadDataFromFile(Student** students, int* totalNumberOfStudent, int* capacity) {
    FILE *file = fopen("student_records.txt", "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    *totalNumberOfStudent = 0;
    while (!feof(file)) {
        if (*totalNumberOfStudent >= *capacity) {
            *capacity *= 2;
            *students = realloc(*students, *capacity * sizeof(Student));
            if (*students == NULL) {
                printf("Memory reallocation failed.\n");
                fclose(file);
                exit(1);
            }
        }

        Student new_student;
        if (fscanf(file, "%49[^,],%d,%f\n", new_student.name, &new_student.studentRollNumber, &new_student.marks) == 3) {
            (*students)[*totalNumberOfStudent] = new_student;
            (*totalNumberOfStudent)++;
        }
    }

    fclose(file);
    printf("Data loaded successfully.\n");
}





// Function to search for a student by roll number
void searchStudent(Student* students, int totalNumberOfStudent, int studentRollNumber) {
    for (int i = 0; i < totalNumberOfStudent; i++) {
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






// Function to sort students by their marks in descending order
void sortStudents(Student* students, int totalNumberOfStudent) {
    if (totalNumberOfStudent == 0) {
        printf("No students to sort.\n");
        return;
    }

    int choice;
    printf("Sort by:\n1. Ascending Order\n2. Descending Order\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    for (int i = 0; i < totalNumberOfStudent - 1; i++) {
        for (int j = i + 1; j < totalNumberOfStudent; j++) {
            int shouldSwap = 0;

            if (choice == 1 && students[i].marks > students[j].marks) {
                shouldSwap = 1;
            } else if (choice == 2 && students[i].marks < students[j].marks) {
                shouldSwap = 1;
            }

            if (shouldSwap) {
                Student temp = students[i];
                students[i] = students[j];
                students[j] = temp;
            }
        }
    }

    printf("Students sorted successfully in %s order.\n",
           (choice == 1) ? "ascending" : "descending");
}






// Function to calculate the average mark of all students
float calculateAverageMark(Student* students, int totalNumberOfStudent) {
    if (totalNumberOfStudent == 0) {
        printf("No students available for average calculation.\n");
        return 0.0;
    }

    float totalMarks = 0;
    for (int i = 0; i < totalNumberOfStudent; i++) {
        totalMarks += students[i].marks;
    }

    return totalMarks / totalNumberOfStudent;
}
