#include "types.h"
#include "multiboot.h"

struct display_info_t {
    uint width;
    uint height;
    char* framebuffer_location;
};

void drawString(char* string);
void consoleInit(multiboot_info_t *buffer_loc);