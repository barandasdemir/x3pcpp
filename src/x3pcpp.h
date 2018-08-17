//
// Created by baran on 17/08/18.
//

#ifndef X3PCPP_X3PCPP_H
#define X3PCPP_X3PCPP_H

#include <string>
#include <vector>
#include <armadillo>

class x3p {
public:
    x3p(std::string path);
    std::vector<double> getCol(int col);
    std::vector<double> getRow(int row);
    double getVal(int row, int col);

private:
    arma::mat matrix;
    std::vector<std::vector<double>> binMatrix;
    int sizeX, sizeY;
    long double incrementX, incrementY;
    void fortify();

};


#endif //X3PCPP_X3PCPP_H
