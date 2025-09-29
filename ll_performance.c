#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NAME_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 50

typedef struct Contact {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    struct Contact* next;
} Contact;

Contact* head = NULL;

// ---------------- Core Functions ----------------
Contact* createContact(char* name, char* phone, char* email) {
    Contact* newContact = (Contact*)malloc(sizeof(Contact));
    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    strcpy(newContact->email, email);
    newContact->next = NULL;
    return newContact;
}

void insertContact(char* name, char* phone, char* email) {
    Contact* newContact = createContact(name, phone, email);
    if (head == NULL) head = newContact;
    else {
        Contact* temp = head;
        while (temp->next != NULL) temp = temp->next;
        temp->next = newContact;
    }
}

Contact* searchContact(char* name) {
    Contact* temp = head;
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
    Contact* temp = head;
    Contact* prev = NULL;

    while (temp != NULL && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return;

    if (prev == NULL) head = temp->next;
    else prev->next = temp->next;

    free(temp);
}

void displayContacts() {
    printf("\n--- Contact List ---\n");
    Contact* temp = head;
    if (temp == NULL) printf("No contacts available.\n");
    while (temp != NULL) {
        printf("Name: %s | Phone: %s | Email: %s\n",
               temp->name, temp->phone, temp->email);
        temp = temp->next;
    }
    printf("---------------------\n");
}

void freeContacts() {
    Contact* temp = head;
    while (temp != NULL) {
        Contact* next = temp->next;
        free(temp);
        temp = next;
    }
    head = NULL;
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
// void benchmarkLinkedList() {
//     int test_sizes[] = {100, 500, 1000};
//     int num_tests = sizeof(test_sizes) / sizeof(test_sizes[0]);
//     srand(time(NULL));

//     printf("DataStructure,Operation,Contacts,Time_ms\n");

//     for (int t = 0; t < num_tests; t++) {
//         int n = test_sizes[t];
//         freeContacts();  // start fresh

//         Contact temp;

//         // ---------- Insert ----------
//         clock_t start = clock();
//         for (int i = 0; i < n; i++) {
//             temp = generateContact();
//             insertContact(temp.name, temp.phone, temp.email);
//         }
//         clock_t end = clock();
//         printf("LinkedList,Insert,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

//         // ---------- Search ----------
//         Contact* curr = head;
//         start = clock();
//         while (curr != NULL) {
//             searchContact(curr->name);
//             curr = curr->next;
//         }
//         end = clock();
//         printf("LinkedList,Search,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

//         // ---------- Update ----------
//         curr = head;
//         start = clock();
//         while (curr != NULL) {
//             updateContact(curr->name, "1234567890", "newemail@test.com");
//             curr = curr->next;
//         }
//         end = clock();
//         printf("LinkedList,Update,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

//         // ---------- Delete ----------
//         curr = head;
//         start = clock();
//         while (head != NULL) {
//             deleteContact(head->name);  // always delete head
//         }
//         end = clock();
//         printf("LinkedList,Delete,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
//     }
// }

// ---------------- Benchmark (Linked List to CSV) ----------------
void benchmarkLinkedListToCSV(const char* filename) {
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
        freeContacts();  // start fresh

        Contact temp;

        // ---------- Insert ----------
        clock_t start = clock();
        for (int i = 0; i < n; i++) {
            temp = generateContact();
            insertContact(temp.name, temp.phone, temp.email);
        }
        clock_t end = clock();
        fprintf(fp, "LinkedList,Insert,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

        // ---------- Search ----------
        Contact* curr = head;
        start = clock();
        while (curr != NULL) {
            searchContact(curr->name);
            curr = curr->next;
        }
        end = clock();
        fprintf(fp, "LinkedList,Search,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

        // ---------- Update ----------
        curr = head;
        start = clock();
        while (curr != NULL) {
            updateContact(curr->name, "1234567890", "newemail@test.com");
            curr = curr->next;
        }
        end = clock();
        fprintf(fp, "LinkedList,Update,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);

        // ---------- Delete ----------
        start = clock();
        while (head != NULL) {
            deleteContact(head->name);  // always delete head
        }
        end = clock();
        fprintf(fp, "LinkedList,Delete,%d,%.3f\n", n, ((double)(end - start)) / CLOCKS_PER_SEC * 1000);
    }

    fclose(fp);
    printf("Linked List benchmark written to %s\n", filename);
}


// ---------------- Main Menu ----------------
int main() {
    int choice;
    char name[NAME_LEN], phone[PHONE_LEN], email[EMAIL_LEN];

    while (1) {
        printf("\nContact Management System (Linked List)\n");
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
                Contact* c; c = searchContact(name);
                if (c != NULL)
                    printf("Found: %s | Phone: %s | Email: %s\n", c->name, c->phone, c->email);
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
                benchmarkLinkedListToCSV("performance_ll.csv");
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
