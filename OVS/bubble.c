
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#define SIZE 10

void bubblesort (int *array, int length)
 {
  int i, j;

  for (i = 0; i < length - 1; ++i)
   { for (j = 0; j < length - i - 1; ++j)
      { if (array[j] > array[j + 1])
         { int tmp = array[j];

           array[j] = array[j + 1];
           array[j + 1] = tmp; }
      } }
 }




int sorted(int *array, int length)
{
    int i;
    for (i=0; i<length-1; ++i) {
        if(array[i] > array[i+1]) {
            return 0;
        }
    }
    
    return(1);
}



int main(int argc, char **argv)
{
    srand(time(NULL));
    
    int numbers[SIZE];
    
    int i;
    for (i = 0; i < SIZE; ++i)
    {
        numbers[i] = rand() % (SIZE + 1); 
    }
    
    for (i = 0; i < SIZE; ++i)
    {
        printf("%d: %d\n", i, numbers[i]);
    }
    
    bubblesort (numbers, SIZE);
    
    assert( sorted(numbers, SIZE));
    
    return(0);
    

}

