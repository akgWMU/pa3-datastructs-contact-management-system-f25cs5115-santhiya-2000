#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 50
#define TABLE_SIZE 101   // prime number for better distribution

// Contact structure (node in chaining linked list)
typedef struct Contact {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    struct Contact* next;
} Contact;

// Hash table (array of pointers to Contact)
Contact* hashTable[TABLE_SIZE];

// Hash function (djb2 algorithm)
unsigned int hash(char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    return hash % TABLE_SIZE;
}

// Create a new contact node
Contact* createContact(char* name, char* phone, char* email) {
    Contact* newContact = (Contact*)malloc(sizeof(Contact));
    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    strcpy(newContact->email, email);
    newContact->next = NULL;
    return newContact;
}

// Insert a contact into the hash table
void insertContact(char* name, char* phone, char* email) {
    unsigned int index = hash(name);
    Contact* newContact = createContact(name, phone, email);

    // Insert at head of linked list (separate chaining)
    newContact->next = hashTable[index];
    hashTable[index] = newContact;

    printf("Contact added: %s\n", name);
}

// Search for a contact by name
Contact* searchContact(char* name) {
    unsigned int index = hash(name);
    Contact* temp = hashTable[index];
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

// Update a contact
void updateContact(char* name, char* newPhone, char* newEmail) {
    Contact* contact = searchContact(name);
    if (contact == NULL) {
        printf("Contact not found.\n");
        return;
    }
    if (newPhone != NULL) strcpy(contact->phone, newPhone);
    if (newEmail != NULL) strcpy(contact->email, newEmail);
    printf("Contact updated: %s\n", name);
}

// Delete a contact by name
void deleteContact(char* name) {
    unsigned int index = hash(name);
    Contact* temp = hashTable[index];
    Contact* prev = NULL;

    while (temp != NULL && strcmp(temp->name, name) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Contact not found.\n");
        return;
    }

    if (prev == NULL) {
        hashTable[index] = temp->next; // remove head
    } else {
        prev->next = temp->next;
    }

    free(temp);
    printf("Contact deleted: %s\n", name);
}

// Display all contacts in the hash table
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

// Free all memory
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

// Menu-driven program
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
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // consume newline

        switch (choice) {
            case 1:
                printf("Enter Name: ");
                fgets(name, NAME_LEN, stdin); name[strcspn(name, "\n")] = 0;
                printf("Enter Phone: ");
                fgets(phone, PHONE_LEN, stdin); phone[strcspn(phone, "\n")] = 0;
                printf("Enter Email: ");
                fgets(email, EMAIL_LEN, stdin); email[strcspn(email, "\n")] = 0;
                insertContact(name, phone, email);
                break;
            case 2:
                printf("Enter Name to Search: ");
                fgets(name, NAME_LEN, stdin); name[strcspn(name, "\n")] = 0;
                {
                    Contact* c = searchContact(name);
                    if (c != NULL)
                        printf("Found: %s | Phone: %s | Email: %s\n",
                               c->name, c->phone, c->email);
                    else
                        printf("Contact not found.\n");
                }
                break;
            case 3:
                printf("Enter Name to Update: ");
                fgets(name, NAME_LEN, stdin); name[strcspn(name, "\n")] = 0;
                printf("Enter New Phone: ");
                fgets(phone, PHONE_LEN, stdin); phone[strcspn(phone, "\n")] = 0;
                printf("Enter New Email: ");
                fgets(email, EMAIL_LEN, stdin); email[strcspn(email, "\n")] = 0;
                updateContact(name, phone, email);
                break;
            case 4:
                printf("Enter Name to Delete: ");
                fgets(name, NAME_LEN, stdin); name[strcspn(name, "\n")] = 0;
                deleteContact(name);
                break;
            case 5:
                displayContacts();
                break;
            case 6:
                freeContacts();
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
