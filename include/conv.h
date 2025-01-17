// ASSUME ALL INPUT DATA IS STORED IN ROW MAJOR ORDER
// ASSUME 1-CHANNEL INPUT DATA

// return matrix padded with pad layers of given value
unsigned char* pad(unsigned char* image_data, int width, int height, int pad, unsigned char value);

// direct convolution of matrix with kernel
unsigned char* conv(unsigned char* data, int width, int height, float* k_data, int k_size);

// // fast-fourier transform convolution of matrix with kernel
// template <typename T>
// unsigned char* fft_conv(unsigned char* data, int width, int height, T* kernel, int k_width, int k_height);

// box blur
unsigned char* box_blur(unsigned char* data, int width, int height, int radius);

// gaussian blur
unsigned char* gauss_blur(unsigned char* data, int width, int height, int radius, float sigma);

// 
// 
// HELPERS

// pad sides of matrix
unsigned char* pad_hor(unsigned char* image_data, int width, int height, int pad, unsigned char value);

// pad top/bottom of matrix
unsigned char* pad_ver(unsigned char* image_data, int width, int height, int pad, unsigned char value);

// convolution with 1-D row filter
unsigned char* one_d_row_conv(unsigned char* data, int width, int height, float* k_data, int k_len);

// convolution with 1-D column filter
unsigned char* one_d_col_conv(unsigned char* data, int width, int height, float* k_data, int k_len);



