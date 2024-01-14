/* KEYBOARD TASK SCHEDULER */
/* IRQ 1 fires off > IRQ 1 stores scancode into queue > process_scan_code() handles the scan code */

#include "kts.h"

static bool shift_key = false;
static bool capslock_key = false;
static bool in_editor = false;
static bool files_displayed = false;

static uint8_t scan_code_buffer[BUFFER_SIZE];
static volatile int buffer_head = 0;
static volatile int buffer_tail = 0;

static char keymap_standard[60] = {
    0,  // 0x00
    27, // Escape
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', // 0x01 to 0x0D
    '\b', // 0x0E BackSpace
    '\t', // 0x0F TAB
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', // 0x10 to 0x1C
    191, // Left control
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', // 0x1D to 0x27
    174, // Left shift
    '\\', // 0x2B 
    'z', 'x', 'c', 'v', 'b', 'n', 'm', // 0x2C to 0x32
    ',',  // 0x33
    '.',  // 0x34
    '/', // 0x35
    175, // 0x36 right shift
    42, // 0x37 (keypad) * pressed
    142, // 0x38 left alt pressed
    32, // 0x39 Space
    184, // 0x3A CapsLock
    '\f', // 0x3B F1 
};

static char keymap_shift[60] = {
    0,  // 0x00
    27, // Escape
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', // 0x01 to 0x0D
    '\b', // 0x0E BackSpace
    '\t', // 0x0F TAB
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', // 0x10 to 0x1C
    191, // Left control
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', // 0x1D to 0x27
    174, // Left shift
    '|', // 0x2B 
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', // 0x2C to 0x32
    '<',  // 0x33
    '>',  // 0x34
    '?', // 0x35
    175, // 0x36 right shift
    42, // 0x37 (keypad) * pressed
    142, // 0x38 left alt pressed
    32, // 0x39 Space
    184, // 0x3A CapsLock
    '\f', // 0x3B F1
};

