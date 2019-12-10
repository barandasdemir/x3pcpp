#include <iostream>
#include <fstream>
#include <filesystem>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "rapidxml/rapidxml_print.hpp"
#include "rapidxml/rapidxml.hpp"
#include "x3pcpp.h"
#include "zipper.h"
#include "unzipper.h"

x3p::x3p() {
    // for creating x3p files manually
}

void x3p::write(std::string filename) {
    // since only the xml file is used, should it get inlined as a template or something?
    std::vector<unsigned char> infobuf;
    zipper::Unzipper unzipper("../bullet1.x3p");
    std::vector<zipper::ZipEntry> entries = unzipper.entries();
    unzipper.extractEntryToMemory(entries.at(1).name, infobuf);
    unzipper.close();

    auto *info = reinterpret_cast<char*>(infobuf.data());
    unsigned long len = infobuf.size();
    info[len] = '\0';

    rapidxml::xml_document<> metaXML;
    metaXML.parse<rapidxml::parse_no_data_nodes>(info);

    // because for some reason rapidxml needs these variables pre-defined
    // cannot convert them inside value()
    std::vector<std::string> nodeValues;
    nodeValues.push_back(std::to_string(sizeX));
    nodeValues.push_back(std::to_string(sizeY));
    nodeValues.push_back(std::to_string(incrementX));
    nodeValues.push_back(std::to_string(incrementY));

    rapidxml::xml_node<> *node;
    node = metaXML.first_node()->first_node("Record3")->first_node("MatrixDimension");
    node->first_node("SizeX")->value(nodeValues.at(0).c_str());
    node->first_node("SizeY")->value(nodeValues.at(1).c_str());
    node = metaXML.first_node()->first_node("Record1")->first_node("Axes");
    node->first_node("CX")->first_node("Increment")->value(nodeValues.at(2).c_str());
    node->first_node("CY")->first_node("Increment")->value(nodeValues.at(3).c_str());

    std::string projectRoot = std::filesystem::temp_directory_path().string() + "/x3pcpp/";
    std::filesystem::create_directory(projectRoot);

    std::ofstream outxml(projectRoot + "meta.xml");
    outxml << metaXML;
    outxml.close();

    std::string bindataDir = projectRoot + "bindata/";
    std::filesystem::create_directory(bindataDir);
    std::ofstream binfile(bindataDir + "data.bin", std::ofstream::binary);
//    std::ofstream asciifile(bindataDir + "data.txt");
    for (std::vector<double> col : binMatrix) {
        for (double val : col) {
            binfile.write(reinterpret_cast<char *>(&val), sizeof(val));
//            asciifile << val << " ";
        }
//        asciifile << std::endl;
    }
    binfile.close();
//    asciifile.close();

    std::ifstream binzip(bindataDir + "data.bin");
    zipper::Zipper zipfile(filename);
    zipfile.add(binzip, "bindata/data.bin");
    zipfile.add(projectRoot + "meta.xml");
    zipfile.close();
    binzip.close();

    std::filesystem::remove_all(projectRoot);
}

void x3p::loadFromTiff(const std::string path) {
    binMatrix.clear();
    cv::Mat img;

    try {
        img = cv::imread(path, cv::IMREAD_UNCHANGED);
    } catch (std::exception& e) {
        std::cerr << e.what();
    }

    int startRow = getTiffStartRow(&img);
    int i;
    for (i = startRow; i < img.rows; i++) {
        std::vector<double> col;
        for (int j = 0; j < img.cols; j++) {
            col.push_back(img.at<double>(i,j));
        }
        binMatrix.push_back(col);
        if (i != img.rows - 1) {
            if (isTiffRowEmpty(img.row(i + 1))) {
                break;
            }
        }
    }

//    std::reverse(binMatrix.begin(), binMatrix.end());

    sizeX = i - startRow;
    sizeY = img.cols;
    incrementX = incrementY = 1.5625e-06;
}

int x3p::getTiffStartRow(cv::Mat *mat) {
    int i;
    for (i = 0; i < mat->rows; i++) {
        bool flag = false;
        for (int j = 0; j < mat->cols; j++){
            if (mat->at<double>(i,j) != 0) {
                flag = true;
                break;
            }
        }
        if (flag) {
            break;
        }
    }
    return i;
}

bool x3p::isTiffRowEmpty(cv::Mat row) {
    for (int i = 0; i < row.cols; i++) {
        if (row.at<double>(0,i) != 0) {
            return true;
        }
    }
    return flag;
}

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
        sizeX = std::stoi(node->first_node("SizeX")->value());
        sizeY = std::stoi(node->first_node("SizeY")->value());

        node = metaXML.first_node()->first_node("Record1")->first_node("Axes");
        incrementX = std::stod(node->first_node("CX")->first_node("Increment")->value());
        incrementY = std::stod(node->first_node("CY")->first_node("Increment")->value());

        for (int j = 0; j <= sizeY; j++) {
            std::vector<double> col;
            int element = (j * sizeX);
            for (unsigned long i = (j > 0) ? 1 : 0; i < sizeX; i++) {
                col.push_back(data[element + i]);
            }
            binMatrix.push_back(col);
        }
    } catch (const std::runtime_error& e) {
        std::cerr << e.what();
    }
}

std::vector<double> x3p::getCol(int col) {
    return binMatrix.at(col);
}

std::vector<double> x3p::getRow(int row) {
    std::vector<double> rowbuf;
    for (int i = 0; i < sizeY; i++) {
        rowbuf.push_back(getCol(i).at(row));
    }
    return rowbuf;
}

double x3p::getVal(int row, int col) {
    return getCol(col).at(row);
}
