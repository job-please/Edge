#include <iostream>
#include <chrono>
#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include "..\include\stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "..\include\stb_image_write.h"

#include "..\include\conv.h"

// namespaces
using std::cout;
using std::endl;
using std::filesystem::path;

// main
int main() {

    // get image path
    path image_path = std::filesystem::current_path().parent_path() / "test_pics" / "test_pic_2.jpg";

    // load image
    int width, height, channels;
    unsigned char* data = stbi_load(image_path.string().c_str(), &width, &height, &channels, 1);
    int size = width * height;

    // check if image loaded successfully
    if (data == NULL) 
    {
        cout << "ERROR: " << stbi_failure_reason() << "\n";
    }

    int p = 7;
    unsigned char* pad_data = pad(data, width, height, 7, 255);
    stbi_write_jpg("OG.jpg", width, height, 1, data, width);
    stbi_write_jpg("pad.jpg", width + 2*p, height + 2*p, 1, pad_data, width + 2*p);

    // free memory
    stbi_image_free(data);




    // // test code run time (ns)
    // auto start = std::chrono::steady_clock::now();

    // //code

    // auto end = std::chrono::steady_clock::now();
    // cout << "\n" << (end - start).count() << endl;

}