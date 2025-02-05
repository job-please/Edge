#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "..\include\page.h"

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



float* fconv(unsigned char* data, int width, int height, float* k_data, int k_radius)
{
    // handle edges by padding with zeros
    unsigned char* pad_data = pad(data, width, height, k_radius / 2, 0);
    int pad_width = width + k_radius - 1;
    int pad_height = height + k_radius - 1;
    int size = width * height;

    // allocate memory to store result
    float* conv_data = (float*)calloc(size, sizeof(float));
    if (conv_data == NULL)
    {
        throw("Memory allocation failed!");
    }

    // perform convolution
    int pad_idx = 0;
    int k_size = k_radius * k_radius;
    for (int i=0; i < size; i++)
    {
        for (int j = 0; j < k_size; j++)
        {
            *(conv_data + i) += *(k_data + j) * (*(pad_data + pad_idx + (j / k_radius)*pad_width + (j % k_radius)));
        }

        if ((i + 1) % width == 0)
        {
            pad_idx += k_radius - 1;
        }
        pad_idx++;
    }

    // free pointers
    free(pad_data);

    return conv_data;
}



unsigned char* box_blur(unsigned char* data, int width, int height, int radius)
{
    // set variables, allocate memory for result
    int size = width * height;
    int pad_width = width + radius - 1;
    int pad_size = pad_width * (height + radius - 1);
    unsigned char* pad_data = pad(data, width, height, radius/2, 0);
    unsigned char* blur_data = (unsigned char*)malloc(size);
    if (blur_data == NULL)
    {
        throw("Memory allocation failed!");
    }

    // perform blur
    int pad_idx = 0;
    int temp_sum = 0;
    int k_size = radius*radius;
    for (int i=0; i < size; i++)
    {
        for (int j = 0; j < k_size; j++)
        {
            temp_sum += *(pad_data + pad_idx + (j / radius)*pad_width + (j % radius));
        }

        *(blur_data + i) = temp_sum / k_size;
        temp_sum = 0;

        if ((i + 1) % width == 0)
        {
            pad_idx += radius - 1;
        }
        pad_idx++;
    }

    // free pointers
    free(pad_data);
    
    return blur_data;
}



unsigned char* sobel(unsigned char* data, int width, int height)
{
    // intialize necessary 1D kernels and vars
    int size = width * height;
    float kx[] = {-1,0,1,
                  -2,0,2,
                  -1,0,1};
    float ky[] = {-1,-2,-1,
                  0,0,0,
                  1,2,1};

    float* sobel_x = fconv(data, width, height, kx, 3);
    float* sobel_y = fconv(data, width, height, ky, 3);

    float* sobel_intensity = (float*)malloc(size * sizeof(float));
    if (sobel_intensity == NULL)
    {
        throw("Memory allocation failed!");
    }

    float max = -1.0;
    for (int i=0; i < size; i++)
    {
        float s_x = *(sobel_x + i);
        float s_y = *(sobel_y + i);

        *(sobel_intensity + i) = sqrtf(s_x*s_x + s_y*s_y);
        if (*(sobel_intensity + i) > max)
        {
            max = *(sobel_intensity + i);
        }
    }

    // free pointers
    free(sobel_x);
    free(sobel_y);

    // normalize
    unsigned char* gradient = (unsigned char*)malloc(size);
    if (gradient == NULL)
    {
        throw("Memory allocation failed!");
    }
    for (int i=0; i < size; i++)
    {
        *(gradient + i) = (unsigned char)((*(sobel_intensity + i) / max) * 255);
    }

    return gradient;
}



// sobel() with additonal pointer passed which to store edge angles
unsigned char* sobel2(unsigned char* data, int width, int height, float** angle_ptr)
{
    // intialize necessary 1D kernels and vars
    int size = width * height;
    float kx[] = {-1,0,1,
                  -2,0,2,
                  -1,0,1};
    float ky[] = {-1,-2,-1,
                  0,0,0,
                  1,2,1};

    float* sobel_x = fconv(data, width, height, kx, 3);
    float* sobel_y = fconv(data, width, height, ky, 3);

    float* intensity = (float*)malloc(size * sizeof(float));
    float* angle = (float*)malloc(size * sizeof(float)); // dont need to store angle, could use 1/2/3/4 for each case
    if (intensity == NULL || angle == NULL)
    {
        throw("Memory allocation failed!");
    }

    float max = -1.0;
    for (int i=0; i < size; i++)
    {
        float s_x = *(sobel_x + i);
        float s_y = *(sobel_y + i);

        *(intensity + i) = sqrtf(s_x*s_x + s_y*s_y);
        *(angle + i) = atan2f(s_y, s_x); // see above, could be faster

        if (*(intensity + i) > max)
        {
            max = *(intensity + i);
        }
    }

    // free pointers
    free(sobel_x);
    free(sobel_y);

    // normalize
    unsigned char* gradient = (unsigned char*)malloc(size);
    if (gradient == NULL)
    {
        throw("Memory allocation failed!");
    }
    for (int i=0; i < size; i++)
    {
        *(gradient + i) = (unsigned char)((*(intensity + i) / max) * 255);
    }

    *angle_ptr = angle;
    return gradient;
}



