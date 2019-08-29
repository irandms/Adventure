// Buildrooms.c 
// Creates a file with descriptions of rooms, to be used by adventure.c

#include <stdio.h>
#include <stdlib.h>
#include <dict.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

int main() 
{
    srand(time(NULL))
    char pid_str[80];

    int pid = getpid();
    sprintf(pid_str, "rooms.%d", pid);
    mkdir(pid_str, 0700); //Make room directory, with read, write, and execute power

    char *rooms[7] = {"Kitten","Gargoyle","Lizard","Facebook","Computer","Witchy","Vacuum","Wonder","Liquor","Pride"}; 
    
    fprintf(sizeof(rooms));
    rooms = {}
    for (int i = 0;i<sizeof(rooms);i++) {
        
    }  

    return 0;
}
