#include<stdio.h>
#include<stdlib.h>
#include<string.h>

// typedef struct{
//   char word[81];

// }wordindex;

typedef int elementtype; 
typedef struct node node;
typedef struct node{ 
  elementtype element; 
  node *next; 
} node; 
node *root; 
node *cur;
node *prev;

node *makenewNode(elementtype addr);
void insertAtHead(elementtype addr);
void insertTail(elementtype addr);
void insertAfterCurrent(elementtype e);
void insertBeforeCurrent(elementtype e);
void deleteFirstElement(node *root);
node *insertAtPosition(elementtype ad, int n);
node *deleteAtPosition(int n);
void getLinkData();
node *list_reverse (node* root);
void freelist();

node *makenewNode(elementtype addr){
  node *newNode = (node*)malloc(sizeof(node));
  newNode->element = addr;
  newNode->next = NULL;
  return newNode;
}

void insertAtHead(elementtype addr){
  node *newNode = makenewNode(addr);
  newNode->next = root;
  root = newNode;
  cur = root;
}

void insertTail(elementtype addr){
  node *newNode = makenewNode(addr);
  if(root == NULL)
    root = newNode;
  else{
    cur = root;
    while(cur->next != NULL){
      cur = cur->next;
    }
    cur->next = newNode;
  }
}

void insertAfterCurrent(elementtype e) {
  node* newNode = makenewNode(e);
  if(root == NULL){
    root = newNode;
    cur = root;
  }else {
    newNode->next=cur->next;
    cur->next = newNode;
    // prev=cur;
    cur = cur->next;
  }
}

void insertBeforeCurrent(elementtype e) {
  node *newNode = makenewNode(e);
  node *tmp = root;
  while (tmp->next != cur && cur != NULL)
    tmp=tmp->next;
  prev = tmp;
  if ( root == NULL ) {
    root = newNode;
    cur = root;
    prev = NULL;
  }else {
    newNode->next=cur;
    if (cur==root){
      root = newNode; /* nut moi them vao tro thanh dau danh sach */
    }
    else prev->next = newNode;
    cur = newNode;
  }
}

/*elementtype readData(){
  elementtype res;
  printf("stt:");
  scanf("%d",&res.stt);
  printf("tel:");
  gets(res.key);
  printf("email:");
  gets(res.email);
  return res;
}

void printData(elementtype res){
  printf("%15s\t%10s\t%20s\n",res.name, res.tel, res.email);
  }

void traverseList() {
  node* p;
  for (p = root; p != NULL; p = p->next ) {
    print(p->element);
  }
  }*/

void deleteFirstElement(node *root){
  node *del = root;
  root = root->next;
  free(del);
  cur = root;
}

void deleteCurrenElement(node *root){
  if(cur == NULL) return;
  if(cur == root) 
    deleteFirstElement(root);
  else{
    prev->next = cur->next;
    free(cur);
    cur = prev->next;
  }
}

node *list_reverse (node* root){
  node *cur, *prev;
  cur = prev = NULL;
  while (root != NULL) {
    cur = root;
    root = root->next;
    cur->next = prev;
    prev = cur;
  }
  return prev;
}

node *insertAtPosition(elementtype ad, int n){
  int i=0;
  node *p = root; 
  node *newNode, *prev = p;
  if (n==0 || root==NULL) {
    newNode = makenewNode(ad);
    newNode->next=root;
    root=newNode;
    cur= newNode;
    return newNode;
  }
  while ((i<n) && (p->next!= NULL)){
    prev=p;
    p=p->next;
    i++;
  } 
  if (i<n)
    prev = p;
  newNode = makenewNode(ad);
  newNode->next = prev->next;
  prev->next = newNode;
  cur = newNode;
  return newNode;
}

node *deleteAtPosition(int n){
  int i=0;
  node *p = root; 
  node *del, *prev =p;
  if (n==0 || root==NULL) {
    del = root;
    root=del->next;
    cur= root;
    free(del);
    cur=root;
    return root;
  }
  while ((i<n) && (p->next!= NULL)){
    prev=p;
    p=p->next;
    i++;
  } 
  if (i<n)
    return root;
  prev->next = p->next;
  free(p);
  cur=root;
  return root;
}

// void getLinkData(){
//   printf("\n");
//   for (node *p = root; p != NULL; p = p->next){
//     printf("%2d %-20s\t %3d\t %3d\t %6d\n",(p->element).id, (p->element).ten, (p->element).num, (p->element).goal_thang,(p->element).goal_thua);
//   }
// }

void freelist(){
  node *del = root;
  while(root != NULL){
    root = root->next;
    free(del);
    del = root;
  }
}
