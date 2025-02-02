// ASSUMPTIONS
    // INPUT DATA IS STORED IN ROW MAJOR ORDER
    // 1-CHANNEL INPUT DATA

// pad input data with given value, given number of pad rows/cols
unsigned char* pad(unsigned char* data, int width, int height, int pad, unsigned char value);

// convolve uint8 matrix with kernel, return float result
float* fconv(unsigned char* data, int width, int height, float* k_data, int k_size);

// apply box blur to data
unsigned char* box_blur(unsigned char* data, int width, int height, int radius);

// apply Sobel filter kernel to highlight edges
unsigned char* sobel(unsigned char* data, int width, int height);

// sobel() with pointer passed to store edge angles
unsigned char* sobel2(unsigned char* data, int width, int height, float** angle_ptr);

// detect edges
unsigned char* EDGE(unsigned char* data, int width, int height);