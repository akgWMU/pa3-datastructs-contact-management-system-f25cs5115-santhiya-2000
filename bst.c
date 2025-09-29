#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAME_LEN 50
#define PHONE_LEN 20
#define EMAIL_LEN 50

// Contact node for BST
typedef struct Contact {
    char name[NAME_LEN];
    char phone[PHONE_LEN];
    char email[EMAIL_LEN];
    struct Contact* left;
    struct Contact* right;
} Contact;

Contact* root = NULL;

// Create a new contact node
Contact* createContact(char* name, char* phone, char* email) {
    Contact* newContact = (Contact*)malloc(sizeof(Contact));
    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    strcpy(newContact->email, email);
    newContact->left = newContact->right = NULL;
    return newContact;
}

// Insert into BST (by name)
Contact* insertContact(Contact* node, char* name, char* phone, char* email) {
    if (node == NULL) {
        printf("Contact added: %s\n", name);
        return createContact(name, phone, email);
    }
    int cmp = strcmp(name, node->name);
    if (cmp < 0)
        node->left = insertContact(node->left, name, phone, email);
    else if (cmp > 0)
        node->right = insertContact(node->right, name, phone, email);
    else
        printf("Contact already exists!\n");
    return node;
}

// Search by name
Contact* searchContact(Contact* node, char* name) {
    if (node == NULL) return NULL;
    int cmp = strcmp(name, node->name);
    if (cmp == 0) return node;
    else if (cmp < 0) return searchContact(node->left, name);
    else return searchContact(node->right, name);
}

// Update a contact
void updateContact(char* name, char* newPhone, char* newEmail) {
    Contact* contact = searchContact(root, name);
    if (contact == NULL) {
        printf("Contact not found.\n");
        return;
    }
    if (newPhone != NULL) strcpy(contact->phone, newPhone);
    if (newEmail != NULL) strcpy(contact->email, newEmail);
    printf("Contact updated: %s\n", name);
}

// Find minimum node (used in deletion)
Contact* findMin(Contact* node) {
    while (node->left != NULL) node = node->left;
    return node;
}

// Delete a contact by name
Contact* deleteContact(Contact* node, char* name) {
    if (node == NULL) {
        printf("Contact not found.\n");
        return NULL;
    }
    int cmp = strcmp(name, node->name);
    if (cmp < 0) {
        node->left = deleteContact(node->left, name);
    } else if (cmp > 0) {
        node->right = deleteContact(node->right, name);
    } else {
        // node found
        if (node->left == NULL && node->right == NULL) {
            free(node);
            return NULL;
        } else if (node->left == NULL) {
            Contact* temp = node->right;
            free(node);
            return temp;
        } else if (node->right == NULL) {
            Contact* temp = node->left;
            free(node);
            return temp;
        } else {
            Contact* successor = findMin(node->right);
            strcpy(node->name, successor->name);
            strcpy(node->phone, successor->phone);
            strcpy(node->email, successor->email);
            node->right = deleteContact(node->right, successor->name);
        }
    }
    return node;
}

// In-order traversal (sorted by name)
void displayContacts(Contact* node) {
    if (node != NULL) {
        displayContacts(node->left);
        printf("Name: %s | Phone: %s | Email: %s\n",
               node->name, node->phone, node->email);
        displayContacts(node->right);
    }
}

// Free memory
void freeContacts(Contact* node) {
    if (node != NULL) {
        freeContacts(node->left);
        freeContacts(node->right);
        free(node);
    }
}

// Menu-driven program
int main() {
    int choice;
    char name[NAME_LEN], phone[PHONE_LEN], email[EMAIL_LEN];

    while (1) {
        printf("\nContact Management System (BST)\n");
        printf("1. Insert Contact\n");
        printf("2. Search Contact\n");
        printf("3. Update Contact\n");
        printf("4. Delete Contact\n");
        printf("5. Display All Contacts (Sorted)\n");
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
                root = insertContact(root, name, phone, email);
                break;
            case 2:
                printf("Enter Name to Search: ");
                fgets(name, NAME_LEN, stdin); name[strcspn(name, "\n")] = 0;
                {
                    Contact* c = searchContact(root, name);
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
                root = deleteContact(root, name);
                break;
            case 5:
                if (root == NULL)
                    printf("No contacts available.\n");
                else
                    displayContacts(root);
                break;
            case 6:
                freeContacts(root);
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}