unsigned char* nonmax_sup(unsigned char* sobel_data, float* angle, int width, int height)
{
    int size = width * height;

    // allocate memory for result
    unsigned char* sup_data = (unsigned char*)malloc(size);
    if (sup_data == NULL)
    {
        throw("Memory allocation failed!");
    }

    // ignore horizontal edges
    unsigned char a;
    unsigned char b;
    for (int i=width+1; i < size - width; i++)
    {
        float a = *(angle + i);
        if (a < 0)
        {
            a += 1.57079; // make positive
        }

        if (a <= 22.5 || a >= 157.5) // 0 degrees
        {
            a = *(sobel_data + i - 1);
            b = *(sobel_data + i + 1);
        }
        else if (a < 67.5) // 45 degrees
        {
            a = *(sobel_data + i - width + 1);
            b = *(sobel_data + i + width - 1);
        }
        else if (a < 112.5) // 90 degrees
        {
            a = *(sobel_data + i - width);
            b = *(sobel_data + i + width);
        }
        else // 135 degrees
        {
            a = *(sobel_data + i - width - 1);
            b = *(sobel_data + i + width + 1);
        }

        // ignore vertical edges
        if (i % (width - 1) == 0)
        {
            i += 2;
        }
        
        if (*(sobel_data + i) >= a && *(sobel_data + i) >= b)
        {
            *(sup_data + i) = *(sobel_data + i);
        }
        else
        {
            *(sup_data + i) = 0;
        }
    }

    return sup_data;
}



// could be done in place, but not in case original data is needed
unsigned char* threshold(unsigned char* data, int size, int low, int high)
{
    unsigned char* thresh_data = (unsigned char*)malloc(size);
    if (thresh_data == NULL)
    {
        throw("Memory allocation failed!");
    }

    // identify strong/weak/irrelevant pixels
    for (int i=0; i < size; i++)
    {
        if (*(data + i) >= high)
        {
            *(thresh_data + i) = 255;
        }
        else if (*(data + i) >= low)
        {
            *(thresh_data + i) = 50;
        }
        else
        {
            *(data + i) = 0;
        }
    }

    return thresh_data;
}



// unsigned char* hysterisis(unsigned char* data, int size, int width, int weak, int strong)
// {
//     unsigned char* hys_data = (unsigned char*)malloc(size);
//     if (hys_data == NULL)
//     {
//         throw("Memory allocation failed!");
//     }
//     for (int i=width; i < size-width; i++)
//     {
//         if (*(data + i) == weak)
//         {
//             if (*(data + i - width - 1) == strong ||
//                 *(data + i - width) == strong ||
//                 *(data + i - width + 1) == strong ||
//                 *(data + i - 1) == strong ||
//                 *(data + i + 1) == strong ||
//                 *(data + i + width - 1) == strong ||
//                 *(data + i + width) == strong ||
//                 *(data + i + width + 1) == strong)
//             {
//                 *(hys_data + i) = 255;
//             }
//             else
//             {
//                 *(hys_data + i) = 0;
//             }

//             if (i % (width-1) == 0)
//             {
//                 i += 2;
//             }
//         }
//     }

//     return hys_data;
// }



unsigned char* EDGE(unsigned char* data, int width, int height)
{
    int size = width * height;

    // pre-process
    // unsigned char* b1 = box_blur(data, width, height, 5);
    // unsigned char* b2 = box_blur(b1, width, height, 5);
    // unsigned char* b3 = box_blur(b2, width, height, 3);

    // sobel filter
    float* angle;
    unsigned char* sob = sobel2(data, width, height, &angle);
    // free(b1);
    // free(b2);

    // nonmax suppresion
    unsigned char* sup = nonmax_sup(sob, angle, width, height);
    free(sob);

    // double threshold
    unsigned char* thresh = threshold(sup, size, (.1 * 255), (.3 * 255));
    free(sup);

    // hysterisis
    // unsigned char* hys = hysterisis(thresh, size, width, 50, 255);
    // free(thresh);

    return thresh;
}