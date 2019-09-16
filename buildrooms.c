// Buildrooms.c 
// Creates a file with descriptions of rooms, to be used by adventure.c

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

//Implementation of the Fisher-Yates shuffle algorithm, taken from https://benpfaff.org/writings/clc/shuffle.html
void shuffle(char *array, size_t n)
{
    size_t i;
    size_t length = sizeof(array) / sizeof(array[0]);

    for (i = 0; i < length - 1; i++) {
        size_t j = i + rand() / (RAND_MAX / (length - i) + 1);
        char temp = array[j];
        array[j] = array[i];
        array[i] = temp;
    }
}


int main() 
{
    srand(time(NULL));
    char pid_str[80];

    int pid = getpid();
    sprintf(pid_str, "rooms.%d", pid);
    mkdir(pid_str, 0700); //Make room directory, with read, write, and execute power

    char *rooms[10] = {"Kitten","Gargoyle","Lizard","Facebook","Computer","Witchy","Vacuum","Wonder","Liquor","Pride"}; 
     
    //for(int loop = 0; loop < 10; loop++)
    //  printf("%s ", rooms[loop]);
    
    shuffle(*rooms, 10);  

    //for(int loop = 0; loop < 10; loop++)
    //  printf("%s ", rooms[loop]);

    return 0;
}
