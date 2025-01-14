#include <stdlib.h>
#include <string.h>


// return image data padded with pad layers of given value
unsigned char* pad(unsigned char* image_data, int width, int height, int pad, unsigned char value);

// direct convolution of matrix with kernel
unsigned char* conv(unsigned char* data, int width, int height, float* k_data, int k_size);

// // fast-fourier transform convolution of matrix with kernel
// template <typename T>
// unsigned char* fft_conv(unsigned char* data, int width, int height, T* kernel, int k_width, int k_height);
