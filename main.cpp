// jackknife method implementation for Monte Carlo simulation statistics
// including useful libraries and/or headers

#include <iostream>
#include <fstream>
#include <math.h>
#include <string>
#include <vector>
#include <numeric>

// -----------------------------------------------------------------------------------------------------------------------------------

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

// -----------------------------------------------------------------------------------------------------------------------------------

// calculate estimator from given set of data (over the whole set)... simplytaking simply taking the mean
template <typename T>
auto mean(std::vector<T> rawData)
{
    // mean
    return std::accumulate(rawData.begin(), rawData.end(), 0.) / static_cast<double>(rawData.size());
}

// -----------------------------------------------------------------------------------------------------------------------------------

// calculate variance FOR jackknife method
template <typename T1, typename T2, typename T3>
auto variance(std::vector<T1> subsetMeans, T2 N, T3 estimator)
{
    using R = decltype(subsetMeans[0] + N + estimator);
    // calculate pre-factor
    auto preFactor = (R)(N - 1) / N;
    // calculate sum
    auto add_square = [estimator](R sum, R i) {auto d = i - estimator; return sum + d * d; };
    return preFactor * std::accumulate(subsetMeans.begin(), subsetMeans.end(), 0.0, add_square);
}

// -----------------------------------------------------------------------------------------------------------------------------------

// taking out the nth block from the full set and returning the subset
template <typename T>
auto jackknifeCut(std::vector<T> const &rawData, int n, int numOfValsInBlock)
{
    // copy the whole set
    std::vector<T> subSet = rawData;
    // erase nth block from copy
    subSet.erase(subSet.begin() + n * numOfValsInBlock, subSet.begin() + (n + 1) * numOfValsInBlock);

    std::cout << n << " " << subSet.size () << std::endl;

    // return subset
    return subSet;
}

// -----------------------------------------------------------------------------------------------------------------------------------

// main function
// argv[1] is the datafile to analyze
// argv[2] is the number of blocks
// argv[3] --> if "unbiased" calculate unbiased estimator
int main(int argc, char **argv)
{
    // file name
    std::string fileName = "None";
    // number of blocks
    int numOfBlocks = 0;

    // check for arguments
    if (argc > 2)
    {
        fileName = argv[1];
        numOfBlocks = std::stoi(argv[2]);
    }

    // error check
    if (fileName == "None")
    {
        std::cout << "No file name was given, or the file dose not exist or unavailable." << std::endl;
        std::exit(-1);
    }
    else if (numOfBlocks == 0 || numOfBlocks <= 0)
    {
        std::cout << "No number of blocks was given, or given number cannot be interpreted (must be positive int)." << std::endl;
        std::exit(-1);
    }

    // create container for raw data
    std::vector<double> rawData = readFile(fileName);

    //throw away (erase) the first values to get equally sized blocks
    rawData.erase(rawData.begin(), rawData.begin() + static_cast<int>(rawData.size()) % numOfBlocks);

    // size of vector
    int sizeOfData = static_cast<int>(rawData.size());

    // number of values in a given block
    int numOfValsInBlock = (int)sizeOfData / numOfBlocks;

    // calculate estimator from full set of data
    double estimator = mean(rawData);

    // create blocks always taking out the nth block and calculate means
    // vector for block means
    std::vector<double> subsetMeans(numOfBlocks, 0.);

    // jackknife blocks and means until last block
    for (int n{0}; n < numOfBlocks; n++)
    {
        subsetMeans[n] = mean(jackknifeCut(rawData, n, numOfValsInBlock));
    }
    
    // calculete variance
    double var = variance(subsetMeans, numOfBlocks, estimator);

    // check if bias to be calculated or not
    if (argc > 3)
    {
        std::string ifBias = argv[3];
        if (ifBias == "unbiased")
        {
            // calculate bias
            double bias = mean(subsetMeans);

            // calculate unbiased estimator
            double estimatorUnbiased = estimator - (numOfBlocks - 1) * (bias - estimator);

            // return unbiased estimator
            std::cout << "The unbiased estimator is: " << estimatorUnbiased << " with s.d.: " << std::sqrt(var) << std::endl;
        }
    }
    else
    {
        // return estimator
        std::cout << "The estimator is: " << estimator << " with s.d.: " << std::sqrt(var) << std::endl;
    }

    // write means to screen
    for (int i{0}; i < numOfBlocks; i++)
    {
        std::cout << subsetMeans[i] << " ";
    }
    std::cout << "\n";

    return 0;
}