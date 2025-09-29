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
    int height;
} Contact;

Contact* root = NULL;

// ----------------- AVL Utilities -----------------
int max(int a, int b){ return (a>b)?a:b; }
int height(Contact* node){ return node?node->height:0; }
int getBalance(Contact* node){ return node?height(node->left)-height(node->right):0; }
Contact* rightRotate(Contact* y){
    Contact* x = y->left;
    Contact* T2 = x->right;
    x->right=y; y->left=T2;
    y->height=1+max(height(y->left),height(y->right));
    x->height=1+max(height(x->left),height(x->right));
    return x;
}
Contact* leftRotate(Contact* x){
    Contact* y=x->right;
    Contact* T2=y->left;
    y->left=x; x->right=T2;
    x->height=1+max(height(x->left),height(x->right));
    y->height=1+max(height(y->left),height(y->right));
    return y;
}
Contact* createContact(char* name,char* phone,char* email){
    Contact* c=(Contact*)malloc(sizeof(Contact));
    strcpy(c->name,name);
    strcpy(c->phone,phone);
    strcpy(c->email,email);
    c->left=c->right=NULL; c->height=1;
    return c;
}

// ----------------- AVL Operations -----------------
Contact* insertContact(Contact* node,char* name,char* phone,char* email){
    if(!node) return createContact(name,phone,email);
    int cmp=strcmp(name,node->name);
    if(cmp<0) node->left=insertContact(node->left,name,phone,email);
    else if(cmp>0) node->right=insertContact(node->right,name,phone,email);
    else return node;

    node->height=1+max(height(node->left),height(node->right));
    int balance=getBalance(node);

    if(balance>1 && strcmp(name,node->left->name)<0) return rightRotate(node);
    if(balance<-1 && strcmp(name,node->right->name)>0) return leftRotate(node);
    if(balance>1 && strcmp(name,node->left->name)>0){ node->left=leftRotate(node->left); return rightRotate(node); }
    if(balance<-1 && strcmp(name,node->right->name)<0){ node->right=rightRotate(node->right); return leftRotate(node); }

    return node;
}

Contact* searchContact(Contact* node,char* name){
    if(!node) return NULL;
    int cmp=strcmp(name,node->name);
    if(cmp==0) return node;
    else if(cmp<0) return searchContact(node->left,name);
    else return searchContact(node->right,name);
}

void updateContact(char* name,char* phone,char* email){
    Contact* c=searchContact(root,name);
    if(!c) return;
    if(phone) strcpy(c->phone,phone);
    if(email) strcpy(c->email,email);
}

Contact* findMin(Contact* node){ while(node->left) node=node->left; return node; }

Contact* deleteContact(Contact* node,char* name){
    if(!node) return NULL;
    int cmp=strcmp(name,node->name);
    if(cmp<0) node->left=deleteContact(node->left,name);
    else if(cmp>0) node->right=deleteContact(node->right,name);
    else{
        if(!node->left || !node->right){
            Contact* temp=node->left?node->left:node->right;
            if(!temp){ free(node); return NULL; }
            else{ *node=*temp; free(temp); }
        }else{
            Contact* succ=findMin(node->right);
            strcpy(node->name,succ->name);
            strcpy(node->phone,succ->phone);
            strcpy(node->email,succ->email);
            node->right=deleteContact(node->right,succ->name);
        }
    }
    node->height=1+max(height(node->left),height(node->right));
    int balance=getBalance(node);
    if(balance>1 && getBalance(node->left)>=0) return rightRotate(node);
    if(balance>1 && getBalance(node->left)<0){ node->left=leftRotate(node->left); return rightRotate(node); }
    if(balance<-1 && getBalance(node->right)<=0) return leftRotate(node);
    if(balance<-1 && getBalance(node->right)>0){ node->right=rightRotate(node->right); return leftRotate(node); }
    return node;
}

void freeContacts(Contact* node){ if(!node) return; freeContacts(node->left); freeContacts(node->right); free(node); }
void displayContacts(Contact* node){ if(!node) return; displayContacts(node->left); printf("Name:%s|Phone:%s|Email:%s\n",node->name,node->phone,node->email); displayContacts(node->right); }

// ----------------- Random Data -----------------
void randomString(char* str,int len){
    static const char charset[]="abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for(int i=0;i<len-1;i++) str[i]=charset[rand()%(sizeof(charset)-1)];
    str[len-1]='\0';
}
Contact generateContact(){
    Contact c; randomString(c.name,10); randomString(c.phone,10); randomString(c.email,15); return c;
}

// ----------------- Benchmark -----------------
// void benchmarkAVL(){
//     int sizes[]={100,500,1000};
//     int ntests=sizeof(sizes)/sizeof(sizes[0]);
//     srand(time(NULL));
//     printf("DataStructure,Operation,Contacts,Time_ms\n");
//     for(int t=0;t<ntests;t++){
//         int n=sizes[t]; freeContacts(root); root=NULL;
//         Contact temp; Contact* stored[n];

