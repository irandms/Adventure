#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

struct Room {
    char* name; 
    int connections[6];
};

//char * findCurrentDirectory {
    
//} 

int main(){

    struct dirent * dir_entry;
    struct stat statbuf;

    DIR * dr = opendir(".");

    char working_dir[10];

    while ((dir_entry = readdir(dr)) != NULL) { //iterate through every file in the game directory 
    
        if (strstr(dir_entry->d_name, "room.") == NULL) { //if the file is a "rooms" directory
            printf("Dir Name is %s", dir_entry->d_name);
            //if (working_dir == NULL) 
            //    working_dir = dir_entry->d_name; // Assume the first room directory encountered is the working directory

            //else {
                
            //}
        }            
    }

    closedir(dr);

    return 0;
}
