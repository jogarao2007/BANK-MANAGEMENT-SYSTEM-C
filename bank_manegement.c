#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifdef _WIN32
    #define CLEAR "cls"
#else
    #define CLEAR "clear"
#endif

// Structure to store date
struct date {
    int month, day, year;
};

// Structure to store account details
struct account {
    char name[60];
    int acc_no, age;
    char address[60];
    char citizenship[15];
    double phone;
    char acc_type[10];
    float amt;
    struct date dob;
    struct date deposit;
    struct date withdraw;
} add, upd, check, rem, transaction;

int main_exit;

// Function to calculate simple interest
float interest(float t, float amount, int rate) {
    return (rate * t * amount) / 100.0;
}

// Function to clear the screen
void clear_screen() {
    system(CLEAR);
}

// Function to pause until user presses Enter
void pause_screen() {
    printf("\nPress Enter to continue...");
    getchar();
}

// Flush leftover input buffer
void flush_input() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Case-insensitive string comparison
int stricmp(const char *a, const char *b) {
    for (; *a && *b; a++, b++) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return (tolower((unsigned char)*a) - tolower((unsigned char)*b));
    }
    return *a - *b;
}

// Function declarations
void menu();
void close_app();

// Function to close application
void close_app() {
    clear_screen();
    printf("\n\n\tThank you for using the Bank Management System.\n");
    exit(0);
}

// Function to create a new account
void new_acc() {
    FILE *ptr = fopen("record.txt", "a+");  // changed file extension to .txt
    if (ptr == NULL) {
        printf("Error opening file.\n");
        return;
    }

    clear_screen();
    printf("\t\t--- CREATE NEW ACCOUNT ---\n");

    printf("\nEnter today's date (mm dd yyyy): ");
    scanf("%d %d %d", &add.deposit.month, &add.deposit.day, &add.deposit.year);
    flush_input();

    printf("Enter account number: ");
    scanf("%d", &add.acc_no);
    flush_input();

    // Check for duplicate account number
    struct account temp;
    while (fscanf(ptr, "%d %s %d/%d/%d %d %s %s %lf %s %f %d/%d/%d",
                  &temp.acc_no, temp.name, &temp.dob.month, &temp.dob.day, &temp.dob.year,
                  &temp.age, temp.address, temp.citizenship, &temp.phone, temp.acc_type,
                  &temp.amt, &temp.deposit.month, &temp.deposit.day, &temp.deposit.year) != EOF) {
        if (temp.acc_no == add.acc_no) {
            printf("Account number already exists!\n");
            fclose(ptr);
            pause_screen();
            return;
        }
    }

    printf("Enter name: ");
    fgets(add.name, sizeof(add.name), stdin);
    add.name[strcspn(add.name, "\n")] = '\0';

    printf("Enter DOB (mm dd yyyy): ");
    scanf("%d %d %d", &add.dob.month, &add.dob.day, &add.dob.year);
    flush_input();

    printf("Enter age: ");
    scanf("%d", &add.age);
    flush_input();

    printf("Enter address: ");
    fgets(add.address, sizeof(add.address), stdin);
    add.address[strcspn(add.address, "\n")] = '\0';

    printf("Enter citizenship number: ");
    scanf("%s", add.citizenship);

    printf("Enter phone number: ");
    scanf("%lf", &add.phone);

    printf("Enter amount to deposit: $");
    scanf("%f", &add.amt);
    flush_input();

    printf("Enter account type (saving/current/fixed1/fixed2/fixed3): ");
    scanf("%s", add.acc_type);

    fprintf(ptr, "%d %s %d/%d/%d %d %s %s %.0lf %s %.2f %d/%d/%d\n",
            add.acc_no, add.name, add.dob.month, add.dob.day, add.dob.year, add.age,
            add.address, add.citizenship, add.phone, add.acc_type, add.amt,
            add.deposit.month, add.deposit.day, add.deposit.year);

    fclose(ptr);
    printf("\nAccount created successfully!\n");
    pause_screen();
    menu();
}

// Function to view all customer accounts
void view_list() {
    FILE *view = fopen("record.txt", "r");  // changed file extension to .txt
    if (view == NULL) {
        printf("Error: No records found.\n");
        pause_screen();
        return;
    }

    clear_screen();
    printf("\nACC. NO.\tNAME\t\tADDRESS\t\tPHONE\n");

    int found = 0;
    while (fscanf(view, "%d %s %d/%d/%d %d %s %s %lf %s %f %d/%d/%d",
                  &add.acc_no, add.name, &add.dob.month, &add.dob.day, &add.dob.year,
                  &add.age, add.address, add.citizenship, &add.phone, add.acc_type,
                  &add.amt, &add.deposit.month, &add.deposit.day, &add.deposit.year) != EOF) {
        printf("\n%6d\t%-10s\t%-10s\t%.0lf\n", add.acc_no, add.name, add.address, add.phone);
        found++;
    }

    if (!found)
        printf("\nNo records found.\n");

    fclose(view);
    pause_screen();
    menu();
}

// Main menu
void menu() {
    int choice;
    clear_screen();
    printf("\n\t--- BANK MANAGEMENT SYSTEM ---\n");
    printf("\n1. Create new account");
    printf("\n2. View customer list");
    printf("\n3. Exit");
    printf("\n\nEnter your choice: ");
    scanf("%d", &choice);
    flush_input();

    switch (choice) {
        case 1: new_acc(); break;
        case 2: view_list(); break;
        case 3: close_app(); break;
        default:
            printf("Invalid choice!\n");
            pause_screen();
            menu();
    }
}

// Program entry point
int main() {
    char pass[20];
    const char password[20] = "admin123";

    clear_screen();
    printf("\nEnter password to login: ");
    scanf("%s", pass);
    flush_input();

    if (strcmp(pass, password) == 0) {
        printf("\nLogin successful!\n");
        pause_screen();
        menu();
    } else {
        printf("\nWrong password!\n");
        pause_screen();
        return 0;
    }

    return 0;
}
