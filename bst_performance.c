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
    struct Contact* left;
    struct Contact* right;
} Contact;

Contact* root = NULL;

// ------------------- BST Operations -------------------
Contact* createContact(char* name, char* phone, char* email) {
    Contact* newContact = (Contact*)malloc(sizeof(Contact));
    strcpy(newContact->name, name);
    strcpy(newContact->phone, phone);
    strcpy(newContact->email, email);
    newContact->left = newContact->right = NULL;
    return newContact;
}

Contact* insertContact(Contact* node, char* name, char* phone, char* email) {
    if (node == NULL) return createContact(name, phone, email);
    int cmp = strcmp(name, node->name);
    if (cmp < 0) node->left = insertContact(node->left, name, phone, email);
    else if (cmp > 0) node->right = insertContact(node->right, name, phone, email);
    return node;
}

Contact* searchContact(Contact* node, char* name) {
    if (node == NULL) return NULL;
    int cmp = strcmp(name, node->name);
    if (cmp == 0) return node;
    else if (cmp < 0) return searchContact(node->left, name);
    else return searchContact(node->right, name);
}

void updateContact(char* name, char* newPhone, char* newEmail) {
    Contact* contact = searchContact(root, name);
    if (!contact) return;
    if (newPhone) strcpy(contact->phone, newPhone);
    if (newEmail) strcpy(contact->email, newEmail);
}

Contact* findMin(Contact* node) {
    while (node->left) node = node->left;
    return node;
}

Contact* deleteContact(Contact* node, char* name) {
    if (!node) return NULL;
    int cmp = strcmp(name, node->name);
    if (cmp < 0) node->left = deleteContact(node->left, name);
    else if (cmp > 0) node->right = deleteContact(node->right, name);
    else {
        if (!node->left && !node->right) { free(node); return NULL; }
        else if (!node->left) { Contact* temp = node->right; free(node); return temp; }
        else if (!node->right) { Contact* temp = node->left; free(node); return temp; }
        else {
            Contact* succ = findMin(node->right);
            strcpy(node->name, succ->name);
            strcpy(node->phone, succ->phone);
            strcpy(node->email, succ->email);
            node->right = deleteContact(node->right, succ->name);
        }
    }
    return node;
}

void freeContacts(Contact* node) {
    if (!node) return;
    freeContacts(node->left);
    freeContacts(node->right);
    free(node);
}

// ------------------- Random Contact Generator -------------------
void randomString(char *str, int length) {
    static const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < length-1; i++)
        str[i] = charset[rand() % (sizeof(charset)-1)];
    str[length-1] = '\0';
}

Contact generateContact() {
    Contact c;
    randomString(c.name, 10);
    randomString(c.phone, 10);
    randomString(c.email, 15);
    return c;
}

// ------------------- Benchmark -------------------
// void benchmarkBST() {
//     int test_sizes[] = {100, 500, 1000};
//     int num_tests = sizeof(test_sizes)/sizeof(test_sizes[0]);
//     srand(time(NULL));

//     printf("DataStructure,Operation,Contacts,Time_ms\n");

//     for (int t = 0; t < num_tests; t++) {
//         int n = test_sizes[t];
//         freeContacts(root);
//         root = NULL;

//         Contact temp;
//         Contact* stored[n];

//         // Insert
//         clock_t start = clock();
//         for (int i=0;i<n;i++){
//             temp = generateContact();
//             root = insertContact(root, temp.name, temp.phone, temp.email);
//             stored[i] = searchContact(root, temp.name);
//         }
//         clock_t end = clock();
//         printf("BST,Insert,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

//         // Search
//         start = clock();
//         for(int i=0;i<n;i++)
//             searchContact(root, stored[i]->name);
//         end = clock();
//         printf("BST,Search,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

//         // Update
//         start = clock();
//         for(int i=0;i<n;i++)
//             updateContact(stored[i]->name,"1234567890","newemail@test.com");
//         end = clock();
//         printf("BST,Update,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

