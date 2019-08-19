// Buildrooms.c 
// Creates a file with descriptions of rooms, to be used by adventure.c

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() 
{
    char pid_str[80];

    int pid = getpid();
    sprintf(pid_str, "rooms.%d", pid);
    mkdir(pid_str, 0700); 
    
    return 0;
}
