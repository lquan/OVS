#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define NB_ELEMENTS 8

#ifdef __VERIFY
int counter = 0;
_Bool nondet_bool ();
void *my_malloc (size_t s)
 {
  if (nondet_bool()) {
	return (NULL);
  }
  counter++;
  return (malloc (s)); 
 }
 
 void my_free(void* arg)
 {
    free(arg);
    counter--;
 }
#else
void *my_malloc (size_t s)
 {
    return (malloc (s));
 }
 
 void my_free(void* arg)
 {
    free(arg);
 }
#endif


struct list_element {
 struct list_element *prev;
 struct list_element *next;
 int data;
};

struct list {
 struct list_element *first;
 struct list_element *last; 
};

/* --- List Implementation ------------------------------------------------ */

struct list_element *list_new_elem (int newData)
 {
  struct list_element *elem;

  elem = my_malloc (sizeof (struct list_element));
  if (!elem) { return (NULL); }
  
  assert(elem);
  elem->prev = elem;
  elem->next = elem;
  elem->data = newData;

  return (elem);
 }

struct list *list_new (void)
 {
  struct list *list = my_malloc (sizeof (struct list));
  if (!list) { return (NULL); }
  
  assert(list);
  list->first = NULL;
  list->last  = NULL;
  return (list);
 }

void list_append (struct list *list, int data)
 {
  if (!list) { return; }
  
  struct list_element *elem = list_new_elem(data);
  if (!elem) { return; }
  
  assert(list);
  if ( list->first == NULL && list->last == NULL)
   {
    list->first = elem;
    list->last  = elem;
    elem->prev  = elem;
    elem->next  = elem;
   }
   else
   {
    assert(list->last);
    elem->prev = list->last;
    assert(list->first);
    elem->next = list->first;
    
    //list->last->next = elem;
    //list->first->prev = elem;
    
    struct list_element *list_last_element = list->last;
    list_last_element->next = elem;
    
    struct list_element *list_first_element = list->first;
    list_first_element->prev = elem;
   
    list->last = elem;
   }

  return;
 }
void list_remove (struct list *list, struct list_element *elem)
 {
  if (!elem || !list) { return; }

  if (elem == list->first && elem == list->last)
   {
    list->first = NULL;
    list->last  = NULL;
   }
   else if (elem == list->first)
   {
    list->first = elem->next;
    
    //list->first->prev = list->last;
    struct list_element *list_first_element = list->first;
    list_first_element->prev = list->last;
    
   }
   else if (elem == list->last)
   {
    list->last = elem->prev;
    
    //list->last->next = list->first;
    struct list_element *list_last_element = list->last;
    list_last_element->next = list->first;
   }
   else
   {
    //elem->prev->next = elem->next;
    struct list_element *elem_prev_element = elem->prev;
    elem_prev_element->next = elem->next;
    
    //elem->next->prev = elem->prev;
    struct list_element *elem_next_element = elem->next;
    elem_next_element->prev = elem->prev;
   }

  my_free (elem);
  return;
 }


void list_release (struct list *list)
 {
  struct list_element *elem;

  if (!list) { return; }

  elem = list->first;
  while (elem)
   {
    list_remove (list, elem);
    elem = list->first;
   }

  my_free (list);
  return;
 }

 
int main()
{
    struct list* my_list = list_new();
    int i;
    for (i=0; i < NB_ELEMENTS; ++i) {
        list_append (my_list, i);
    }

    list_release(my_list);
    #ifdef __VERIFY
	assert(counter == 0);
    #endif
}

