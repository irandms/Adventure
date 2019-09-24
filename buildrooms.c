// Buildrooms.c 
// Creates a file with descriptions of rooms, to be used by adventure.c

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

enum RoomType {START_ROOM, END_ROOM, MID_ROOM};

struct Room {
    int position; 
    char name[9];
    enum RoomType roomType;
    int connections[6];
};

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

void initConnections(bool connectionMap[7][7], int num_rows) {

    for (int s = 0; s < num_rows; s++) {
        for (int o = 0; o < 3; o++) {
            int randPlace = rand() % (num_rows);
            while (randPlace == s) 
                //If attempting to draw a connection between a node and itself or a connection that already exists, reroll
                randPlace = rand() % (num_rows);
            
            connectionMap[s][randPlace] = connectionMap[randPlace][s] = true;
        }
    }
    for (int p = 0; p < num_rows; p++) { 
        for (int h = 0; h < num_rows; h++) {
            printf("%d ", connectionMap[p][h]);
        } 
        
        printf("\n");
    }
}

void createFiles(char *rooms, size_t num_of_rooms) 
{
    for (size_t h = 0; h < 7; h++) {
        
        char roomType[20];
        char roomName[30];

        sprintf(roomName, "%s_room", rooms[h]);

        FILE *fPtr = fopen(roomName, "w"); //Create a file for the first seven rooms

        fprintf(fPtr, "ROOM NAME: %s \n", roomName);

        fclose(fPtr);
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

    char *rooms[] = {"Kitten","Gargoyle","Lizard","Facebook","Computer","Witchy","Vacuum","Wonder","Liquor","Pride"}; 
    
    bool connectionMap[7][7] = {0}; //An array that stores all of the room connections 
    
    shuffle(rooms, 10);  

    initConnections(connectionMap, 7);

    return 0;

}
/*
        if (h == 0)
            strcpy(roomType, "START_ROOM");
        else if (h == 1)
            strcpy(roomType, "END_ROOM");
        else
            strcpy(roomType, "MID_ROOM");

        fprintf(fPtr, "ROOM TYPE: %s", roomType);
        fclose(fPtr);
    } 

    return 0; */
