#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <assert.h>


/* --- Data Structures ---------------------------------------------------- */

struct address {
 char *name;
 char *address;
 char *phone; };

struct list_element {
 struct list_element *prev;
 struct list_element *next;
 struct address *addr; };

struct list {
 struct list_element *first;
 struct list_element *last; };


/* --- Helper Functions --------------------------------------------------- */

#define INPUT_BUF_SIZE 1024

char *input (int n)
 {
  static char buf[INPUT_BUF_SIZE];
  char c;
  int i = 0;
  while ( i <= n && i < (INPUT_BUF_SIZE - 1) ) //fixed buffer overflow vulnerability
   { 
     c = getchar();
     printf ("%c", c);
     if (c != '\n' ) { buf[i] = c; i++; } 
     else { break; } // we can immediately break out of the loop
   }

  buf[i] = '\0';

  return (buf);
 }


/* --- List Implementation ------------------------------------------------ */

struct list_element *list_new_elem (struct address *addr)
 {
  assert(addr);  
  struct list_element *elem = malloc (sizeof (struct list_element));
  if (!elem) { return (NULL); }

  elem->prev = elem;
  elem->next = elem;
  elem->addr = addr;

  return (elem);
 }

struct list *list_new (void)
 {
  struct list *list = malloc (sizeof (struct list));
  if (!list) { return (NULL); }
  
  list->first = NULL;
  list->last  = NULL;

  return (list);
 }

int list_append (struct list *list, struct address *addr)
 {
  assert(list);
  assert(addr);

  struct list_element *elem = list_new_elem(addr);
  if (!elem) { return 0; } //failed to add address element

  if (list->first == NULL && list->last == NULL)
   {
    list->first = elem;
    list->last  = elem;

    elem->prev  = elem;
    elem->next  = elem;
   }
   else
   {
    elem->prev = list->last;
    elem->next = list->first;
    //list->last->next = elem;
    //list->first->prev = elem;
    
    struct list_element *list_last_element = list->last;
    list_last_element->next = elem;
    
    struct list_element *list_first_element = list->first;
    list_first_element->prev = elem;
    
    list->last = elem;
   }

  return 1; //succesfully added address element
 }

void list_remove (struct list *list, struct list_element *elem)
 {
  assert(list);

  if (!elem) { return; }
  
  if (elem == list->first && elem == list->last)
   {
    list->first = NULL;
    list->last  = NULL;
   }
   else if (elem == list->first)
   {
    list->first = elem->next;
    list->first->prev = list->last;
   }
   else if (elem == list->last)
   {
    list->last = elem->prev;
    list->last->next = list->first;
   }
   else
   {
    elem->prev->next = elem->next;
    elem->next->prev = elem->prev;
   }
  
  
  struct address *p = elem->addr;
  assert(p);
  assert(p->name);
  assert(p->address);
  assert(p->phone);
  
  free (p->name);
  free (p->address);
  free (p->phone);
  free (p);
  free (elem);
 }

void list_release (struct list *list)
 {
  assert(list);

  struct list_element *elem = list->first;
  while (elem)
   {
    list_remove (list, elem);
    elem = list->first;
   }

  free (list);
 }


/* --- Address Book Functionality ----------------------------------------- */

#define check_abort          { printf ("abort (y/n)? "); \
                               c = 0; while (c != 'y' && c != 'n') \
                                { c = getchar(); printf ("%c", c); } \
                               if (c == 'y') { goto ABORT_INPUT; } }
#define get_string(tag, dst) { printf (tag); \
                              tmp = input (INPUT_BUF_SIZE); \
                              if (strlen(tmp) == 0) { check_abort } \
                              dst = malloc(INPUT_BUF_SIZE * (sizeof (char))); \
                              if (!dst) { goto ABORT_INPUT; } \
                              strcpy(dst, tmp); }


struct address *input_addr ()
 {
  char *tmp, *name = NULL, *addr = NULL, *phone = NULL;
  char c;

  get_string ("name: ", name);
  get_string ("address: ", addr);
  get_string ("phone: ", phone);

  struct address *address = malloc (sizeof(struct address));
  if(!address) { goto ABORT_INPUT; }
  
  address->name    = name;
  address->address = addr;
  address->phone   = phone;
  return (address);

ABORT_INPUT:
  if (name)  { free (name); }
  if (addr)  { free (addr); }
  if (phone) { free (phone); }
  return (NULL);
 }

void print_addr (struct address *addr)
 {
  assert(addr); //verify null pointer dereference
  
  printf (addr->name); printf("; ");
  printf ("%s; %s\n", addr->address, addr->phone);
 }

void list_addrs (struct list *list)
 {
  assert(list); //verify null pointer dereference
  struct list_element *elem = list->first;
  unsigned int i = 0;
  while (elem)
  {
     printf ("%04u:\n  ", i);
     print_addr (elem->addr);

     if (elem == list->last)
      { 
        elem = NULL; 
      }
      else
      { elem = elem->next; }
     i++; 
   }
 }

void del_addr (struct list *list)
 {
  assert(list); //verify null pointer dereference
  struct list_element *elem = list->first;
  unsigned int i, n;
  char *tmp;

  printf ("delete #: ");
  tmp = input (INPUT_BUF_SIZE);
  n = atoi (tmp); //UB possible
  
  i = 0;
  while (elem && i != n)
  {
    if (elem == list->last)
     { 
       elem = NULL;
     }
     else
     { elem = elem->next; }
    i++; 
  }

  list_remove (list, elem);
 }

int cmp_addr (struct address *addr, char *str)
 {
  assert(addr);
  //if (!addr) { return 0; } //fixed null pointer dereference
  if (strcasestr (addr->name, str))    { return (1); }
  if (strcasestr (addr->address, str)) { return (1); }
  if (strcasestr (addr->phone, str))   { return (1); }

  return (0);
 }

void search_addr (struct list *list)
 {
  assert(list);
  //if (!list) { return; } //fixed null pointer dereference
  struct list_element *elem = list->first;
  
  unsigned int i;
  char *str;

  printf ("search: ");
  str = input (INPUT_BUF_SIZE);

  i = 0;
  while (elem)
   {
     if (cmp_addr (elem->addr, str))
      { printf ("%04u:\n  ", i);
        print_addr (elem->addr); }

     if (elem == list->last)
      {
       elem = NULL;        
       }
      else
      { elem = elem->next; }
     i++; 
   }
 }


/* --- main() ------------------------------------------------------------- */

int main (int argc, char **argv)
 {
  /* list initialisation */
  struct list *list = list_new ();
  if (!list) { abort(); }
  
  /* terminal setup */
  struct termios oldT, newT;
  ioctl(0,TCGETS,&oldT);
  newT=oldT;
  newT.c_lflag &= ~ECHO;
  newT.c_lflag &= ~ICANON;
  ioctl(0,TCSETS,&newT);

  struct address *address1 = malloc (sizeof(struct address));
  struct address *address2 = malloc (sizeof(struct address));
  address1->name    = "%08x %08x %08x %08x %08x";
  address1->address = "test address 1";
  address1->phone   = "11111111111";
  
  address2->name    = "%n %n %n %n";
  address2->address = "test address 2";
  address2->phone   = "0123456789";
  
  list_append(list, address1);
  list_append(list, address2);
  list_addrs (list);

  //memory freeing stuff and so on...
  return (0);
 }

