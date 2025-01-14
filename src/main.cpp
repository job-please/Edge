#include <iostream>
#include <chrono>
#include <filesystem>

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
using std::endl;
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
        cout << "ERROR: " << stbi_failure_reason() << "\n";
    }

    // create filter kernel, size = 5
    float k[] = {.04,.04,.04,.04,.04,
                 .04,.04,.04,.04,.04,
                 .04,.04,.04,.04,.04,
                 .04,.04,.04,.04,.04,
                 .04,.04,.04,.04,.04};

    unsigned char* pls = conv(image.data, image.width, image.height, k, 5);
    stbi_write_jpg("CONV.jpg", image.width, image.height, 1, pls, image.width);

    // free memory
    stbi_image_free(image.data);



    // // test code run time (ns)
    // auto start = std::chrono::steady_clock::now();

    // //code

    // auto end = std::chrono::steady_clock::now();
    // cout << "\n" << (end - start).count() << endl;

}