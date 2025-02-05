#include <iostream>
#include <chrono>
#include <filesystem>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "..\include\stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "..\include\stb_image_write.h"

#include "..\include\page.h"

template<typename T> struct Matrix {
    T* data;
    int width;
    int height;
};

typedef Matrix<unsigned char> Image;

// namespaces
using std::cout;
using std::filesystem::path;

void p(unsigned char* data, int width, int height) {
    for (int i=0; i<height; i++) {
        for (int j=0; j<width; j++) {
            cout << (int)*(data + i*width + j) << " ";
        }
        cout << "\n";
    }
}

// main
int main() {

    // get image path
    path image_path = std::filesystem::current_path().parent_path() / "test_pics" / "test_pic_4.jpg";

    // load image into Matrix
    Image image;
    int channels;
    image.data = stbi_load(image_path.string().c_str(), &image.width, &image.height, &channels, 1);

    // check if image loaded successfully
    if (image.data == NULL) 
    {
        throw(stbi_failure_reason());
    }    

    unsigned char* cool = EDGE(image.data, image.width, image.height);
    stbi_write_jpg("COOL.jpg", image.width, image.height, 1, cool, 0);


    // free memory
    free(cool);
    stbi_image_free(image.data);

}

// NOTES

// Lossy conversions from float to uint8 cause banding, BAD!!!