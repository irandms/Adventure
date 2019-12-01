#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <search.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "buildrooms.h"

#define MAX_ROOMS (7)

struct TestInput {
    bool connectionMap[7][7];
    char types[7][11];
};

struct TestInput *generate_TestInput(void) {
    bool connectionMap[7][7] = {0}; //An array that stores all of the room connections  
    char types[7][11] = {'\0'};

    mkdir("tmp", 0700); //Make room directory, with read, write, and execute power
    chdir("tmp"); 

    char *rooms[] = {"Kitten","Gargoyle","Lizard","Facebook","Computer","Witchy","Vacuum","Wonder","Liquor","Pride"}; 
    shuffle(rooms, 10); 
    initConnections(connectionMap, MAX_ROOMS);
    initTypes(types, MAX_ROOMS);

    struct TestInput *t = malloc(sizeof(struct TestInput));

    memcpy(t->connectionMap, connectionMap, sizeof(connectionMap));
    memcpy(t->types, types, sizeof(types));

    return t;
}

bool test_MinMaxConnections() {
    for(int i = 0; i < 1000; i++) {
        struct TestInput *tc = generate_TestInput();
        int num_failed_rooms = helper_test_MinMaxConnections(tc);
        if(num_failed_rooms != 0) {
            return false;
        }
    }

    return true;
}

int helper_test_MinMaxConnections(struct TestInput *tc) {
    int failed_rooms = 0;
    for(int cur_room = 0; cur_room < MAX_ROOMS; cur_room++) {
        int counted_connections = 0;
        for(int conn = 0; conn < MAX_ROOMS; conn++) {
           counted_connections += tc->connectionMap[cur_room][conn]; 
        }
        if (counted_connections < 3 || counted_connections > 6) {
            fprintf(stderr, " ↱ Room %u: %u connections found\n", cur_room, counted_connections);
            failed_rooms++;
        }
    }

    return failed_rooms;
}

int main() 
{
    srand(time(NULL)); //Seed randomization

    int tc_failcount = 0;
    if( !test_MinMaxConnections() ) {
        fprintf(stderr, "A graph with nodes that either have fewer than 3 or more than 6 connections has been detected\n");
        tc_failcount++;
    }

    fprintf(stderr, "A total of %u of %u tests failed.", tc_failcount, 1);
    if(tc_failcount == 0) {
        fprintf(stderr, " Congratulations!");
    }
    fprintf(stderr, "\n");

    return 0;

}
