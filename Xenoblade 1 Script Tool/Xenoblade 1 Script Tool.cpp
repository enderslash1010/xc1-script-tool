// Xenoblade 1 Script Tool.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <fstream>
#include "Script.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 2) cout << "Usage: " << argv[0] << " <fileName>" << endl;
    else {
        try {
            Script script(argv[1]);
        }
        catch (exception& e) {
            cout << e.what() << endl;
        }
    }
}

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
