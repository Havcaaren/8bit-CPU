#include <iostream>

#include "Rosetta_Stone.h"

int main(int argc, char** argv) {
    if (argc == 1) {
        std::cout<<"No input file."<<std::endl;
        return -1;
    }
    // "..\\test.casm" argv[1]
    std::ifstream in(argv[1]);
    if (!in.is_open()) {
        std::cout<<"Cannot open file."<<std::endl;
        return -2;
    }
    in.close();


    try {
        Rosetta_Stone ps(argv[1]);
        ps.parse();
        ps.print();
        ps.create_hex();
    } catch (std::invalid_argument& e) {
        std::cerr<<e.what()<<std::endl;
        return -3;
    }
    return 0;
}
