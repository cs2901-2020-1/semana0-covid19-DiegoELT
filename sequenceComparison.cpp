#include <iostream>
#include <fstream>
#include <algorithm> // Merely used to rotate strings to the right. Not for actual similarity calculation. 

std::string loadFileIntoString(std::string filename) // Loads the file into the sequence variable.
{
    std::string sequence, line;
    std::ifstream genomeFile(filename);
    if (genomeFile.is_open())
    {
        getline(genomeFile, line);
        while(getline(genomeFile, line))
        {
            for (size_t i = 0; i < line.size(); i++)
            {
                if(line[i] >= 'A' && line[i] <= 'Z') // To not include the \n at the end of each line.
                    sequence += line[i];
            }
        }
    }
    else std::cout << "File wasn't able to be opened.";
    return sequence;
}

int hammingDistance(std::string stringA, std::string stringB) // Chosen because of its O(n) complexity, useful for simple genome comparisons.
{
    int numberOfDifferences = 0; 
    for (size_t i = 0; i < stringA.size(); i++)
    {
        if(stringA[i] != stringB[i])
            numberOfDifferences++;
    }
    return numberOfDifferences;
}

void padding(std::string &sequence, int difference) // Adds a fixed amount of '-' characters to the sequence to be able to use Hamming distance.
{
    for (size_t i = 0; i < difference; i++)
        sequence += '-';
}

void shiftString(std::string &sequence) // Shifts the string one position to the right. 
{
    std::rotate(sequence.begin(), sequence.begin()+sequence.size()-1, sequence.end());
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        std::cout << "Usage of the sequenceComparison:\n./sequenceComparison <file1> <file2>";
        return 0; 
    }

    // Loading the filenames into the sequences, and creating variables.
    std::string sequenceA, sequenceB, filenameA, filenameB;
    filenameA = argv[1]; 
    filenameB = argv[2];
    sequenceA = loadFileIntoString(filenameA);
    sequenceB = loadFileIntoString(filenameB);
    int difference, similarityValue, tempValue; 

    if(sequenceA.size() > sequenceB.size())
    {
        similarityValue = sequenceA.size(); // The default value would be if the sequences where completely different, so the size of the largest string.
        difference = sequenceA.size() - sequenceB.size();
        padding(sequenceB, difference); // We pad the smaller string.
        for (size_t i = 0; i <= difference; i++) 
        {
            tempValue = hammingDistance(sequenceA, sequenceB); // If the value obtained by the hammingDistance is smaller, it becomes the new minimum.
            if(tempValue < similarityValue)
                similarityValue = tempValue; 
            shiftString(sequenceB); // We shift the string one to the right, so we try all possible allignments (common issue in genomes)
        }
    }
    else // Same case but if the other sequence is the smaller one. 
    {
        similarityValue = sequenceB.size();
        difference = sequenceB.size() - sequenceA.size();
        padding(sequenceA, difference);
        for (size_t i = 0; i <= difference; i++)
        {
            tempValue = hammingDistance(sequenceA, sequenceB);
            if(tempValue < similarityValue)
                similarityValue = tempValue;
            shiftString(sequenceA);
        }
    }

    // Output the amount of differences and similarity percentage. 
    std::cout << "The amount of differences (by Hamming Distance) between the sequences is: "  << similarityValue << "\n" << "Percentage of similarity: " << 100 * (1 - float(similarityValue)/float(sequenceA.size())) << "%";
}