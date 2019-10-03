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

bool connectionMap[7][7] = {0}; //An array that stores all of the room connections  

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

void initConnections(int num_rows) {

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

char* determineType(size_t room_place) {
        char* room_type;

        if (room_place == 0)
            room_type = "START_ROOM";
        else if (room_place == 1)
            room_type = "END_ROOM";
        else
            room_type = "MID_ROOM";

        return room_type;
}

void createFiles(char *rooms[],  size_t num_of_rooms) 
{
    for (size_t h = 0; h < 7; h++) {
        
        char roomType[20];
        char roomName[30];

        sprintf(roomName, "%s_room", rooms[h]);

        FILE *fPtr = fopen(roomName, "w"); //Create a file for the first seven rooms

        fprintf(fPtr, "ROOM NAME: %s \n", rooms[h]);

        int num_of_conn = 1;

        for (int y = 0; y < 7; y++) {
            if (connectionMap[h][y] == true) {
                fprintf(fPtr, "CONNECTION %d: %s \n", num_of_conn, rooms[y]);
                num_of_conn++;
            }
        }       
        
        fprintf(fPtr, "ROOM TYPE: %s \n", determineType(h)); 

        fclose(fPtr);
    }
}


int main() 
{
    srand(time(NULL)); //Seed randomization

    const int room_num = 7;

    char pid_str[80];

    int pid = getpid();
    sprintf(pid_str, "harpekar.rooms.%d", pid);
    mkdir(pid_str, 0700); //Make room directory, with read, write, and execute power
    
    chdir(pid_str); 

    char *rooms[] = {"Kitten","Gargoyle","Lizard","Facebook","Computer","Witchy","Vacuum","Wonder","Liquor","Pride"}; 
    
    shuffle(rooms, 10); 
    
    initConnections(room_num);

    createFiles(rooms, room_num);

    return 0;

}
