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


//Implementation of the Fisher-Yates shuffle algorithm, taken from https://benpfaff.org/writings/clc/shuffle.html
void shuffle(char *array[], size_t length)
{
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
}

void initTypes(char* types[], int num_of_rooms) { 

    int start_ind = rand() % (num_of_rooms-1);

    int end_ind = start_ind;

    while (end_ind == start_ind) {
        end_ind = rand() % (num_of_rooms-1);
    }

    for (int i = 0; i < 7; i++) {
        if (i == start_ind)
            types[i] = "START_ROOM";
        else if (i == end_ind)
            types[i] = "END_ROOM";
        else 
            types[i] = "MID_ROOM";
    }
}



void createFiles(bool connectionMap[7][7], char* types[], char *rooms[],  size_t num_of_rooms) 
{

    for (size_t h = 0; h < num_of_rooms; h++) {
        
        char roomType[20];
        char roomName[30];

        sprintf(roomName, "%s_room", rooms[h]);

        FILE *fPtr = fopen(roomName, "w"); //Create a file for the first seven rooms

        fprintf(fPtr, "ROOM NAME: %s \n", rooms[h]);

        int numOfConn = 1;

        for (int y = 0; y < 7; y++) {
            if (connectionMap[h][y] == true) {
                fprintf(fPtr, "CONNECTION %d: %s \n", numOfConn, rooms[y]);
                numOfConn++;
            }
        }       
        
        fprintf(fPtr, "ROOM TYPE: %s \n", types[h]); 

        fclose(fPtr);
    }
}


int main() 
{
    srand(time(NULL)); //Seed randomization

    const int room_num = 7;

    char pid_str[80];

    bool connectionMap[7][7] = {0}; //An array that stores all of the room connections  
    char *types[7];

    int pid = getpid();
    sprintf(pid_str, "harpekar.rooms.%d", pid);
    mkdir(pid_str, 0700); //Make room directory, with read, write, and execute power
    
    chdir(pid_str); 

    char *rooms[] = {"Kitten","Gargoyle","Lizard","Facebook","Computer","Witchy","Vacuum","Wonder","Liquor","Pride"}; 
    
    shuffle(rooms, 10); 
    
    initConnections(connectionMap, room_num);

    initTypes(types, room_num);

    createFiles(connectionMap, types, rooms, room_num);

    return 0;

}
