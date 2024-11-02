// going to try doing some kind of scheduling, creating a program here and
// linking it in the kernel not going to make a separate program yet because
// that would require me to put some kind of filesystem in don't know how to do
// that so gonna put it in here instead
#include "functions.h"
int consoleBuf[128];
char hexString[3];

char charToAscii(char value) {
  if (value < 10) {
    return value + 0x30;
  }
  return value + 0x37;
}

char *charToHexString(char value) {
  hexString[0] = charToAscii(value & 0xF);
  hexString[1] = charToAscii((value >> 4) & 0xF);
  hexString[2] = 0;
  return hexString;
}

long hexStringToChar(int *string) {
  int i = 0;
  long dest = 0;
  while (string[i] != '\0') {
    char value = string[i];
    dest = dest << 4;
    uchar offset;
    if (value <= 0x3A) {
      offset = 0x30;
    } else {
      offset = 0x37;
    }
    dest = dest + (value - offset);
    i++;
  }
  return dest;
}

void printHex() {
  int size = 8;
  int hasArg = 1;
  if (consoleBuf[1] == 'c') {
    size = 1;
  } else if (consoleBuf[1] == 'i') {
    size = 2;
  } else if (consoleBuf[1] == 'l') {
    size = 4;
  } else {
    hasArg = 0;
  }
  long location = hexStringToChar((int *)consoleBuf + 1 + hasArg);
  volatile uchar *memLoc = (uchar *)location;
  for (int i = 0; i < size; i++) {
    char value = memLoc[i];
    drawString(charToHexString(value));
  }
}

int consoleTestProgMain() {
  int c;
  int index = 0;
  while (1) {
    c = getchar();
    if (c == 10) {
      switch (consoleBuf[0]) {
        case 'x':
          printHex();
      }
      for (int i = 0; i < 128; i++) {
        consoleBuf[i] = 0;
      }
      drawChar('\n');
      index = 0;
    } else if (c >= ' ' && c <= 'z') {
      consoleBuf[index] = c;
      drawChar(c - ' ');
      index++;
    } else if(c == 8){
      consoleBuf[index] = 0;
      deleteChar();
      index--;
    }
  }
  return 0;
}
