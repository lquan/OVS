
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* ------------------------------------------------------------------------ */

#ifdef __VERIFY
_Bool nondet_bool ();

void *my_malloc (size_t s)
 {
  if (nondet_bool ()) { return (NULL); };
  return (malloc (s)); 
 }
#else
void *my_malloc (size_t s)
 {
  return (malloc (s));
 }
#endif

/* ------------------------------------------------------------------------ */
void print_str (char *str)
 {
  int i;

  for (i = 0; str[i] != '\0'; i++)
   { printf ("%c", str[i]); }

  return;
 }

/* ------------------------------------------------------------------------ */
void change_str (char *str)
 {
  int i;

  for (i = 0; str[i] != '\0'; i++)
   {
//#ifndef __VERIFY
    str[i] += 1;
//#endif 
   }

  return;
 }

/* ------------------------------------------------------------------------ */
char *my_strdup (char *str)
 {
  int s, i;
  char *str2 = NULL;

  s = strlen (str);
  if (s > 0) {
    str2 = malloc ((sizeof (int) * s) + 1);
//#ifndef __VERIFY
    for (i = 0; i < s; i++) { str2[i] = str[i]; }
    str2[s] = '\0';
//#endif 
   }

  return (str2);
 }

/* ------------------------------------------------------------------------ */

struct point {
  int x;
  int y;
  int z;
  char *description;
 };

#define POINTS 5


int main (int argc, char **argv)
 {
  struct point *point[POINTS];
  int i;
  char str[] = "hello world";



  for (i = 0; i < POINTS; i++)
   {
    point[i] = my_malloc (sizeof (struct point));
  
    point[i]->x = 5;
    point[i]->y = 6;
    point[i]->z = 7;
    point[i]->description = strdup("a point.\n");
//    point[i]->description = my_strdup("a point.\n");
//    point[i]->description = my_strdup(str);
   }

  

  for (i = 0; i < POINTS; i++)
   {
//    free (point[i]->description);
    free (point[i]);
   }


//  change_str (point[0]->description);
  printf ("%i\n", point[0]->x);
  printf ("%s", point[0]->description);
//  print_str (point[0]->description);
//  change_str (point[0]->description);


  return (0);
 }

