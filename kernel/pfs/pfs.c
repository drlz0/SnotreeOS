#include "pfs.h"


struct FileSystem myFileSystem;

void create_file(char* buffer)
{
    // Extract filename from buffer
    char filename[FILE_NAME_LENGTH];
    strncpy(filename, buffer, FILE_NAME_LENGTH);
    filename[FILE_NAME_LENGTH - 1] = '\0';  // Null-terminate the filename

    // Find the position of '\n' in the filename and replace it with '\0'
    char* new_line_pos = strchr(filename, '\n');
    if (new_line_pos != NULL)
    {
        *new_line_pos = '\0';
    }

    // Find an empty slot in the file system
    size_t empty_slot = MAX_NUM_OF_FILES;
    for (size_t i = 0; i < MAX_NUM_OF_FILES; ++i)
    {
        if (myFileSystem.files[i].filename[0] == '\0')
        {
            empty_slot = i;
            break;
        }
    }

    // If no empty slot found, return
    if (empty_slot == MAX_NUM_OF_FILES)
    {
        terminal_writestring("Error: File system full.\n");
        return;
    }

    // Copy filename and buffer data to the file system
    strncpy(myFileSystem.files[empty_slot].filename, filename, FILE_NAME_LENGTH - 1);
    myFileSystem.files[empty_slot].filename[FILE_NAME_LENGTH - 1] = '\0';

    // Copy data from buffer starting from the 26th character
    strncpy(myFileSystem.files[empty_slot].data, buffer + FILE_NAME_LENGTH, MAX_DATA_SIZE - 1);
    myFileSystem.files[empty_slot].data[MAX_DATA_SIZE - 1] = '\0';

    terminal_writestring("File created successfully.\n");
}

void show_files(void)
{
    size_t empty_slot = MAX_NUM_OF_FILES;
    for (size_t i = 0; i < MAX_NUM_OF_FILES; ++i)
    {
        if (myFileSystem.files[i].filename[0] == '\0')
        {
            empty_slot = i;
            break;
        }
    }
    char qwert[5] = "qwert";
    char zxcvb[5] = "zxcvb";
    terminal_writestring("List of files:\n");
    for (size_t i = 0; i < empty_slot; i++)
    {
        terminal_writestring(myFileSystem.files[i].filename);
        terminal_writestring("press ");
        terminal_putchar(qwert[i]);
        terminal_writestring(" for opening the file or ");
        terminal_putchar(zxcvb[i]);
        terminal_writestring(" for deleting the file ");
    }
}

/* TODO: SAVE PREVIOUS CURSOR POSITION */
void open_file(uint8_t num)
{
    char temp_buffer[FILE_NAME_LENGTH + MAX_DATA_SIZE];

    strncpy(temp_buffer, myFileSystem.files[num].filename, FILE_NAME_LENGTH - 1);

    strncpy(temp_buffer + FILE_NAME_LENGTH, myFileSystem.files[num].data, MAX_DATA_SIZE - 1);
    
    size_t limit = FILE_NAME_LENGTH + MAX_DATA_SIZE - 1;
    for (size_t i = 0; i < limit; i++)
    {
        terminal_putchar(temp_buffer[i]);

    }
}

void delete_file(uint8_t num)
{
    if (num >= MAX_NUM_OF_FILES)
    {
        return;
    }

    // Shift the remaining files to fill the gap
    for (size_t i = num; i < MAX_NUM_OF_FILES - 1; i++)
    {
        memcpy(&myFileSystem.files[i], &myFileSystem.files[i + 1], sizeof(struct File));
    }

    // Clear the last file 
    memset(&myFileSystem.files[MAX_NUM_OF_FILES - 1], 0, sizeof(struct File));
}

bool check_file_exists_by_index(size_t num)
{
    if (num < MAX_NUM_OF_FILES)
    {
        // Check if the file at the specified index has a non-empty filename
        return myFileSystem.files[num].filename[0] != '\0';
    }

    return false; // Invalid file index
}

void save_changes_to_file(char* buffer)
{
    char filename[FILE_NAME_LENGTH];
    strncpy(filename, buffer, FILE_NAME_LENGTH);
    filename[FILE_NAME_LENGTH - 1] = '\0';

    char* new_line_pos = strchr(filename, '\n');
    if (new_line_pos != NULL)
    {
        *new_line_pos = '\0';
    }

    for (size_t i = 0; i < MAX_NUM_OF_FILES; ++i)
    {
        if (strcmp(myFileSystem.files[i].filename, filename) == 0)
        {
            // Match found, update the data in the file
            strncpy(myFileSystem.files[i].filename, filename, FILE_NAME_LENGTH - 1);
            myFileSystem.files[i].filename[FILE_NAME_LENGTH - 1] = '\0';

            // Copy data from buffer starting from the 26th character
            strncpy(myFileSystem.files[i].data, buffer + FILE_NAME_LENGTH, MAX_DATA_SIZE - 1);
            myFileSystem.files[i].data[MAX_DATA_SIZE - 1] = '\0';
        }
    }
}