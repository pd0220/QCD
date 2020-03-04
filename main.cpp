// jackknife method implementation for Monte Carlo simulation statistics
// including useful libraries and/or headers

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>

// read given file
std::vector<double> readFile(std::string fileName)
{
    // string for lines
    std::string line;

    // data structure to store data
    int length = 0;
    std::vector<double> dataStructure(length, 0);

    // start reading
    std::ifstream fileToRead;
    fileToRead.open(fileName);
    // check if open
    if (fileToRead.is_open())
    {
        // read line by line
        while (std::getline(fileToRead, line))
        {
            // convert string to numbers and push to vector
            dataStructure.push_back(std::stod(line));
        }
        // close file
        fileToRead.close();
    }
    // error check
    else
    {
        std::cout << "Unable to open given file." << std::endl;
        std::exit(-1);
    }

    // return raw data
    return dataStructure;
}

//

// main function
// argv[1] is the datafile to analyze
int main(int argc, char **argv)
{
    // file name
    std::string fileName = "None";
    // check for arguments
    if (argc > 1)
    {
        fileName = argv[1];
    }
    // error check
    if (fileName == "None")
    {
        std::cout << "No file name was given, or the file dose not exist or unavailable." << std::endl;
        std::exit(-1);
    }

    // create container for raw data
    std::vector<double> rawData = readFile(fileName);

    return 0;
}