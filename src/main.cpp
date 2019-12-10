#include <iostream>
#include "x3pcpp.h"

int main(int argc, char *argv[]) {
    if (argc < 2 || argc > 3) {
        std::cout << "usage: x3pcpp height.tiff out.x3p" << std::endl;
        return 1;
    }

    std::string imgFile = argv[1];
    std::string x3pFile = argv[2];

    x3p tiff;
    tiff.loadFromTiff(imgFile);
    tiff.write(x3pFile);
    return 0;
}
