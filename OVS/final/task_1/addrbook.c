#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>

#include <assert.h>


/* ------------------------------------------------------------------------ */
#ifdef __VERIFY
_Bool nondet_bool ();
void *my_malloc (size_t s)
 {
  if (nondet_bool()) { return (NULL); }
  return (malloc (s)); 
 }
 
#else
void *my_malloc (size_t s)
 {
  return (malloc (s));
 }
#endif
/* ------------------------------------------------------------------------ */


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
#ifdef __VERIFY
#define INPUT_BUF_SIZE 8
#else
#define INPUT_BUF_SIZE 1024
#endif
char *input (int n)
 {
  static char buf[INPUT_BUF_SIZE];
  char c;
  int  i;

  i = 0;
  while (c != '\n' && i <= n && i < INPUT_BUF_SIZE)
   { c = getchar();
     printf ("%c", c);
     if (c != '\n' ) { buf[i] = c; i++; } }

  buf[i] = '\0';

  return (buf);
 }


/* --- List Implementation ------------------------------------------------ */

struct list_element *list_new_elem (struct address *addr)
 {
  assert(addr);
  
  struct list_element *elem = my_malloc (sizeof (struct list_element));
  if (!elem) { return (NULL); }

  assert(elem);
  elem->prev = elem;
  elem->next = elem;
  elem->addr = addr;

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

void list_append (struct list *list, struct address *addr)
 {
  assert(list);
  assert(addr);
  struct list_element *elem = list_new_elem(addr);
  
  if(!elem) { return; } 
  
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
    list->last->next = elem;
    list->first->prev = elem;
    list->last = elem;
   }

  return;
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
  
  free (elem->addr);
  free (elem);

  return;
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
  return;
 }


/* --- Address Book Functionality ----------------------------------------- */

#define check_abort          { printf ("abort (y/n)? "); \
                               c = 0; while (c != 'y' && c != 'n') \
                                { c = getchar(); printf ("%c", c); } \
                               if (c == 'y') { goto ABORT_INPUT; } }
#define get_string(tag, dst) { printf (tag); \
                              tmp = input (INPUT_BUF_SIZE); \
                              if (strlen(tmp) == 0) { check_abort } \
                              dst = my_malloc(INPUT_BUF_SIZE * (sizeof (char))); \
                              if (!dst) { goto ABORT_INPUT; } \
                              strcpy(dst, tmp); }


struct address *input_addr ()
 {
  char *tmp, *name = NULL, *addr = NULL, *phone = NULL;
  char c;

  get_string ("name: ", name);
  get_string ("address: ", addr);
  get_string ("phone: ", phone);

  struct address *address = my_malloc (sizeof(struct address));
  if(!address) { goto ABORT_INPUT; }
  
  assert(address);
  assert(name);
  assert(addr);
  assert(phone);
    
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
  assert(addr);
  printf (addr->name); printf("; ");
  printf ("%s; %s\n", addr->address, addr->phone);

  return;
 }

void list_addrs (struct list *list)
 {
  assert(list);
  struct list_element *elem = list->first;
  unsigned int i = 0;
  while (elem)
  {
     printf ("%04u:\n  ", i);
     print_addr (elem->addr);

     if (elem == list->last)
      { elem = NULL; }
      else
      { elem = elem->next; }
     i++; 
   }
 }

void del_addr (struct list *list)
 {
  assert(list);
  struct list_element *elem = list->first;
  unsigned int i, n;
  char *tmp;

  printf ("delete #: ");
  tmp = input (INPUT_BUF_SIZE);
  n = atoi (tmp);

  i = 0;
  while (elem && i != n)
  {
    if (elem == list->last)
    {
      elem = NULL; 
    }
    else
    {
      elem = elem->next; 
    }
    i++; 
    
  }

  list_remove (list, elem);

  return;
 }

int cmp_addr (struct address *addr, char *str)
 {
  assert(addr);
  assert(str);
  if (strcasestr (addr->name, str))    { return (1); }
  if (strcasestr (addr->address, str)) { return (1); }
  if (strcasestr (addr->phone, str))   { return (1); }

  return (0);
 }

void search_addr (struct list *list)
 {
  assert(list);
  struct list_element *elem = list->first;
  unsigned int i = 0;
  char *str;

  printf ("search: ");
  str = input (INPUT_BUF_SIZE);

  while (elem)
   {
     if (cmp_addr (elem->addr, str))
      { printf ("%04u:\n  ", i);
        print_addr (elem->addr); }

     if (elem == list->last)
      { elem = NULL; }
      else
      { elem = elem->next; }
     i++; 
   }

  return;
 }


/* --- main() ------------------------------------------------------------- */

int main (int argc, char **argv)
 {
  /* list initialisation */
  struct list *list = list_new();
  if (!list) { abort(); }
  
  /* terminal setup */
  struct termios oldT, newT;
  ioctl(0,TCGETS,&oldT);
  newT=oldT;
  newT.c_lflag &= ~ECHO;
  newT.c_lflag &= ~ICANON;
  ioctl(0,TCSETS,&newT);

  char c = 0;
  /* main loop */
  while (c != 'q')
   {
    printf ("a: add; l: list; d: delete; s: search; q: quit\n");
    c = getchar();
    if (c != '\n') { printf ("%c\n", c); }
    switch (c) {
     case 'a': { struct address *addr = input_addr();
                 if (addr) { list_append (list, addr); }
                 break; }
     case 'l': { list_addrs (list);
                 break; }
     case 'd': { del_addr (list);
                 break; }
     case 's': { search_addr (list);
                 break; }
     default : { break; } 
    }
   }

  /* cleanup */
  list_release (list);

  /* restore terminal settings */
  ioctl(0,TCSETS,&oldT);

  return (0);
 }

