#include "console.h"
#include "functions.h"
#include "glyphs.h"
#include "multiboot.h"
#include "types.h"
#include <stdint.h>

// top left of screen is 0, 0
uint cur_col = 0;
uint cur_row = 0;

const uchar ftwd = 4; // font width
const uint fullGlyphWidth = ftwd + 1;
const uchar ftht = 8; // font height
uint position = 0;

const uint screen_width = 640;
const uint screen_height = 480;

const uint cols = screen_width / fullGlyphWidth;

uint32_t linear_buffer_loc;

multiboot_info_t *display_info;

char charBuffer[128];

void nextLine() {
  position = ((position / cols) + 1) * cols;
  for (int i = 0; i < 128; i++) {
    charBuffer[i] = 0;
  }
}

void drawCharToScreen(char character, uint32_t pixel_offset) {
  // ok what the fuck do i do here
  // uhhh
  uint *frame_buffer_loc =
      (uint *)(uint)display_info
          ->framebuffer_addr; // ??? only works if every pixel is 4 bytes
  // assume bpp is 32, every pixel is represented by a uint
  uint pixels = (ftwd * ftht);
  for (uint i = 0; i < pixels; i++) {
    uint32_t color = ((glyphs[(int)character] >> (pixels - i - 1)) & 0x1) == 1
                         ? 0xFFFFFFFF
                         : 0;
    uint base = ((i % ftwd) + pixel_offset) + (((i / ftwd)) * screen_width);
    frame_buffer_loc[base] = color;
  }
}

void drawChar(unsigned char character) {
  uint row = (position * fullGlyphWidth) / screen_width;
  uint col = ((position * fullGlyphWidth) % screen_width) / fullGlyphWidth;
  if (character == '\n') {
    nextLine();
  } else {
    drawCharToScreen(character,
                     (row * ftht * screen_width) + (col * fullGlyphWidth));
  }
}

void deleteChar(){
  position = position - 1;
  drawChar(' ' - ' ');
  position = position - 1;
}

int strlen(const char *str) {
  uint i = 0;
  while (str[i] != '\0')
    i++;
  return i;
}

void acceptCharacter(uchar character) {
  charBuffer[position] = character;
  position++;
}

void drawString(char *string) {
  uint len = strlen(string);
  for (uint i = 0; i < len; i++) {
    acceptCharacter(string[i]);
    drawChar(string[i] - ' ');
  }
}

void consoleInit(multiboot_info_t *d_info) {
  display_info = d_info;
  ioapicEnable(1);
}

int getchar() {
  int curPos = position;
  while (curPos == position) {
    // spin here until new key
  }
  return charBuffer[curPos];
}
