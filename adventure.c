#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

struct Room {
    char name[10]; 
    char type[15];
    char* connections[6];
};

char * findCurrentDirectory() { 
    
    struct dirent * dir_entry;
    struct stat statbuf;

    DIR * dr = opendir(".");

    char * workingDir = "";
    time_t moddedTime;

    while ((dir_entry = readdir(dr)) != NULL) { //iterate through every file in the game directory 
    
        if (strstr(dir_entry->d_name, ".rooms") != NULL) { //if the file is a "rooms" directory

            stat(dir_entry->d_name, &statbuf);

            // If the current directory was modified after the current workingDir
            // Or if this is the first rooms dir encountered. 
            if ((difftime(statbuf.st_mtim.tv_sec, moddedTime) > 0) || (workingDir == "")) { 
                workingDir = dir_entry->d_name;

                moddedTime = statbuf.st_mtim.tv_sec;
            }
        }            
    }

    closedir(dr);

    //char* dirPointer = (char*) workingDir;
    printf("Returning %s \n", workingDir);
    return workingDir;
} 

void parseLine(char * line, char* lineParts[]) {

     char* token = strtok(line, " :");
     //token = strtok(NULL, ": ");

     int linePart = 0;

     while (token != NULL) {

        lineParts[linePart] = token;
        //printf("token is %s \n", lineParts[linePart]);

        token = strtok(NULL, " :");
        linePart++;
     }

     printf("Last linepart is %s \n", lineParts[2]);
}

void readRooms (char * dirName, struct Room *rooms) {
    
    struct dirent * dir_entry; 

    chdir(dirName);

    DIR *dr = opendir(".");
    FILE *fptr;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    char* lineParts[4];

    int roomNum = 0; 

    while ((dir_entry = readdir(dr)) != NULL) { //iterate through every file in the game directory 
        
        printf("name of file is %s \n", dir_entry->d_name);

        //char * fileName = dir_entry->d_name;

        if (strstr(dir_entry->d_name, "room") == NULL) { //Skip any non-room file
            printf("Skipping bad files \n");
            continue;
        }

        fptr = fopen(dir_entry->d_name, "r");

        while ((read = getline(&line, &len, fptr)) != -1)  { //while the end of the file is not reached
            
            //printf("Retrieved line of length %zu :\n", read);

            printf("The room num is %d \n", roomNum);

            //char * line_ptr = &line;

            parseLine(line, lineParts);

            //printf("lineparts[1] is %s \n", lineParts[1]);
            
            if (strcmp(lineParts[0], "CONNECTION") == 0) {
                //printf("found a connection! \n ");
                int connectNum = atoi(lineParts[1]) - 1;
                rooms[roomNum].connections[connectNum] = lineParts[2];

                printf("Setting connection %d to %s \n", connectNum, rooms[roomNum].connections[connectNum]);
            }

            else if (strcmp(lineParts[1], "TYPE") == 0) {
                printf("Room type found \n");
                strcpy(rooms[roomNum].type, lineParts[2]);
                printf("setting room type as %s \n", rooms[roomNum].type);
            }
            
            else {
                printf("Room name found \n");
                //char* roomName = lineParts[2];

                printf("Room name found to be %s \n", lineParts[2]);

                strcpy(rooms[roomNum].name, lineParts[2]);
                printf("setting room name as %s \n", rooms[roomNum].name); 
            }

        }

        roomNum++;
    }

    printf("Within readRooms, second room name is %s \n", rooms[1].name);
}

void printConnections(struct Room rooms[], int roomPos) {
    
}

int findRoom(struct Room rooms[], char* roomToFind, char* charType) { //Takes in the room characteristic type and value and returns its position in the rooms struct

    printf("Finding a %s that matches %s \n", charType, roomToFind); 

    for (int i = 0; i < 7; i++) { 
        char* testRoom;
        printf("Currently on room %s \n", rooms[i].name);

        if (charType == "type") {
            testRoom = rooms[i].type;
            printf("Currently Testing %s \n", testRoom);
        }
        else if (charType == "name") {
            testRoom = rooms[i].name;
        }

        printf("Testing if %s matches %s \n", testRoom, roomToFind);

        if (testRoom == roomToFind) 
            printf("Found! \n");
            return i;       
    } 

    printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN. \n");
}

int goToRoom(struct Room rooms[], char* room, char* charType, int stepNum) {

    int roomPos = findRoom(rooms, room, charType);

    printf("CURRENT LOCATION: %s \n", rooms[roomPos].name);
    printf("POSSIBLE CONNECTIONS:");

    printConnections(rooms, roomPos);

    printf("WHERE TO? >");
}

void playGame(struct Room  rooms[]) {

    printf("a random room name is %s \n", rooms[1].name);

    int numSteps = goToRoom(rooms, "START_ROOM", "type", 0);
    
    printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS! \n YOU TOOK %d STEPS. \n", numSteps);    
}

int main(){    
    
    struct Room rooms[7]; 
    char * workingDir = "";

    //printf("Entering findcurrent \n");

    workingDir = findCurrentDirectory(); //Find the most recent directory created by buildrooms.c
   
    //printf("Working dir now is %s \n", workingDir);

    readRooms(workingDir, rooms); //read all Room files into the rooms array

    playGame(rooms);

    return 0;
}
