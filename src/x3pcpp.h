#ifndef X3PCPP_X3PCPP_H
#define X3PCPP_X3PCPP_H

#include <string>
#include <vector>
#include <opencv2/core/core.hpp>

class x3p {
    public:
        x3p();
        x3p(std::string path);
        std::vector<double> getCol(int col);
        std::vector<double> getRow(int row);
        double getVal(int row, int col);
        void loadFromTiff(std::string path);
        void write(std::string filename);

    private:
        std::vector<std::vector<double>> binMatrix;
        double incrementX, incrementY;
        int getTiffStartRow(cv::Mat *mat);
        bool isTiffRowEmpty(cv::Mat row);
        int sizeX, sizeY;
};


#endif //X3PCPP_X3PCPP_H
