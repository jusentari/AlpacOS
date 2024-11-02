#include "kbd.h"
#include "x86.h"

int kbdgetc(){
    uint status;
    // get keybaord status
    status = inb(KBSTATP);

    // if status has thee input flag disabled
    if((status & KBS_DIB) == 0)
        return -1;
    
    uint characterCode;
    // read in thru the data port
    characterCode = inb(KBDATAP);
    // return the map of the character i.e. 0x2 => 0x49 -> '1'
    return normalmap[characterCode];
}



/*
int kbdgetc(){
    static uint shift;
    static uchar *charcode[4] = {
        normalmap, shiftmap, ctlmap, ctlmap
    };
    uint st, data, c;
    // get the keyboard status
    st = inb(KBSTATP);
    // no idea what this flag means, probably that the kbd is not working somehow
    if((st & KBS_DIB) == 0)
        return -1;

    // get the actual keyboard data
    data = inb(KBDATAP);
    // if key is 0xE0, add E0ESC to shift
    if(data == 0xE0){
        shift |= E0ESC;
        return 0;
    } else if(data & 0x80){
        // Key released
        data = (shift & E0ESC ? data : data & 0x7F);
        shift &= ~(shiftcode[data] | E0ESC);
        return 0;
    } else if(shift & E0ESC){
        // Last character was an E0 escape; or with 0x80
        data |= 0x80;
        shift &= ~E0ESC;
    }

    shift |= shiftcode[data];
    shift ^= togglecode[data];
    c = charcode[shift & (CTL | SHIFT)][data];
    if(shift & CAPSLOCK){
        if('a' <= c && c <= 'z')
        c += 'A' - 'a';
        else if('A' <= c && c <= 'Z')
        c += 'a' - 'A';
    }
    return c;
}*/