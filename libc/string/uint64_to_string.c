#include <string.h>

char* uint64_to_string(uint64_t value) 
{
    static char buffer[21];  // Maximum length of a 64-bit integer in decimal is 20 digits, plus the null terminator
    int i = sizeof(buffer) - 1;

    // Handle the case where the value is zero separately
    if (value == 0) {
        buffer[i--] = '0';
    } else {
        while (value > 0 && i >= 0) {
            buffer[i--] = '0' + value % 10;
            value /= 10;
        }
    }

    return &buffer[i + 1];
}