static char keymap_caps[60] = {
    0,  // 0x00
    27, // Escape
    '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', // 0x01 to 0x0D
    '\b', // 0x0E BackSpace
    '\t', // 0x0F TAB
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', // 0x10 to 0x1C
    191, // Left control
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`', // 0x1D to 0x27
    174, // Left shift
    '\\', // 0x2B 
    'Z', 'X', 'C', 'V', 'B', 'N', 'M', // 0x2C to 0x32
    ',',  // 0x33
    '.',  // 0x34
    '/', // 0x35
    175, // 0x36 right shift
    42, // 0x37 (keypad) * pressed
    142, // 0x38 left alt pressed
    32, // 0x39 Space
    184, // 0x3A CapsLock
    '\f', // 0x3B F1 
};

void enqueue_scan_code(uint8_t scancode)
{
    // Add scancode to the buffer
    int next = (buffer_head + 1) % BUFFER_SIZE;
    if (next != buffer_tail)
    {
        scan_code_buffer[buffer_head] = scancode;
        buffer_head = next;
    }
}

uint8_t dequeue_scan_code()
{
    // Retrieve scancode from the buffer
    if (buffer_tail != buffer_head)
    {
        uint8_t scancode = scan_code_buffer[buffer_tail];
        buffer_tail = (buffer_tail + 1) % BUFFER_SIZE;
        return scancode;
    }
    return 0; // Buffer is empty
}

void process_scan_code (void)
{
    while (buffer_tail != buffer_head)
    {
        uint8_t scancode = dequeue_scan_code();

        if (scancode == 0x02 && in_editor == false) { // 1 - show files
            show_files();
            files_displayed = true;
            return;
        }
        if (scancode == 0x03 && in_editor == false) { // 2 - open "editor"
            terminal_clear();
            in_editor = true;
            return;
        }

        if (scancode == 0x04) { // 3 - show elapsed time
            show_elapsed_time();
            return;
        }

        if (scancode == 0x01 && in_editor) { // esc - create new file
            char *buffer = terminal_save_buffer_state();
            create_file(buffer);
            terminal_clear();
            in_editor = false;
            print_welcome_string();
            return;
        }

        if (scancode == 0x38 && in_editor) { // left alt - save changes to the file
            char *buffer = terminal_save_buffer_state();
            save_changes_to_file(buffer);
            terminal_clear();
            in_editor = false;
            print_welcome_string();
            return;
        }

        /* open the file */
        if (files_displayed && (scancode == 0x10 || scancode == 0x11 || scancode == 0x12 || scancode == 0x13 || scancode == 0x14)) {
            files_displayed = false;

            if (scancode == 0x10) { // q
                bool does_file_exist = check_file_exists_by_index(0);
                in_editor = true;
                if (does_file_exist) {
                    open_file(0);
                } else {return;}
                
            } else if (scancode == 0x11) { // w
                bool does_file_exist = check_file_exists_by_index(1);
                in_editor = true;
                if (does_file_exist) {
                    open_file(1);
                } else {return;}

            } else if (scancode == 0x12) { // e
                bool does_file_exist = check_file_exists_by_index(2);
                in_editor = true;
                if (does_file_exist) {
                    open_file(2);
                } else {return;}
            } else if (scancode == 0x13) { // r
                bool does_file_exist = check_file_exists_by_index(3);
                in_editor = true;
                if (does_file_exist) {
                    open_file(3);
                } else {return;}
            } else if (scancode == 0x14) { // t
                bool does_file_exist = check_file_exists_by_index(4);
                in_editor = true;
                if (does_file_exist) {
                    open_file(4);
                } else {return;}
            }
            return;
        }

        /* delete the file*/
        if (files_displayed && (scancode == 0x2C || scancode == 0x2D || scancode == 0x2E || scancode == 0x2F || scancode == 0x30)) {
            files_displayed = false;

            if (scancode == 0x2C) { // z
                bool does_file_exist = check_file_exists_by_index(0);
                if (does_file_exist) {
                    delete_file(0);
                } else {return;}
            } else if (scancode == 0x2D) { // x
                bool does_file_exist = check_file_exists_by_index(1);
                if (does_file_exist) {
                    delete_file(1);
                } else {return;}
            } else if (scancode == 0x2E) { // c
                bool does_file_exist = check_file_exists_by_index(2);
                if (does_file_exist) {
                    delete_file(2);
                } else {return;}
            } else if (scancode == 0x2F) { // v
                bool does_file_exist = check_file_exists_by_index(3);
                if (does_file_exist) {
                    delete_file(3);
                } else {return;}
            } else if (scancode == 0x30) { // b
                bool does_file_exist = check_file_exists_by_index(4);
                if (does_file_exist) {
                    delete_file(4);
                } else {return;}
            }
            return;
        }

        if (scancode == 0x2A || scancode == 0x36) // Shift pressed
        {
            shift_key = true;
        }
        else if (scancode == 0xAA || scancode == 0xB6) // Shift released
        {
            shift_key = false;
        }

        if (scancode == 0x3A)  // Caps Lock pressed
        {
            capslock_key = !capslock_key;  // Toggle the Caps Lock state
        }

        // If scancode is in array range and it's not ctrl or alt
        if (scancode < 60 && scancode != 0x1D && scancode != 0x2A && scancode != 0x36
        && scancode != 0x38 && scancode != 0x3A && scancode != 0xBA)
        {
            if (capslock_key) 
            {
                if (shift_key) 
                {
                    const char key_pressed_scan_char = keymap_standard[scancode];
                    terminal_write(&key_pressed_scan_char, sizeof(key_pressed_scan_char));
                    return;
                }
                const char key_pressed_scan_char = keymap_caps[scancode];
                terminal_write(&key_pressed_scan_char, sizeof(key_pressed_scan_char));
            }
            else if (shift_key) 
            {
                const char key_pressed_scan_char = keymap_shift[scancode];
                terminal_write(&key_pressed_scan_char, sizeof(key_pressed_scan_char));
            } 
            else 
            {
                const char key_pressed_scan_char = keymap_standard[scancode];
                terminal_write(&key_pressed_scan_char, sizeof(key_pressed_scan_char));
            }
        }
    }
}