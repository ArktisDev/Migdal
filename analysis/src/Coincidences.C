#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include <string>

#include "DataStructs.hh"
#include "Timing.hh"
#include "Coincidences.hh"

int main(int argc, char** argv)
{
    if (argc < 3) {
        std::cout << "You need three arguments, see source code" << std::endl;
        return 0;
    }
    
    std::string inputFilename = std::string(argv[1]);
    int nParticles = std::stoi(argv[2]);
    
    int samplesPerIteration = 1000;
    if (argc > 3) {
        samplesPerIteration = std::stoi(argv[3]);
    }
    
    int iterations = 1;
    if (argc > 4) {
        iterations = std::stoi(argv[4]);
    }
    
    std::string strippedInputFilename = inputFilename.substr(0, inputFilename.length() - 5);
    
    TFile inFile(inputFilename.c_str());
    SetupDataStructs(&inFile);
    
    Timer timer;
    
    timer.start();
    std::string outFile = strippedInputFilename + std::string("_rr_coincidences.root");
    CalculateRecoilCoincidences(&inFile, outFile, iterations, samplesPerIteration, nParticles);
    std::cout << "1 took " << timer.elapsedMilli() << "ms to run" << std::endl;
    
    timer.start();
    outFile = strippedInputFilename + std::string("_re_coincidences.root");
    CalculateEdepRecoilCoincidences(&inFile, outFile, iterations, samplesPerIteration, nParticles);
    std::cout << "2 took " << timer.elapsedMilli() << "ms to run" << std::endl;
    
    inFile.Close();
}