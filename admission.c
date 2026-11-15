#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RED     "\033[1;31m"
#define GREEN   "\033[1;32m"
#define YELLOW  "\033[1;33m"
#define CYAN    "\033[1;36m"
#define RESET   "\033[0m"

struct Student {
    int id;
    char name[50];
    int age;
    char course[50];
    float marks;
};

void clearInput() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// 🔍 Check duplicate ID
int isDuplicate(int id) {
    FILE *fp = fopen("record.txt", "r");
    struct Student s;

    if (!fp) return 0;

    while (fscanf(fp,
        "ID:%d | Name:%49[^|] | Age:%d | Course:%49[^|] | Percentage:%f\n",
        &s.id, s.name, &s.age, s.course, &s.marks) == 5) {

        if (s.id == id) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// 🔐 Login
int login() {
    char user[20], pass[20], u[20], p[20];
    FILE *fp = fopen("users.txt", "r");

    if (!fp) {
        printf(RED "Error: users.txt not found!\n" RESET);
        return 0;
    }

    printf(CYAN "\n===== LOGIN =====\n" RESET);

    printf("Username: ");
    scanf("%19s", user);

    printf("Password: ");
    scanf("%19s", pass);

    while (fscanf(fp, "%19s %19s", u, p) == 2) {
        if (strcmp(user, u) == 0 && strcmp(pass, p) == 0) {
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

// ➕ Add
void addStudent() {
    FILE *fp = fopen("record.txt", "a");
    struct Student s;

    if (!fp) {
        printf(RED "File error!\n" RESET);
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &s.id);

    if (isDuplicate(s.id)) {
        printf(YELLOW "ID already exists!\n" RESET);
        fclose(fp);
        return;
    }

    printf("Enter Name: ");
    scanf("%49s", s.name);

    printf("Enter Age: ");
    scanf("%d", &s.age);

    printf("Enter Course: ");
    scanf("%49s", s.course);

    printf("Enter Intermediate Percentage: ");
    scanf("%f", &s.marks);

    fprintf(fp,
        "ID:%d | Name:%s | Age:%d | Course:%s | Percentage:%.2f\n",
        s.id, s.name, s.age, s.course, s.marks);

    fclose(fp);
    printf(GREEN "✅ Record Added!\n" RESET);
}

// 🔍 View
void viewStudentByID() {
    FILE *fp = fopen("record.txt", "r");
    struct Student s;
    int id, found = 0;

    if (!fp) {
        printf(YELLOW "No records found!\n" RESET);
        return;
    }

    printf("Enter ID: ");
    scanf("%d", &id);

    while (fscanf(fp,
        "ID:%d | Name:%49[^|] | Age:%d | Course:%49[^|] | Percentage:%f\n",
        &s.id, s.name, &s.age, s.course, &s.marks) == 5) {

        if (s.id == id) {
            printf(GREEN "\n===== STUDENT FOUND =====\n" RESET);
            printf(RED "ID: %d\nName: %s\nAge: %d\nCourse: %s\nPercentage: %.2f\n",
                   s.id, s.name, s.age, s.course, s.marks);
            found = 1;
            break;
        }
    }

    if (!found)
        printf(RED "❌ Record not found!\n" RESET);

    fclose(fp);
}

// ✏️ Modify
void modifyStudent() {
    FILE *fp = fopen("record.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    struct Student s;
    int id, found = 0;

    if (!fp || !temp) return;

    printf("Enter ID: ");
    scanf("%d", &id);

    while (fscanf(fp,
        "ID:%d | Name:%49[^|] | Age:%d | Course:%49[^|] | Percentage:%f\n",
        &s.id, s.name, &s.age, s.course, &s.marks) == 5) {

        if (s.id == id) {
            printf("New Name: "); scanf("%49s", s.name);
            printf("New Age: "); scanf("%d", &s.age);
            printf("New Course: "); scanf("%49s", s.course);
            printf("New Percentage: "); scanf("%f", &s.marks);
            found = 1;
        }

        fprintf(temp,
            "ID:%d | Name:%s | Age:%d | Course:%s | Percentage:%.2f\n",
            s.id, s.name, s.age, s.course, s.marks);
    }

    fclose(fp);
    fclose(temp);

    remove("record.txt");
    rename("temp.txt", "record.txt");

    if (found)
        printf(GREEN "Updated!\n" RESET);
    else
        printf(RED "Not found!\n" RESET);
}

// ❌ Delete
void deleteStudent() {
    FILE *fp = fopen("record.txt", "r");
    FILE *temp = fopen("temp.txt", "w");

    struct Student s;
    int id, found = 0;

    if (!fp || !temp) return;

    printf("Enter ID: ");
    scanf("%d", &id);

    while (fscanf(fp,
        "ID:%d | Name:%49[^|] | Age:%d | Course:%49[^|] | Percentage:%f\n",
        &s.id, s.name, &s.age, s.course, &s.marks) == 5) {

        if (s.id != id) {
            fprintf(temp,
                "ID:%d | Name:%s | Age:%d | Course:%s | Percentage:%.2f\n",
                s.id, s.name, s.age, s.course, s.marks);
        } else {
            found = 1;
        }
    }

    fclose(fp);
    fclose(temp);

    remove("record.txt");
    rename("temp.txt", "record.txt");

    if (found)
        printf(GREEN "Deleted!\n" RESET);
    else
        printf(RED "Not found!\n" RESET);
}

// 📌 Menu (unchanged)
void menu() {
    int choice;

    while (1) {
        printf(CYAN "\n====================================================\n" RESET);
        printf(CYAN "           STUDENT ADMISSION SYSTEM\n" RESET);
        printf(CYAN "====================================================\n" RESET);

        printf(GREEN "   [1] Register New Student\n" RESET);
        printf(GREEN "   [2] View Student Details\n" RESET);
        printf(GREEN "   [3] Update Student Record\n" RESET);
        printf(GREEN "   [4] Cancel Admission (Delete)\n" RESET);
        printf(GREEN "   [5] Exit System\n" RESET);

        printf(YELLOW "\n>> Enter your choice: " RESET);

        if (scanf("%d", &choice) != 1) {
            printf(RED "\n[ERROR] Invalid input! Enter a number.\n" RESET);
            clearInput();
            continue;
        }

        printf("\n");

        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewStudentByID(); break;
            case 3: modifyStudent(); break;
            case 4: deleteStudent(); break;
            case 5: exit(0);
            default: printf(RED "[ERROR] Invalid choice! Try again.\n" RESET);
        }
    }
}

int main() {
    if (login())
        menu();
    else
        printf("Login Failed\n");

    return 0;
}