#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

struct Room {
    char* name; 
    int connections[6];
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
        printf("workingdir is quotes");
    else
        printf("workingdir is not quotes");

    while ((dir_entry = readdir(dr)) != NULL) { //iterate through every file in the game directory 
    
        if (strstr(dir_entry->d_name, ".rooms") != NULL) { //if the file is a "rooms" directory

            stat(dir_entry->d_name, &statbuf);

            // If the current directory was modified after the current workingDir
            // Or if this is the first rooms dir encountered. 
            if ((difftime(statbuf.st_mtim.tv_sec, moddedTime) > 0) || (workingDir == "")) { 
                workingDir = dir_entry->d_name;
                printf("workingdir is %s \n", workingDir);

                moddedTime = statbuf.st_mtim.tv_sec;
            }
        }            
    }

    closedir(dr);

    //char* dirPointer = (char*) workingDir;
    printf("Returning %s \n", workingDir);
    return workingDir;
} 

void readRooms (char * dirName) {
    
    struct dirent * dir_entry; 

    printf("Dir name is %s \n", dirName);

    DIR *dr = opendir(dirName);

    while ((dir_entry = readdir(dr)) != NULL) { //iterate through every file in the game directory 
        
        printf("name of file is %s \n", dir_entry->d_name);

    }
}

int main(){    
    
    struct Room * rooms[7]; 
    char * workingDir = "";

    printf("Entering findcurrent \n");

    workingDir = findCurrentDirectory(); //Find the most recent directory created by buildrooms.c
   
    printf("Working dir now is %s \n", workingDir);

    readRooms(workingDir); //read all Room files into the rooms array

    return 0;
}
