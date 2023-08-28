#include <iostream>
#include <fstream>
#include "Script.h"
#include "Tests.h"

void printUsageStatement(std::string progName) 
{
    std::cout << "Usage:\n" <<
        "Generate Output File: " << progName << " decompile <input_script> <output_csv>\n" <<
        "Generate Script File: " << progName << " compile <input_csv> <output_script>\n";
}

int main(int argc, char *argv[])
{
    /* Generate Output File: <prog_name> decompile <input_script> <output_csv>
       Generate Script File: <prog_name> compile <input_csv> <output_script>
    */
    try {
        if (argc != 4) printUsageStatement(argv[0]);
        else {
            if (strcmp(argv[1], "decompile") == 0) {
                Script script(argv[2], true);
                script.generateOutfile(argv[3]);
                std::cout << "Decompiled " << argv[2] << " to " << argv[3] << ".csv";
            }
            else if (strcmp(argv[1], "compile") == 0) {
                Script script(argv[2], false);
                script.generateScriptFile(argv[3]);
                std::cout << "Compiled " << argv[2] << " to " << argv[3] << ".sb";
            }
            else if (strcmp(argv[1], "test") == 0) {
                std::cout << "Running Tests...\n";
                Tests tests(argv[2], argv[3]);
                std::cout << "Completed Tests";
            }
            else printUsageStatement(argv[0]);
        }
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}