//         // Delete
//         start = clock();
//         for(int i=0;i<n;i++)
//             root = deleteContact(root, stored[i]->name);
//         end = clock();
//         printf("BST,Delete,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);
//     }
// }

// ---------------- Benchmark (BST to CSV) ----------------
void benchmarkBSTToCSV(const char* filename) {
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
        freeContacts(root);
        root = NULL;

        Contact temp;
        Contact* stored[n];

        // ---------- Insert ----------
        clock_t start = clock();
        for (int i = 0; i < n; i++) {
            temp = generateContact();
            root = insertContact(root, temp.name, temp.phone, temp.email);
            stored[i] = searchContact(root, temp.name);
        }
        clock_t end = clock();
        fprintf(fp, "BST,Insert,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

        // ---------- Search ----------
        start = clock();
        for (int i = 0; i < n; i++)
            searchContact(root, stored[i]->name);
        end = clock();
        fprintf(fp, "BST,Search,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

        // ---------- Update ----------
        start = clock();
        for (int i = 0; i < n; i++)
            updateContact(stored[i]->name, "1234567890", "newemail@test.com");
        end = clock();
        fprintf(fp, "BST,Update,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

        // ---------- Delete ----------
        start = clock();
        for (int i = 0; i < n; i++)
            root = deleteContact(root, stored[i]->name);
        end = clock();
        fprintf(fp, "BST,Delete,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);
    }

    fclose(fp);
    printf("BST benchmark written to %s\n", filename);
}


// ------------------- Display Contacts (In-order) -------------------
void displayContacts(Contact* node) {
    if (node != NULL) {
        displayContacts(node->left);
        printf("Name: %s | Phone: %s | Email: %s\n",
               node->name, node->phone, node->email);
        displayContacts(node->right);
    }
}

// ------------------- Main Menu -------------------
int main() {
    int choice;
    char name[NAME_LEN], phone[PHONE_LEN], email[EMAIL_LEN];
    srand(time(NULL));

    while(1){
        printf("\nContact Management System (BST)\n");
        printf("1. Insert Contact\n2. Search Contact\n3. Update Contact\n4. Delete Contact\n");
        printf("5. Display Contacts\n6. Benchmark Performance\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d",&choice);
        getchar();

        switch(choice){
            case 1:
                printf("Enter Name: "); fgets(name,NAME_LEN,stdin); name[strcspn(name,"\n")]=0;
                printf("Enter Phone: "); fgets(phone,PHONE_LEN,stdin); phone[strcspn(phone,"\n")]=0;
                printf("Enter Email: "); fgets(email,EMAIL_LEN,stdin); email[strcspn(email,"\n")]=0;
                root = insertContact(root,name,phone,email);
                printf("Contact added: %s\n",name);
                break;
            case 2:
                printf("Enter Name to Search: "); fgets(name,NAME_LEN,stdin); name[strcspn(name,"\n")]=0;
                {
                    Contact* c = searchContact(root,name);
                    if(c) printf("Found: %s | %s | %s\n",c->name,c->phone,c->email);
                    else printf("Not found.\n");
                }
                break;
            case 3:
                printf("Enter Name to Update: "); fgets(name,NAME_LEN,stdin); name[strcspn(name,"\n")]=0;
                printf("Enter New Phone: "); fgets(phone,PHONE_LEN,stdin); phone[strcspn(phone,"\n")]=0;
                printf("Enter New Email: "); fgets(email,EMAIL_LEN,stdin); email[strcspn(email,"\n")]=0;
                updateContact(name,phone,email);
                printf("Contact updated: %s\n",name);
                break;
            case 4:
                printf("Enter Name to Delete: "); fgets(name,NAME_LEN,stdin); name[strcspn(name,"\n")]=0;
                root = deleteContact(root,name);
                printf("Contact deleted: %s\n",name);
                break;
            case 5:
                if (root == NULL)
                    printf("No contacts available.\n");
                else
                    displayContacts(root);
                break;
            case 6:
                printf("Running benchmark...\n");
                benchmarkBSTToCSV("performance_bst.csv");
                break;
            case 7:
                freeContacts(root);
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    return 0;
}
