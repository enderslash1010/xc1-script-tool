#include <iostream>
#include <fstream>
#include "Script.h"

int main(int argc, char *argv[])
{
    if (argc != 2) std::cout << "Usage: " << argv[0] << " <fileName>" << std::endl;
    else {
        try {
            Script script(argv[1]);
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}
