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

    //Build the temporary node
    struct node *tmpptr = (struct node *) malloc(sizeof(Node));
    tmpptr->data = N;
    tmpptr->next = NULL;

    //If this is the end of the list or the next value is larger
   if (listptr == NULL || N < listptr->data)
   {
       //insert into list
      tmpptr->next = listptr;
      listptr = tmpptr;
   }
   else 
   {
       //'traversal' node created
      struct node *curptr = listptr;
      //Is the next node the end? Is it larger than N?
      while (curptr->next != NULL && curptr->next->data <= N)
      {
          //If so move the traversal pointer
          curptr = curptr->next;
      }
      tmpptr->next = curptr->next;
      curptr->next = tmpptr;
   }
   return listptr;
}

void do_insertRecursive(int N, Node *listptr, Node *tmpptr)
{
    if(listptr->next == NULL || listptr->next->data >= N){
    // if(listptr == NULL ||  N < listptr->data){
        tmpptr->next = listptr;
        listptr = tmpptr;
        return;
    } 
    else
    {
        listptr = listptr->next;
        do_insertRecursive(N, listptr, tmpptr);
        
    }
}

Node * insertRecursive(int N, Node *listptr)
{
    //Save the head node's location
    struct node *head = listptr;

    //Build the temporary node
    struct node *tmpptr = (struct node *) malloc(sizeof(Node));
    tmpptr->data = N;
    tmpptr->next = NULL;
    do_insertRecursive(N,listptr,tmpptr);
    return head;
} 


void PrintList(Node *listptr){
   if(listptr == NULL){
       printf("NULL\n");   
       return;
   } else {
       printf("%d", listptr->data);
        printf(" -> ");
       
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
    list = insertRecursive(6,list);

    list = insertRecursive(20,list);
    PrintList(list);
    
}