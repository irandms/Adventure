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
void shuffle(char *array[], size_t length)
{
    //size_t i;
    //size_t length = sizeof(array) / sizeof(array[0]);

    for (size_t i = 0; i < length; i++) {
        size_t j = rand() % (i + 1);
        char *temp = array[j];
        // strcpy(temp, array[j]);
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
    
    chdir(pid_str); 

    char *rooms[] = {"Kitten","Gargoyle","Lizard","Facebook","Computer","Witchy","Vacuum","Wonder","Dungeon","Bouncy"}; 
    
    shuffle(rooms, 10);  

    for (size_t h = 0; h < 7; h++) {
        
        FILE *fPtr; 
        char roomType[20];
        char room_name[30];

        sprintf(room_name, "%s_room", rooms[h]);

        fPtr = fopen(room_name, "w"); //Create a file for the first seven rooms

        if (h == 0)
            strcpy(roomType, "START_ROOM");
        else if (h == 1)
            strcpy(roomType, "END_ROOM");
        else
            strcpy(roomType, "MID_ROOM");

        fputs(roomType, fPtr);
        fclose(fPtr);
    } 

    return 0;
}
