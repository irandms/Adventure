#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>
#include <pthread.h>

typedef struct Room Room;

struct Room {
    char name[10]; 
    char type[15];
    //char connections[6][10];
    Room * connections[6];
    int numConns;
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

            // Set the current working directory If the one being compared was modified after the current one
            // Or if this is the first rooms dir encountered. 
            if ((difftime(statbuf.st_mtim.tv_sec, moddedTime) > 0) || (workingDir == "")) { 

                workingDir = dir_entry->d_name;
                moddedTime = statbuf.st_mtim.tv_sec;
            }
        }            
    }

    closedir(dr);

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

     //printf("Last linepart is %s \n", lineParts[2]);
}

Room* findConnection(struct Room *room, char* roomToFind) { //Takes in the room name and returns the pointer to the room

    for (int i = 0; i < room->numConns; i++) { 
        Room* testRoom;
        printf("Currently on room %s \n", room->connections[i]->name);

        testRoom = room->connections[i];
        printf("Current room type is %s \n", testRoom->type);

        printf("Testing if %s matches %s \n", testRoom->name, roomToFind);

        if (strcmp(testRoom->name, roomToFind) == 0) { 
            printf("Found! \n");
            return testRoom;       
        }
    } 

    printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN. \n"); //Fallback if the room provided doesn't exist
    return NULL;
}

Room* findRoom(struct Room rooms[], char* roomName) { //Finds the room in the entire room array (used for initial file parsing)
    
    for (int i = 0; i < 7; i++) { 
        //Room* testRoom;
        printf("Currently on room %s \n", rooms[i].name);

        char* testRoom = rooms[i].name;

        if (strcmp(testRoom, roomName) == 0) { 
            printf("Found! \n");
            return &(rooms[i]);       
        }
    }

    printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN. \n"); //Fallback if the room provided doesn't exist
} 

Room* findStartRoom(struct Room rooms[]) {
    
    for (int i = 0; i < 7; i++) { 
        //Room* testRoom;
        printf("Currently on room %s \n", rooms[i].name);

        char* testRoom = rooms[i].type;
        printf("Current room type is %s \n", testRoom);

        //printf("Testing if %s matches %s \n", testRoom->name, roomToFin);

        if (strcmp(testRoom, "START_ROOM") == 0) { 
            printf("Found! \n");
            return &(rooms[i]);       
        }
    } 

    printf("HUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN. \n"); //Fallback if the room provided doesn't exist
}

void initRooms(struct Room rooms[], struct dirent *dir_entry, DIR* dr) {

    int roomInd = 0;

    while ((dir_entry = readdir(dr)) != NULL) { //iterate through every file in the game directory 
        printf("name of file in initrooms is %s \n", dir_entry->d_name);

        char * fileName = dir_entry->d_name;

        if (strstr(fileName, "room") != NULL) { //If the file is a room file
            char* roomName = strtok(fileName, "_");
            printf("Found Room name is %s \n", roomName);

            strcpy(rooms[roomInd].name, roomName);

            roomInd++;
        }
    }
}

void readRooms (struct Room rooms[]) {
    
    //For file iteration in directory
    DIR *dr = opendir(".");
    FILE *fptr;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    struct dirent *dir_entry;

    //For parsing of Connection values
    char *endptr;

    char* lineParts[4];

    int roomNum = 0;

    printf("Starting to flesh out rooms array \n");
        
    while ((dir_entry = readdir(dr)) != NULL) { //iterate through every file in the game directory 
        printf("name of file is %s \n", dir_entry->d_name);

        //char * fileName = dir_entry->d_name;

        if (strstr(dir_entry->d_name, "room") == NULL) { //Skip any non-room file
            //printf("Skipping bad files \n");
            continue;
        }

        fptr = fopen(dir_entry->d_name, "r");

        while ((read = getline(&line, &len, fptr)) != -1)  { //while the end of the file is not reached
            
            printf("The room num is %d \n", roomNum);

            //char * line_ptr = &line;

            parseLine(line, lineParts);

            printf("lineparts[1] is %s \n", lineParts[1]);
            
            if (strcmp(lineParts[1], "TYPE") == 0) {
                printf("The room type in the file is %s \n", lineParts[2]);
                strcpy(rooms[roomNum].type, lineParts[2]);
                printf("setting room type as %s \n", rooms[roomNum].type);
            }
            
            else if (strcmp(lineParts[0], "CONNECTION") == 0) {
                int numOfConns = atoi(lineParts[1]); 
                    
                rooms[roomNum].numConns = numOfConns; //Number of connections is the last Connection number in the file: will be in the last parsed connection
                 
                int connectNum = numOfConns - 1; //For use in array indexing

                //Find index of parsed room name, then pass pointer to that room object to the Connections array
                rooms[roomNum].connections[connectNum] = (findRoom(rooms, lineParts[2])); 

                printf("Setting connection %d to %s \n", connectNum, rooms[roomNum].connections[connectNum]->name);

            }

        }

        printf("Within readRooms, a random room conn is %s \n", rooms[1].connections[1]->name);
        roomNum++;
    }
}

void printConnections(struct Room *room) {

    int roomConnNum = room->numConns;

    for(int j = 0; j < roomConnNum; j++) {
        
        if (j == (roomConnNum - 1)) { //If it's the last connection, format correctly. Otherwise, just continue the list
            printf("%s. \n", room->connections[j]->name);
            break;
        }
        else 
            printf("%s, ", room->connections[j]->name);
    }    
}


int goToRoom(Room* room, int stepNum) {

    printf("Room type is %s \n", room->type);

    if (strcmp(room->type, "END_ROOM") == 0) { //If the room is the end room, the game is over
        printf("End room found! \n");
        return stepNum;
    }

    char destination[25]; //Allow for a long mistyped room

    Room* destRoom = NULL;
    while (destRoom == NULL) { //Loop on user input until they give a valid connection name
        printf("CURRENT LOCATION: %s \n", room->name);
        printf("POSSIBLE CONNECTIONS: ");

        printConnections(room);

        printf("WHERE TO? >");

        scanf("%s", destination);

        printf("Want to go to %s \n", destination);
        destRoom = findConnection(room, destination);
    }

    stepNum++;

    int itrStepNum = goToRoom(destRoom, stepNum);
}

void playGame(struct Room rooms[]) {

    printf("a random room name is %s \n", rooms[3].name);
    
    Room* startRoom = findStartRoom(rooms); 

    int numSteps = goToRoom(startRoom, 0); //Start at 0 steps
    
    printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS! \n YOU TOOK %d STEPS. \n", numSteps);    
}

int main(){ 
    struct Room rooms[7];
   
    char * workingDir = "";

    struct dirent * dir_entry; 

    //For file iteration in directory
    FILE *fptr;
    char *line = NULL;

    workingDir = findCurrentDirectory(); //Find the most recent directory created by buildrooms.c
   
    chdir(workingDir);
    DIR *dr = opendir(".");

    printf("Working dir now is %s \n", workingDir);

    initRooms(rooms, dir_entry, dr); //Initialize room array with room names

    readRooms(rooms); //read all Room types and connections into the rooms array

    playGame(rooms);

    closedir(dr);

    return 0;
}
