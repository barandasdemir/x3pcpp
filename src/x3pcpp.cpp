//
// Created by baran on 17/08/18.
//

#include "x3pcpp.h"
#include <iostream>
#include <filesystem>
#include "unzipper.h"
#include <rapidxml.hpp>

x3p::x3p(std::string path) {
    try {
        std::vector<unsigned char> databuf, infobuf;

        zipper::Unzipper unzipper(path);
        std::vector<zipper::ZipEntry> entries = unzipper.entries();
        unzipper.extractEntryToMemory(entries.at(0).name, databuf);
        unzipper.extractEntryToMemory(entries.at(1).name, infobuf);
        unzipper.close();

        auto *data = reinterpret_cast<double*>(databuf.data());
        auto *info = reinterpret_cast<char*>(infobuf.data());
        uint64_t len = infobuf.size();
        info[len] = '\0';

        rapidxml::xml_document<> metaXML;
        metaXML.parse<0>(info);
        rapidxml::xml_node<> *node;

        node = metaXML.first_node()->first_node("Record3")->first_node("MatrixDimension");
        this->sizeX = std::stoi(node->first_node("SizeX")->value());
        this->sizeY = std::stoi(node->first_node("SizeY")->value());

        node = metaXML.first_node()->first_node("Record1")->first_node("Axes");
        this->incrementX = std::stold(node->first_node("CX")->first_node("Increment")->value());
        this->incrementY = std::stold(node->first_node("CY")->first_node("Increment")->value());

//        this->matrix = arma::mat(this->sizeX, this->sizeY);

        for (int j = 0; j <= this->sizeY; j++) {
            std::vector<double> col;
            int element = (j * this->sizeX);
            for (unsigned long i = (j > 0) ? 1 : 0; i < this->sizeX; i++) {
                col.push_back(data[element + i]);
            }
            this->binMatrix.push_back(col);
//            arma::mat C(col);
//            matrix.insert_cols(j, C);
        }
//        matrix.print("matrix: ");
    } catch (const std::runtime_error& e) {
        std::cerr << e.what();
    }
}

std::vector<double> x3p::getCol(int col) {
    return binMatrix.at(col);
}

std::vector<double> x3p::getRow(int row) {
    std::vector<double> rowbuf;
    for (unsigned long i = 0; i < sizeY; i++) {
        rowbuf.push_back(getCol(i).at(row));
    }
    return rowbuf;
}

double x3p::getVal(int row, int col) {
    return getCol(col).at(row);
}

void x3p::fortify() {
//    int rowCount = sizeX * sizeY;
//    std::vector<long double> x, y, z;
//    std::vector<std::vector<long double>> frame;
//    for (int i = 0; i < rowCount; i++) {
//        x.push_back(i % sizeX);
//        y.push_back(i % sizeY);
//    }
}

int main(int argc, char *argv[]) {
    std::cout << "testing with included x3p file\n";
    x3p bullet("../bullet1.x3p");
    std::cout << "value at 3rd row, 1st column (3rd value in data.bin): " << bullet.getVal(2,0) << std::endl;

    return 0;
}
