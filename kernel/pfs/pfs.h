#ifndef PFS_PFS_H
#define PFS_PFS_H

#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "kernel/tty.h"
#include <stdlib.h>
#include <stdbool.h>

#define MAX_DATA_SIZE 1975
#define MAX_NUM_OF_FILES 5
#define FILE_NAME_LENGTH 25

struct File 
{
    char filename[FILE_NAME_LENGTH];
    char data[MAX_DATA_SIZE];
};

struct FileSystem
{
    struct File files[MAX_NUM_OF_FILES];
};

void create_file(char* buffer);
void show_files(void);
void open_file(uint8_t num);
void delete_file(uint8_t num);
bool check_file_exists_by_index(size_t num);
void save_changes_to_file(char* buffer);

#endif 