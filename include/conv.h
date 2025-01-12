#include <stdlib.h>
#include <string.h>



// return image data padded with pad layers of given value
    // sizeof(unsigned char) = 1 byte
    // change for loop to while loop ??? (index not necessary)
unsigned char* pad(unsigned char* image_data, int width, int height, int pad, unsigned char value);