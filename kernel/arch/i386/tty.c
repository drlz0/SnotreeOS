#include <kernel/tty.h>

#include "vga.h"
#include "io.h"

/* The I/O ports */
#define FB_COMMAND_PORT     0x3D4
#define FB_DATA_PORT        0x3D5

/* The I/O port commands */
#define FB_HIGH_BYTE_COMMAND   14
#define FB_LOW_BYTE_COMMAND    15

#define TAB_WIDTH				4

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;
static uint16_t* const VGA_MEMORY = (uint16_t*) 0xB8000;

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;
char* auxiliary_buffer;

void fb_move_cursor(unsigned short pos) 
{
    outb(FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT, ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    outb(FB_DATA_PORT, pos & 0x00FF);
}
	
void terminal_initialize(void) 
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	fb_move_cursor(0);
	for (size_t y = 0; y < VGA_HEIGHT; y++) {
		for (size_t x = 0; x < VGA_WIDTH; x++) {
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = vga_entry(' ', terminal_color);
		}
	}
}


void terminal_setcolor(uint8_t color) 
{
	terminal_color = color;
}

static void terminal_scroll(uint8_t color) 
{
	for (size_t i = VGA_WIDTH; i < VGA_WIDTH * VGA_HEIGHT + VGA_WIDTH - 1; i++) {
		char temp = terminal_buffer[i];
		terminal_buffer[i-VGA_WIDTH] = vga_entry(temp, color);
	}
}

void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) 
{
	size_t index = y * VGA_WIDTH + x;

	if (index == VGA_WIDTH * VGA_HEIGHT + VGA_WIDTH - 1 || (index > VGA_WIDTH * VGA_HEIGHT - 1 && c == '\n')) { // scroll enable
		terminal_scroll(color);
	}

	// Replace '\n' with space 
    if (c == '\n') {
        terminal_buffer[index] = vga_entry(' ', color);
    } else {
        terminal_buffer[index] = vga_entry(c, color);
    }
}

void terminal_putchar(char c) 
{
    if (c == '\t') {
        // Handle tab by advancing the cursor to the next tab stop
        size_t tabs = (terminal_column + TAB_WIDTH) / TAB_WIDTH;
        size_t spaces = tabs * TAB_WIDTH - terminal_column;

        for (size_t i = 0; i < spaces; i++) {
            terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
            if (++terminal_column == VGA_WIDTH) {
                terminal_column = 0;
                if (++terminal_row == VGA_HEIGHT) {
                    terminal_scroll(terminal_color);
                    terminal_row--;
                }
            }
        }
    } else if (c == '\b') {
        // Handle backspace by moving the cursor back one position
        if (terminal_column > 0) {
            terminal_column--;
        } else if (terminal_row > 0) {
            terminal_row--;
            terminal_column = VGA_WIDTH - 1;
        }
        terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
    } else if (c == '\f') {
        // Handle 1 line backspace
        // Move the cursor to the end of the previous row
        if (terminal_row > 0) {
            terminal_row--;
            terminal_column = VGA_WIDTH - 1;
        }
    } else {
        // Handle other characters
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        if (++terminal_column == VGA_WIDTH || c == '\n') {
            if (terminal_row == VGA_HEIGHT - 1) {
                terminal_scroll(terminal_color);
                terminal_column = 0;
            } else {
                terminal_row++;
                terminal_column = 0;
            }
        }
    }

    fb_move_cursor(terminal_row * VGA_WIDTH + terminal_column);
}


void terminal_write(const char* data, size_t size) 
{
	for (size_t i = 0; i < size; i++)
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data) 
{
	terminal_write(data, strlen(data));
}

void terminal_clear(void) 
{
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
    terminal_row = 0;
    terminal_column = 0;

    fb_move_cursor(0);
}

char *terminal_save_buffer_state(void) 
{
    size_t limit = VGA_WIDTH*VGA_HEIGHT - 1;
    for (size_t i = 0; i < limit; i++) {
        unsigned char character = (unsigned char)(terminal_buffer[i] & 0xFF);
        auxiliary_buffer[i] = character;
    }
    return auxiliary_buffer;
}