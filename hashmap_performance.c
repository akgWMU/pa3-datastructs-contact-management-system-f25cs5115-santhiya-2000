#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAME_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 50
#define TABLE_SIZE 101   // prime number for better distribution

typedef struct Contact {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    struct Contact* next;
} Contact;

Contact* hashTable[TABLE_SIZE] = {NULL};

// ---------------- Hash Functions ----------------
unsigned int hash(char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash % TABLE_SIZE;
}

Contact* createContact(char* name, char* phone, char* email) {
    Contact* newContact = (Contact*)malloc(sizeof(Contact));
    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    strcpy(newContact->email, email);
    newContact->next = NULL;
    return newContact;
}

// ---------------- Core Operations ----------------
void insertContact(char* name, char* phone, char* email) {
    unsigned int index = hash(name);
    Contact* newContact = createContact(name, phone, email);
    newContact->next = hashTable[index];
    hashTable[index] = newContact;
}

Contact* searchContact(char* name) {
    unsigned int index = hash(name);
    Contact* temp = hashTable[index];
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) return temp;
        temp = temp->next;
    }
    return NULL;
}

void updateContact(char* name, char* newPhone, char* newEmail) {
    Contact* contact = searchContact(name);
    if (contact == NULL) return;
    if (newPhone != NULL) strcpy(contact->phone, newPhone);
    if (newEmail != NULL) strcpy(contact->email, newEmail);
}

void deleteContact(char* name) {
    unsigned int index = hash(name);
    Contact* temp = hashTable[index];
    Contact* prev = NULL;

    while (temp != NULL && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }
    if (temp == NULL) return;
    if (prev == NULL) hashTable[index] = temp->next;
    else prev->next = temp->next;
    free(temp);
}

void displayContacts() {
    printf("\n--- Contact List ---\n");
    int empty = 1;
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact* temp = hashTable[i];
        while (temp != NULL) {
            printf("Name: %s | Phone: %s | Email: %s\n",
                   temp->name, temp->phone, temp->email);
            temp = temp->next;
            empty = 0;
        }
    }
    if (empty) printf("No contacts available.\n");
    printf("---------------------\n");
}

void freeContacts() {
    for (int i = 0; i < TABLE_SIZE; i++) {
        Contact* temp = hashTable[i];
        while (temp != NULL) {
            Contact* next = temp->next;
            free(temp);
            temp = next;
        }
        hashTable[i] = NULL;
    }
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

// // ---------------- Benchmark ----------------
// void benchmarkHashMap() {
//     int test_sizes[] = {100, 500, 1000};
//     int num_tests = sizeof(test_sizes)/sizeof(test_sizes[0]);
//     srand(time(NULL));

//     printf("DataStructure,Operation,Contacts,Time_ms\n");

//     for (int t = 0; t < num_tests; t++) {
//         int n = test_sizes[t];
//         freeContacts();  // start fresh

//         Contact temp;
//         Contact* stored[n];  // store pointers for search/update/delete

//         // ---------- Insert ----------
//         clock_t start = clock();
//         for (int i = 0; i < n; i++) {
//             temp = generateContact();
//             insertContact(temp.name, temp.phone, temp.email);
//             stored[i] = searchContact(temp.name); // store reference for search/update/delete
//         }
//         clock_t end = clock();
//         printf("HashMap,Insert,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

//         // ---------- Search ----------
//         start = clock();
//         for (int i = 0; i < n; i++)
//             searchContact(stored[i]->name);
//         end = clock();
//         printf("HashMap,Search,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

//         // ---------- Update ----------
//         start = clock();
//         for (int i = 0; i < n; i++)
//             updateContact(stored[i]->name, "1234567890", "newemail@test.com");
//         end = clock();
//         printf("HashMap,Update,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

//         // ---------- Delete ----------
//         start = clock();
//         for (int i = 0; i < n; i++)
//             deleteContact(stored[i]->name);
//         end = clock();
//         printf("HashMap,Delete,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);
//     }
// }
// ---------------- Benchmark (Hash Map to CSV) ----------------
void benchmarkHashMapToCSV(const char* filename) {
    int test_sizes[] = {100, 500, 1000};
    int num_tests = sizeof(test_sizes)/sizeof(test_sizes[0]);
    srand(time(NULL));

    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    fprintf(fp, "DataStructure,Operation,Contacts,Time_ms\n");

    for (int t = 0; t < num_tests; t++) {
        int n = test_sizes[t];
        freeContacts();  // start fresh

        Contact temp;
        Contact* stored[n];  // store pointers for search/update/delete

        // ---------- Insert ----------
        clock_t start = clock();
        for (int i = 0; i < n; i++) {
            temp = generateContact();
            insertContact(temp.name, temp.phone, temp.email);
            stored[i] = searchContact(temp.name); // store reference
        }
        clock_t end = clock();
        fprintf(fp, "HashMap,Insert,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

        // ---------- Search ----------
        start = clock();
        for (int i = 0; i < n; i++)
            searchContact(stored[i]->name);
        end = clock();
        fprintf(fp, "HashMap,Search,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

        // ---------- Update ----------
        start = clock();
        for (int i = 0; i < n; i++)
            updateContact(stored[i]->name, "1234567890", "newemail@test.com");
        end = clock();
        fprintf(fp, "HashMap,Update,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

        // ---------- Delete ----------
        start = clock();
        for (int i = 0; i < n; i++)
            deleteContact(stored[i]->name);
        end = clock();
        fprintf(fp, "HashMap,Delete,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);
    }

    fclose(fp);
    printf("Hash Map benchmark written to %s\n", filename);
}

// ---------------- Main Menu ----------------
int main() {
    int choice;
    char name[NAME_LEN], phone[PHONE_LEN], email[EMAIL_LEN];

    while (1) {
        printf("\nContact Management System (Hash Map)\n");
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
                {
                    Contact* c = searchContact(name);
                    if (c != NULL)
                        printf("Found: %s | Phone: %s | Email: %s\n", c->name, c->phone, c->email);
                    else printf("Contact not found.\n");
                }
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
                benchmarkHashMapToCSV("performance_hashmap.csv");
                break;
            case 7:
                freeContacts();
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
