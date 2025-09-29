#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_CONTACTS 1000
#define NAME_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 50

typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
} Contact;

Contact contacts[MAX_CONTACTS];
int contactCount = 0;

// ---------------- Core Functions ----------------
void insertContact(char *name, char *phone, char *email) {
    strcpy(contacts[contactCount].name, name);
    strcpy(contacts[contactCount].phone, phone);
    strcpy(contacts[contactCount].email, email);
    contactCount++;
}

int searchContact(char *name) {
    for (int i = 0; i < contactCount; i++) {
        if (strcmp(contacts[i].name, name) == 0) return i;
    }
    return -1;
}

void updateContact(char *name, char *newPhone, char *newEmail) {
    int index = searchContact(name);
    if (index == -1) return;
    if (newPhone != NULL) strcpy(contacts[index].phone, newPhone);
    if (newEmail != NULL) strcpy(contacts[index].email, newEmail);
}

void deleteContact(char *name) {
    int index = searchContact(name);
    if (index == -1) return;
    for (int i = index; i < contactCount - 1; i++) contacts[i] = contacts[i + 1];
    contactCount--;
}

void displayContacts() {
    printf("\n--- Contact List ---\n");
    for (int i = 0; i < contactCount; i++)
        printf("Name: %s | Phone: %s | Email: %s\n",
               contacts[i].name, contacts[i].phone, contacts[i].email);
    if (contactCount == 0) printf("No contacts available.\n");
    printf("---------------------\n");
}

// ---------------- Random Contact Generator ----------------
void randomString(char *str, int length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < length - 1; i++)
        str[i] = charset[rand() % (sizeof(charset) - 1)];
    str[length - 1] = '\0';
}

Contact generateContact() {
    Contact c;
    randomString(c.name, 10);
    randomString(c.phone, 10);
    randomString(c.email, 15);
    return c;
}

// // ---------------- Benchmark Function ----------------
// void benchmarkArray() {
//     int test_sizes[] = {100, 500, 1000};
//     int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);
//     srand(time(NULL));

//     printf("DataStructure,Operation,Contacts,Time_ms\n");

//     for (int t = 0; t < num_tests; t++) {
//         int n = test_sizes[t];
//         contactCount = 0;

//         Contact temp;

//         // -------- Insert Benchmark --------
//         clock_t start = clock();
//         for (int i = 0; i < n; i++) {
//             temp = generateContact();
//             insertContact(temp.name, temp.phone, temp.email);
//         }
//         clock_t end = clock();
//         printf("Array,Insert,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

//         // -------- Search Benchmark --------
//         start = clock();
//         for (int i = 0; i < n; i++) {
//             searchContact(contacts[i].name);
//         }
//         end = clock();
//         printf("Array,Search,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

//         // -------- Update Benchmark --------
//         start = clock();
//         for (int i = 0; i < n; i++) {
//             updateContact(contacts[i].name, "1234567890", "newemail@test.com");
//         }
//         end = clock();
//         printf("Array,Update,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

//         // -------- Delete Benchmark --------
//         start = clock();
//         for (int i = n - 1; i >= 0; i--) {
//             deleteContact(contacts[i].name);
//         }
//         end = clock();
//         printf("Array,Delete,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
//     }
// }

// ---------------- Benchmark Function ----------------
void benchmarkArrayToCSV(const char* filename) {
    int test_sizes[] = {100, 500, 1000};
    int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);
    srand(time(NULL));

    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    fprintf(fp, "DataStructure,Operation,Contacts,Time_ms\n");

    for (int t = 0; t < num_tests; t++) {
        int n = test_sizes[t];
        contactCount = 0;

        Contact temp;

        // -------- Insert Benchmark --------
        clock_t start = clock();
        for (int i = 0; i < n; i++) {
            temp = generateContact();
            insertContact(temp.name, temp.phone, temp.email);
        }
        clock_t end = clock();
        fprintf(fp, "Array,Insert,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

        // -------- Search Benchmark --------
        start = clock();
        for (int i = 0; i < n; i++) {
            searchContact(contacts[i].name);
        }
        end = clock();
        fprintf(fp, "Array,Search,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

        // -------- Update Benchmark --------
        start = clock();
        for (int i = 0; i < n; i++) {
            updateContact(contacts[i].name, "1234567890", "newemail@test.com");
        }
        end = clock();
        fprintf(fp, "Array,Update,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

        // -------- Delete Benchmark --------
        start = clock();
        for (int i = n - 1; i >= 0; i--) {
            deleteContact(contacts[i].name);
        }
        end = clock();
        fprintf(fp, "Array,Delete,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
    }

    fclose(fp);
    printf("Benchmark written to %s\n", filename);
}


// ---------------- Main Menu ----------------
int main() {
    int choice;
    char name[NAME_LEN], phone[PHONE_LEN], email[EMAIL_LEN];

    while (1) {
        printf("\nContact Management System (Array-based)\n");
        printf("1. Insert Contact\n");
        printf("2. Search Contact\n");
        printf("3. Update Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Display All Contacts\n");
        printf("6. Benchmark Performance\n");
        printf("7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter Name: "); fgets(name, NAME_LEN, stdin); name[strcspn(name,"\n")]=0;
                printf("Enter Phone: "); fgets(phone, PHONE_LEN, stdin); phone[strcspn(phone,"\n")]=0;
                printf("Enter Email: "); fgets(email, EMAIL_LEN, stdin); email[strcspn(email,"\n")]=0;
                insertContact(name, phone, email);
                printf("Contact added: %s\n", name);
                break;
            case 2:
                printf("Enter Name to Search: "); fgets(name, NAME_LEN, stdin); name[strcspn(name,"\n")]=0;
                int idx; idx = searchContact(name);
                if (idx != -1)
                    printf("Found: %s | Phone: %s | Email: %s\n",
                           contacts[idx].name, contacts[idx].phone, contacts[idx].email);
                else printf("Contact not found.\n");
                break;
            case 3:
                printf("Enter Name to Update: "); fgets(name, NAME_LEN, stdin); name[strcspn(name,"\n")]=0;
                printf("Enter New Phone: "); fgets(phone, PHONE_LEN, stdin); phone[strcspn(phone,"\n")]=0;
                printf("Enter New Email: "); fgets(email, EMAIL_LEN, stdin); email[strcspn(email,"\n")]=0;
                updateContact(name, phone, email);
                printf("Contact updated: %s\n", name);
                break;
            case 4:
                printf("Enter Name to Delete: "); fgets(name, NAME_LEN, stdin); name[strcspn(name,"\n")]=0;
                deleteContact(name);
                printf("Contact deleted: %s\n", name);
                break;
            case 5:
                displayContacts();
                break;
            case 6:
                printf("Running benchmark...\n");
                benchmarkArrayToCSV("performance_array.csv");
                break;
            case 7:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
