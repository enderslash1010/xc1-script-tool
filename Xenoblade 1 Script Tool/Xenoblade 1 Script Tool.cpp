#include <iostream>
#include <fstream>
#include "Script.h"

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
    if (argc != 4) printUsageStatement(argv[0]);
    else {
        if (strcmp(argv[1], "decompile") == 0) {
            try {
                Script script(argv[2]);
                script.generateOutfile(argv[3]);
            }
            catch (std::exception& e) {
                std::cout << e.what() << std::endl;
            }
        }
        else if (strcmp(argv[1], "compile") == 0) {
            std::cout << "unimplemented\n";
        }
        else printUsageStatement(argv[0]);
    }
}
