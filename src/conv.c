#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "..\include\conv.h"


// ASSUME EVEN PADDING DESIRED ON ALL SIDES
unsigned char* pad(unsigned char* data, int width, int height, int pad, unsigned char value)
{
    // get padded image width , height
    int pad_width = width + pad * 2;
    int pad_height = height + pad * 2;

    // allocate memory to store result
    unsigned char* pad_data = (unsigned char*)malloc(pad_width * pad_height);
    if (pad_data == NULL)
    {
        throw("Memory allocation failed!");
    }

    // set values of pad_data
    int pad_block_size = pad_width * pad;
    memset(pad_data, value, pad_block_size);
    unsigned char* pad_row_start = pad_data + pad_block_size;
    for (int i = 0; i < height; i++)
    {
        memset(pad_row_start, value, pad); 
        memcpy((pad_row_start + pad), data, width);
        memset((pad_row_start + pad + width), value, pad);

        pad_row_start += pad_width;
        data += width;
    }
    memset(pad_row_start, value, pad_block_size);

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
        throw("Memory allocation failed!");
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



// THIS COULD BE IMPLEMENTED MUCH BETTER (don't need kernel, all values are same)
unsigned char* box_blur(unsigned char* data, int width, int height, int radius)
{
    // create 1-D kernel (row, col filters have same values for box blur)
    float* one_d_kernel = (float*)malloc(radius * sizeof(float));
    if (one_d_kernel == NULL)
    {
        throw("Memory allocation failed!");
    }
    float val = 1.0 / radius;
    for (int i=0; i < radius; i++)
    {
        *(one_d_kernel + i) = val;
    }

    // apply blur
    unsigned char* row_conv = one_d_row_conv(data, width, height, one_d_kernel, radius);
    unsigned char* blur_data = one_d_col_conv(row_conv, width, height, one_d_kernel, radius);
    free(row_conv);

    return blur_data;
}



// THIS COULD BE IMPLEMENTED BETTER (Normalize as you convolve so you don't have to run through the kernel an extra time)
unsigned char* gauss_blur(unsigned char* data, int width, int height, int radius, float sigma)
{
    float* one_d_kernel = (float*)malloc(radius * sizeof(float));
    if (one_d_kernel == NULL)
    {
        throw("Memory allocation failed!");
    }
    int len;
    int k_i = len = radius/2;
    float sum = 0;
    float k_val;
    for (int i = 0; i < len; i++)
    {
        k_val = (.39894 * exp(-(k_i*k_i)/(2 * sigma * sigma)) / sigma);
        k_i--;
        *(one_d_kernel + i) = k_val;
        *(one_d_kernel + radius - 1 - i) = k_val;
        sum += 2*k_val;
    }
    *(one_d_kernel + len) = .39894 * sigma;
    sum += *(one_d_kernel + len);

    // normalize
    for (int i=0; i < radius; i++)
    {
        *(one_d_kernel + i) /= sum;
    }


    // apply blur
    unsigned char* row_conv = one_d_row_conv(data, width, height, one_d_kernel, radius);
    unsigned char* blur_data = one_d_col_conv(row_conv, width, height, one_d_kernel, radius);
    free(row_conv);

    return blur_data;
}



//
// HELPERS
//

unsigned char* pad_hor(unsigned char* data, int width, int height, int pad, unsigned char value)
{
    // get padded image width
    int pad_width = width + pad * 2;

    // allocate memory to store result
    unsigned char* pad_data = (unsigned char*)malloc(pad_width * height);
    if (pad_data == NULL)
    {
        throw("Memory allocation failed!");
    }

    // set values of pad_data
    unsigned char* row = pad_data;
    for (int i = 0; i < height; i++)
    {
        memset(row, value, pad); 
        memcpy((row + pad), data, width);
        memset((row + pad + width), value, pad);
        
        row += pad_width;
        data += width;
    }

    // return padded image
    return pad_data;
}



unsigned char* pad_ver(unsigned char* data, int width, int height, int pad, unsigned char value)
{
    // math
    int size = width * height;
    int pad_block_size = width * pad;

    // allocate memory to store result
    unsigned char* pad_data = (unsigned char*)malloc(size + 2*pad_block_size);
    if (pad_data == NULL)
    {
        throw("Memory allocation failed!");
    }

    // set values of pad_data
    memset(pad_data, value, pad_block_size);
    memcpy((pad_data + pad_block_size), data, size);
    memset((pad_data + pad_block_size + size), value, pad_block_size);

    // return padded image
    return pad_data;
}



unsigned char* one_d_row_conv(unsigned char* data, int width, int height, float* k_data, int k_len)
{
    // handle edges by padding with zeros
    unsigned char* pad_data = pad_hor(data, width, height, k_len / 2, 0);
    int pad_width = width + k_len - 1;
    int size = width * height;

    // allocate memory to store result
    unsigned char* conv_data = (unsigned char*)calloc(size, 1);
    if (conv_data == NULL)
    {
        throw("Memory allocation failed!");
    }

    // perform convolution
    int pad_idx = 0;
    for (int i=0; i < size; i++)
    {
        for (int j = 0; j < k_len; j++)
        {
            *(conv_data + i) += *(k_data + j) * (*(pad_data + pad_idx + j));
        }

        if ((i + 1) % width == 0)
        {
            pad_idx += k_len - 1;
        }
        pad_idx++;
    }

    return conv_data;
}



unsigned char* one_d_col_conv(unsigned char* data, int width, int height, float* k_data, int k_len)
{
    // handle edges by padding with zeros
    unsigned char* pad_data = pad_ver(data, width, height, k_len / 2, 0);
    int pad_height = height + k_len - 1;
    int size = width * height;

    // allocate memory to store result
    unsigned char* conv_data = (unsigned char*)calloc(size, 1);
    if (conv_data == NULL)
    {
        throw("Memory allocation failed!");
    }

    // perform convolution
    for (int i=0; i < size; i++)
    {
        for (int j = 0; j < k_len; j++)
        {
            *(conv_data + i) += *(k_data + j) * (*(pad_data + i + j*width));
        }
    }

    return conv_data;
}