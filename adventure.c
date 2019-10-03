#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <unistd.h>

struct Room {
    char* name; 
    char* type;
    char* rooms[];
};

char * findCurrentDirectory() { 
    
    struct dirent * dir_entry;
    struct stat statbuf;

    DIR * dr = opendir(".");

    char * workingDir = "";
    time_t moddedTime;

    printf("working dir at the start is %s \n", workingDir);

    bool what = (workingDir == "");

    if (what == true)
        printf("workingdir is quotes \n");
    else
        printf("workingdir is not quotes \n");

    while ((dir_entry = readdir(dr)) != NULL) { //iterate through every file in the game directory 
    
        if (strstr(dir_entry->d_name, ".rooms") != NULL) { //if the file is a "rooms" directory

            stat(dir_entry->d_name, &statbuf);

            // If the current directory was modified after the current workingDir
            // Or if this is the first rooms dir encountered. 
            if ((difftime(statbuf.st_mtim.tv_sec, moddedTime) > 0) || (workingDir == "")) { 
                workingDir = dir_entry->d_name;
                printf("inside if, workingdir is %s \n", workingDir);

                moddedTime = statbuf.st_mtim.tv_sec;
            }
        }            
    }

    closedir(dr);

    //char* dirPointer = (char*) workingDir;
    printf("Returning %s \n", workingDir);
    return workingDir;
} 

void readRooms (char * dirName, struct Room * rooms) {
    
    struct dirent * dir_entry; 

    chdir(dirName);

    DIR *dr = opendir(".");
    FILE *fptr;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;


    while ((dir_entry = readdir(dr)) != NULL) { //iterate through every file in the game directory 
        
        printf("name of file is %s \n", dir_entry->d_name);

        //char * fileName = dir_entry->d_name;

        fptr = fopen(dir_entry->d_name, "r");

        int room_num = 0; 

        while ((read = getline(&line, &len, fptr)) != -1)  { //while the end of the file is not reached
            
            //printf("Retrieved line of length %zu :\n", read);

            printf("%s", line);

            //char * line_ptr = &line;

            char* token = strtok(line, ":");
            token = strtok(NULL, ": "); //Call strtok a second time to get the value on the other side of the colon

            printf("token is %s \n", token);

            if (strstr(line, "NAME") != NULL) {
                 //rooms[room_num].name = token;
            }

            else if (strstr(line, "CONNECTION") != NULL)  {
            
            }

            else if (strstr(line, "TYPE") != NULL) {
                rooms[room_num].type = token;
                printf("Room type set as %s \n", rooms[room_num].type); 
            }

            room_num++;
        }
    }
}

int main(){    
    
    struct Room rooms[7]; 
    char * workingDir = "";

    printf("Entering findcurrent \n");

    workingDir = findCurrentDirectory(); //Find the most recent directory created by buildrooms.c
   
    printf("Working dir now is %s \n", workingDir);

    readRooms(workingDir, rooms); //read all Room files into the rooms array

    return 0;
}
