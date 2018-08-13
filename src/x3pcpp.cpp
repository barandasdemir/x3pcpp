#include "x3pcpp.h"
#include <iostream>
#include <filesystem>
#include "unzipper.h"

x3p::x3p(std::string path) {
    try {
        zipper::Unzipper unzipper(path);
        std::vector<zipper::ZipEntry> entries = unzipper.entries();
        unzipper.extractEntryToMemory(entries.at(0).name, this->data);
        unzipper.extractEntryToMemory(entries.at(1).name, this->meta);
        unzipper.close();

        double* dval = reinterpret_cast<double*>(this->data.data());
        for (int i = 0; i < 10; i++) {
            std::cout << dval[i] << "\n";
        }

    } catch (const std::runtime_error& e) {
        std::cerr << e.what();
    }
}

int main (int argc, char *argv[]) {
    x3p x3p("bullet1.x3p");

    return 0;
}