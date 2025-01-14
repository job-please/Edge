#include "..\include\conv.h"


// ASSUMES EVEN PADDING DESIRED ON ALL SIDES
unsigned char* pad(unsigned char* data, int width, int height, int pad, unsigned char value)
{
    // get padded image width , height
    int pad_width = width + pad * 2;
    int pad_height = height + pad * 2;

    // allocate memory to store padded image data
    unsigned char* pad_data = (unsigned char*)malloc(pad_width * pad_height);
    if (pad_data == NULL)
    {
        throw("malloc() failed");
    }

    // set values of pad_data
    int pad_block_size = pad_width * pad; // size of top and bottom padding blocks
    memset(pad_data, value, pad_block_size); // first pad rows
    unsigned char* pad_row_start = pad_data + pad_block_size; // pointer to start of each row of padded data
    for (int i = 0; i < height; i++) // assign each row of pad_data
    {
        memset(pad_row_start, value, pad); 
        memcpy((pad_row_start + pad), data, width);
        memset((pad_row_start + pad + width), value, pad);
        // increment pointers to next row
        pad_row_start += pad_width;
        data += width;
    }
    memset(pad_row_start, value, pad_block_size); // last pad rows

    // return padded image
    return pad_data;
}

// ASSUMES SQUARE KERNEL
// Potentially slow for large kernel
unsigned char* conv(unsigned char* data, int width, int height, float* k_data, int k_size)
{
    // handle edges by padding with zeros
    unsigned char* pad_data = pad(data, width, height, k_size / 2, 0);
    int pad_width = width + k_size - 1;
    int pad_height = height + k_size - 1;
    int size = width * height;

    // allocate memory to store result
    unsigned char* conv_data = (unsigned char*)calloc(size, 1);
    if (conv_data == NULL)
    {
        throw("malloc() failed");
    }

    // perform convolution
    int pad_idx = 0;
    for (int i=0; i < size; i++)
    {
        for (int j = 0; j < k_size*k_size; j++)
        {
            *(conv_data + i) += *(k_data + j) * (*(pad_data + pad_idx + (j / k_size)*pad_width + (j % k_size)));
        }

        if ((i + 1) % width == 0)
        {
            pad_idx += k_size - 1;
        }
        pad_idx++;
    }

    return conv_data;
}