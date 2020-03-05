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

// calculate estimator from given set of data (over the whole set)... simply mean
double mean(std::vector<double> rawData)
{
    // mean
    return std::accumulate(rawData.begin(), rawData.end(), 0.) / static_cast<double>(rawData.size());
}

// -----------------------------------------------------------------------------------------------------------------------------------

// calculate variance for jackknife method
double variance(std::vector<double> subsetMeans, double N, double estimator)
{
    // calculate pre-factor
    double preFactor = (N - 1) / N;
    // calculate sum
    auto add_square = [estimator](double sum, int i) {auto d = i - estimator; return sum + d * d; };
    return preFactor * std::accumulate(subsetMeans.begin(), subsetMeans.end(), 0.0, add_square);
}

// -----------------------------------------------------------------------------------------------------------------------------------

// taking out the nth value from the full set and returning the subset
std::vector<double> jackknifeCut(std::vector<double> rawData, int n)
{
    // copy the whole set
    std::vector subSet = rawData;
    // erase nth value from copy
    subSet.erase(subSet.begin() + n);
    // return subset
    return subSet;
}

// -----------------------------------------------------------------------------------------------------------------------------------

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
    // size of vector
    double N = static_cast<double>(rawData.size());

    // calculate estimator from full set of data
    double estimator = mean(rawData);

    // create N subsets always taking out the nth value and calculate means
    // empty vector
    std::vector<double> subsetMeans(0, 0);

    double test = 0;

    // jackknife subsets
    for (int i = 0; i < N; i++)
    {
        subsetMeans.push_back(mean(jackknifeCut(rawData, i)));
        test += (subsetMeans[i] - estimator) * (subsetMeans[i] - estimator);
    }

    // calculete variance
    double var = variance(subsetMeans, N, estimator);

    // check if bias to calculated or not
    if (argc > 2)
    {
        std::string ifBias = argv[2];
        if (ifBias == "unbiased")
        {
            // calculate bias
            double bias = mean(subsetMeans);

            // calculate unbiased estimator
            double estimatorUnbiased = estimator - (N - 1) * (bias - estimator);

            std::cout << "The unbiased estimator is: " << estimatorUnbiased << " with s.d.: " << std::sqrt(var) << std::endl;
        }
    }
    else
    {
        std::cout << "The estimator is: " << estimator << " with s.d.: " << std::sqrt(var) << std::endl;
    }

    return 0;
}