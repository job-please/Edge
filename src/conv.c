#include <stdlib.h>
#include <string.h>



unsigned char* pad(unsigned char* image_data, int width, int height, int pad, unsigned char value)
{
    // get padded image width , height
    int pad_width = width + pad * 2;
    int pad_height = height + pad * 2;

    // allocate memory to store padded image data
    unsigned char* pad_data = (unsigned char*)malloc(pad_width * pad_height);

    // set values of pad_data
    int pad_block_size = pad_width * pad; // size of top and bottom padding blocks
    memset(pad_data, value, pad_block_size); // first pad rows
    unsigned char* pad_row_start = pad_data + pad_block_size; // pointer to start of each row of padded data
    for (int i = 0; i < height; i++) // assign each row of pad_data
    {
        memset(pad_row_start, value, pad); 
        memcpy((pad_row_start + pad), image_data, width);
        memset((pad_row_start + pad + width), value, pad);
        // increment pointers to next row
        pad_row_start += pad_width;
        image_data += width;
    }
    memset(pad_row_start, value, pad_block_size); // last pad rows

    // return padded image
    return pad_data;
}
