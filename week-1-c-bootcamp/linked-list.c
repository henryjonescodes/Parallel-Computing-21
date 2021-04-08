//linked-list.c
//author: John Rieffel
#include <stdio.h>
#include <stdlib.h>
// ------ a node has              \
// | ---|--->a pointer-to-node field 
// ------   a data field             
// | 17 | and it has the type-name: Node   
//------
typedef struct node {
    struct node *next;
    int data;           
    } Node;

Node * insert(int N, Node *listptr)
{
    //allocate space for new Node

   struct node *tmpptr = (struct node *) malloc(sizeof(Node));
   tmpptr->data = N;
   tmpptr->next = NULL;

   if (listptr == NULL || N < listptr->data)
   {
      tmpptr->next = listptr;
      listptr = tmpptr;
   }
   else 
   {
      struct node *curptr = listptr;
      while (curptr->next != NULL && curptr->next->data <= N)
      {
          curptr = curptr->next;
      }
      tmpptr->next = curptr->next;
      curptr->next = tmpptr;
   }
   return listptr;
}

Node * insertRecursive(int N, Node *listptr)
{
    struct node *head = listptr;
    if(listptr->data <= N){
        struct node *tmpptr = (struct node *) malloc(sizeof(Node));
        tmpptr->data = N;
        tmpptr->next = listptr->next;
        listptr->next = tmpptr;  
        return head;
   } else {
       insertRecursive(N, listptr->next);
   }
}

void PrintList(Node *listptr){
   if(listptr == NULL){
       printf("\n");   
       return;
   } else {
       printf("%d", listptr->data);
       if(listptr->next != NULL){
           printf(" -> ");
       }
       PrintList(listptr->next);
   }
}

int main()
{

    struct node *list = NULL;
    list = insert(10,list);
    list = insert(5,list);
    list = insert(7,list);
    list = insert(12,list);
    list = insert(1,list);
    // printf("%d\n",list->data);
    PrintList(list);
    list = insertRecursive(2,list);
    list = insertRecursive(20,list);
    PrintList(list);
    
}