// Do not include header file multiple times
#pragma once

#include <stddef.h>
#include <stdbool.h>

void shuffle(char *array[], size_t length);
void initConnections(bool connectionMap[7][7], int num_rows);
void initTypes(char* types[], int num_of_rooms);
void createFiles(bool connectionMap[7][7], char* types[], char *rooms[],  size_t num_of_rooms);
