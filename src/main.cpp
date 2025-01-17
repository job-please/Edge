#include <iostream>
#include <chrono>
#include <filesystem>
#include <math.h>

#define STB_IMAGE_IMPLEMENTATION
#include "..\include\stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "..\include\stb_image_write.h"

#include "..\include\conv.h"

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
    path image_path = std::filesystem::current_path().parent_path() / "test_pics" / "test_pic_2.jpg";

    // load image into Matrix
    Image image;
    int channels;
    image.data = stbi_load(image_path.string().c_str(), &image.width, &image.height, &channels, 1);

    // check if image loaded successfully
    if (image.data == NULL) 
    {
        throw(stbi_failure_reason());
    }    

    unsigned char* g = gauss_blur(image.data, image.width, image.height, 31, 1.0);
    stbi_write_jpg("BUH.jpg", image.width, image.height, 1, g, 0);
    stbi_write_jpg("OG.jpg", image.width, image.height, 1, image.data, 0);

    // free memory
    stbi_image_free(image.data);


    
    //
    // TESTING
    //

    // // create filter kernel, size = 5
    // float k[] = {.04,.04,.04,.04,.04,
    //              .04,.04,.04,.04,.04,
    //              .04,.04,.04,.04,.04,
    //              .04,.04,.04,.04,.04,
    //              .04,.04,.04,.04,.04};

    // float tk[] = {.11,.11,.11,
    //               .11,.11,.11,
    //               .11,.11,.11};

    // unsigned char t[] = {100,100,100,100,
    //                      100,100,100,100,
    //                      100,100,100,100};

}