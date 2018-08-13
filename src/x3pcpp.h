#ifndef X3PCPP_LIBRARY_H
#define X3PCPP_LIBRARY_H

#include <filesystem>

void hello();

class x3p {
    public:
        x3p(std::string path);

    private:
        std::vector<unsigned char> data, meta;
        int64_t * axisX, axisY, axisZ;
        int64_t incrementX, incrementY;
};

#endif