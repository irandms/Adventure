#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <time.h>

struct Room {
    char* name; 
    int connections[6];
};

char * findCurrentDirectory() { 
    
    struct dirent * dir_entry;
    struct stat statbuf;

    DIR * dr = opendir(".");

    char workingDir[20] = "";
    time_t moddedTime;

    while ((dir_entry = readdir(dr)) != NULL) { //iterate through every file in the game directory 
    
        if (strstr(dir_entry->d_name, ".rooms") != NULL) { //if the file is a "rooms" directory

            stat(dir_entry->d_name, &statbuf);

            if (difftime(statbuf.st_mtim.tv_sec, moddedTime) > 0) { // If the current directory was modified after the current workingDir
                strcpy(workingDir, dir_entry->d_name);
                //workingDir = dir_entry->d_name;

                moddedTime = statbuf.st_mtim.tv_sec;
            }
        }            
    }

    closedir(dr);

    char* dirPointer = (char*) workingDir;
    return dirPointer;

} 

int main(){

    char * workingDir;
    
    workingDir = findCurrentDirectory(); //
    
    //printf("working dir is %s \n", workingDir);

    return 0;
}