//         // Insert
//         clock_t start=clock();
//         for(int i=0;i<n;i++){ temp=generateContact(); root=insertContact(root,temp.name,temp.phone,temp.email); stored[i]=searchContact(root,temp.name);}
//         clock_t end=clock();
//         printf("AVL,Insert,%d,%.3f\n",n,((double)(end-start))/CLOCKS_PER_SEC*1000);

//         // Search
//         start=clock();
//         for(int i=0;i<n;i++) searchContact(root,stored[i]->name);
//         end=clock();
//         printf("AVL,Search,%d,%.3f\n",n,((double)(end-start))/CLOCKS_PER_SEC*1000);

//         // Update
//         start=clock();
//         for(int i=0;i<n;i++) updateContact(stored[i]->name,"1234567890","newemail@test.com");
//         end=clock();
//         printf("AVL,Update,%d,%.3f\n",n,((double)(end-start))/CLOCKS_PER_SEC*1000);

//         // Delete
//         start=clock();
//         for(int i=0;i<n;i++) root=deleteContact(root,stored[i]->name);
//         end=clock();
//         printf("AVL,Delete,%d,%.3f\n",n,((double)(end-start))/CLOCKS_PER_SEC*1000);
//     }
// }

// ---------------- Benchmark (AVL to CSV) ----------------
void benchmarkAVLToCSV(const char* filename) {
    int sizes[] = {100, 500, 1000};
    int ntests = sizeof(sizes)/sizeof(sizes[0]);
    srand(time(NULL));

    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Cannot open file %s\n", filename);
        return;
    }

    fprintf(fp, "DataStructure,Operation,Contacts,Time_ms\n");

    for (int t = 0; t < ntests; t++) {
        int n = sizes[t];
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
        fprintf(fp, "AVL,Insert,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

        // ---------- Search ----------
        start = clock();
        for (int i = 0; i < n; i++)
            searchContact(root, stored[i]->name);
        end = clock();
        fprintf(fp, "AVL,Search,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

        // ---------- Update ----------
        start = clock();
        for (int i = 0; i < n; i++)
            updateContact(stored[i]->name, "1234567890", "newemail@test.com");
        end = clock();
        fprintf(fp, "AVL,Update,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);

        // ---------- Delete ----------
        start = clock();
        for (int i = 0; i < n; i++)
            root = deleteContact(root, stored[i]->name);
        end = clock();
        fprintf(fp, "AVL,Delete,%d,%.3f\n", n, ((double)(end-start))/CLOCKS_PER_SEC*1000);
    }

    fclose(fp);
    printf("AVL benchmark written to %s\n", filename);
}


// ----------------- Main Menu -----------------
int main(){
    int choice; char name[NAME_LEN], phone[PHONE_LEN], email[EMAIL_LEN]; srand(time(NULL));
    while(1){
        printf("\nContact Management System (AVL Tree)\n1.Insert\n2.Search\n3.Update\n4.Delete\n5.Display\n6.Benchmark\n7.Exit\nEnter choice:");
        scanf("%d",&choice); getchar();
        switch(choice){
            case 1: printf("Name: "); fgets(name,NAME_LEN,stdin); name[strcspn(name,"\n")]=0;
                    printf("Phone: "); fgets(phone,PHONE_LEN,stdin); phone[strcspn(phone,"\n")]=0;
                    printf("Email: "); fgets(email,EMAIL_LEN,stdin); email[strcspn(email,"\n")]=0;
                    root=insertContact(root,name,phone,email); break;
            case 2: printf("Name: "); fgets(name,NAME_LEN,stdin); name[strcspn(name,"\n")]=0;
                    { Contact* c=searchContact(root,name); if(c) printf("Found:%s|%s|%s\n",c->name,c->phone,c->email); else printf("Not found\n");} break;
            case 3: printf("Name: "); fgets(name,NAME_LEN,stdin); name[strcspn(name,"\n")]=0;
                    printf("Phone: "); fgets(phone,PHONE_LEN,stdin); phone[strcspn(phone,"\n")]=0;
                    printf("Email: "); fgets(email,EMAIL_LEN,stdin); email[strcspn(email,"\n")]=0;
                    updateContact(name,phone,email); break;
            case 4: printf("Name: "); fgets(name,NAME_LEN,stdin); name[strcspn(name,"\n")]=0;
                    root=deleteContact(root,name); break;
            case 5: if(!root) printf("No contacts.\n"); else displayContacts(root); break;
            case 6: benchmarkAVLToCSV("performance_avl.csv"); break;
            case 7: freeContacts(root); exit(0);
            default: printf("Invalid choice\n");
        }
    }
}
