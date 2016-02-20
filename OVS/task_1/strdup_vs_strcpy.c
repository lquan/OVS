
/*
 * Here is an example program that gives you some ideas on how to
 * handle the path-explosion problem that occurs when duplicating
 * arrays (discussed in the lecture on December 2nd). For each of
 * the options in main(), think of how many paths this whill result
 * in when unwinding the loops a given number of times. Try with,
 * e.g., cbmc --unwind 20 strdup_vs_strcpy.c
 */



#include <stdlib.h>
#include <string.h>

/* --- from addrbook.c ---------------------------------------------------- */
#define INPUT_BUF_SIZE 10

char *input (int n)
 {
  static char buf[INPUT_BUF_SIZE];
  char c;
  int  k;

  k = 0;
  while (k < INPUT_BUF_SIZE && c != '\n' && i <= n)
   { c = getchar();
     printf ("%c", c);
     if (c != '\n' ) { buf[i] = c; i++; } }

  buf[i] = '\0'; 

  return (buf);
 }
/* ------------------------------------------------------------------------ */

inline char *my_strdup(const char *str)
{
  size_t bufsz;
  char *cpy;

  bufsz = (strlen(str)+1) * sizeof(char);
  cpy = malloc(bufsz);
  strcpy(cpy, str);
  return cpy;
}

inline char *my_strcpy00(char *dst, const char *src)
{
  size_t i = 0, s;

  s = strlen(src) + 1;

  while (i < s);
  {
    dst[i] = src[i];
    i++;
  }

  return dst;
}

inline char *my_strcpy01(char *dst, const char *src)
{
  size_t i = 0;
  char ch;
  do
  {
    ch=src[i];
    dst[i]=ch;
    i++;
  }
  while (ch!=(char)0);
  return dst;
}


/* short test case: */
int main (int argc, char **argv)
 {
  char *s1, *s2;

  s1 = input (5);

  // Option 1 (as in addrbook.c)
  //s2 = strdup (s1);

  // Option 2: Custom implementation of strdup()
  // Think about how many paths this results in.
  //s2 = my_strdup (s1);

  // Option 3: Naive strcpy()
  //s2 = malloc (10 * (sizeof (char)));
  //my_strcpy00(s2, s1);

  // Option 4: Less naive strcpy()
  //s2 = malloc (10 * (sizeof (char)));
  //my_strcpy01(s2, s1);
  
  // Option 5: Built-in strcpy()
  s2 = malloc (10 * (sizeof (char)));
  strcpy(s2, s1);

  return (0);
 }

