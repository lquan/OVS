#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* MALLOC INSTRUMENTATION */
#ifdef __VERIFY
#define NB_ELEMENTS 12
int counter = 0;
_Bool nondet_bool ();
void *my_malloc (size_t s)
 {
  if (nondet_bool()) { return (NULL); }
  
  counter++;
  return (malloc (s)); 
 }
 
 void my_free(void* arg)
 {
    free(arg);
    counter--;
 }
#else
#define NB_ELEMENTS 1024
void *my_malloc (size_t s)
 {
  return (malloc (s));
 }
  
void my_free(void* arg)
 {
    free(arg);
 }
#endif

// forward declaration of struct and typedef
typedef struct node Node;

// Linked list data structure
struct node {
    int data;
    Node* next;
};

// Given a reference (pointer to pointer) to the head
// of a list and an int, push a new node on the front of the list.
// Creates a new node with the int, links the list off the .next of the
// new node, and finally changes the head to point to the new node.
void push(Node** headRef, int newData) {
    // allocate node
    Node* newNode = (Node*) my_malloc(sizeof(Node));
    if (!newNode) return;
            
    // put in the data
    newNode->data = newData;
    // link the old list off the new node
    newNode->next = (*headRef);

    // move the head to point to the new node
    (*headRef) = newNode;
}

int pop(Node** headRef) {
    Node* head = *headRef;
    
    if(!head) {
        fputs("Error: pop from empty stack\n", stderr);
        abort();
    }
    
    //assert(head != NULL);
    int result = head->data; // pull out the data before the node is deleted
    *headRef = head->next;
    // unlink the head node for the caller
    // Note the * -- uses a reference-pointer
    // just like push() and DeleteList().
    my_free(head); // free the head node
        
    return(result); // don't forget to return the data from the link
}

int is_empty(Node* head) {
    return head == NULL;
}

int main()
{
    Node* stack = NULL;
     
    int i;
    for (i=0; i < NB_ELEMENTS; ++i) {
        //printf("pushed: %d\n", i);
        push(&stack, i);
    }

    int popped;
    while( stack ) {
        popped = pop(&stack);
        //printf("popped: %d\n", popped);
    }
    
    #ifdef __VERIFY
    assert(counter==0);
    #endif
    return(0);
}
