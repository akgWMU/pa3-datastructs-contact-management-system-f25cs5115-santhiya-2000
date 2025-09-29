#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CONTACTS 1000
#define NAME_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 50

// Contact structure
typedef struct {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
} Contact;

// Global array to store contacts
Contact contacts[MAX_CONTACTS];
int contactCount = 0;

// Function to insert a contact
void insertContact(char *name, char *phone, char *email) {
    strcpy(contacts[contactCount].name, name);
    strcpy(contacts[contactCount].phone, phone);
    strcpy(contacts[contactCount].email, email);
    contactCount++;
    printf("Contact added: %s\n", name);
}

// Function to search for a contact by name
int searchContact(char *name) {
    for (int i = 0; i < contactCount; i++) {
        if (strcmp(contacts[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to update a contact
void updateContact(char *name, char *newPhone, char *newEmail) {
    int index = searchContact(name);
    if (index == -1) {
        printf("Contact not found.\n");
        return;
    }
    if (newPhone != NULL) strcpy(contacts[index].phone, newPhone);
    if (newEmail != NULL) strcpy(contacts[index].email, newEmail);
    printf("Contact updated: %s\n", name);
}

// Function to delete a contact
void deleteContact(char *name) {
    int index = searchContact(name);
    if (index == -1) {
        printf("Contact not found.\n");
        return;
    }
    for (int i = index; i < contactCount - 1; i++) {
        contacts[i] = contacts[i + 1];
    }
    contactCount--;
    printf("Contact deleted: %s\n", name);
}

// Function to display all contacts
void displayContacts() {
    printf("\n--- Contact List ---\n");
    for (int i = 0; i < contactCount; i++) {
        printf("Name: %s | Phone: %s | Email: %s\n",
               contacts[i].name, contacts[i].phone, contacts[i].email);
    }
    if (contactCount == 0) {
        printf("No contacts available.\n");
    }
    printf("---------------------\n");
}

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
                int idx;
                idx = searchContact(name);
                if (idx != -1)
                    printf("Found: %s | Phone: %s | Email: %s\n",
                           contacts[idx].name, contacts[idx].phone, contacts[idx].email);
                else
                    printf("Contact not found.\n");
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
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
