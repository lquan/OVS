#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/* MALLOC INSTRUMENTATION */
#ifdef __VERIFY
#define NB_ELEMENTS 8
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
    
    int result = head->data; // pull out the data before the node is deleted
    *headRef = head->next;
    my_free(head); // free the head node
        
    return result;
}

int is_empty(Node* head) {
    return head == NULL;
}


int main()
{
    Node* stack = NULL; //initialize stack
    
    int i;
    for (i=0; i < NB_ELEMENTS; ++i) {
        #ifndef __VERIFY 
        printf("pushed: %d\n", i);
        #endif
        push(&stack, i);
    }

    int popped;
    while( stack ) {
        popped = pop(&stack);
        assert( popped >= 0 && popped < NB_ELEMENTS);
        #ifndef __VERIFY 
        printf("popped: %d\n", popped);
        #endif
    }
    
    #ifdef __VERIFY
    assert(counter==0);
    #endif
    return 0;
}
