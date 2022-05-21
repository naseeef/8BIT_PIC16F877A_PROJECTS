// C program for generating a
// random number in a given range.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
  
// Generates and prints 'count' random
// numbers in range [lower, upper].
int printRandoms(int lower, int upper, 
                             int count)
{
    int i;
    for (i = 0; i < count; i++) {
        int num = (rand() % (upper - lower + 1)) + lower;
        return num;
    }
}
  
// Driver code
int random_number()
{
    int lower = 0, upper = 1, count = 1, rn;
  
    // Use current time as 
    // seed for random generator
    srand(time(0));
  
    rn = printRandoms(lower, upper, count);
  
    return rn;
}