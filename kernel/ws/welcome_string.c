#include "welcome_string.h"
#include "../include/kernel/tty.h"

void print_welcome_string ()
{
    printf("Welcome to\n");
    terminal_setcolor(0b1011);
    printf("                                                    \n");
    printf("     _____             _                  _____ _____ \n");
    printf("    /  ___|           | |                |  _  /  ___|\n");
    printf("    \\ `--. _ __   ___ | |_ _ __ ___  ___ | | | \\ `--. \n");
    printf("     `--. \\ '_ \\ / _ \\| __| '__/ _ \\/ _ \\| | | |`--. \\\n");
    printf("    /\\__/ / | | | (_) | |_| | |  __/  __/\\ \\_/ /\\__/ /\n");
    printf("    \\____/|_| |_|\\___/ \\__|_|  \\___|\\___| \\___/\\____/ \n");
    printf("                                                    \n");
    printf("                                                    \n");
    terminal_setcolor(0b0111);
    printf("1 - show files\n");
    printf("2 - create file\n");
    printf("3 - show the time elapsed from the start\n");
}