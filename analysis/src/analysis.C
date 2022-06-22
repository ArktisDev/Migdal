#include "TROOT.h"
#include "TFile.h"

#include <string>

#include "DataStructs.hh"
#include "DetectorSurfaceCounts.hh"

int main(int argc, char** argv)
{
    std::string inputFilename = "../../build/output.root";
    
    if (argc == 2) {
        inputFilename = std::string(argv[0]);
    }
    
    TFile tfile(inputFilename.c_str());
    SetupDataStructs(&tfile);
    
    DrawTotalCountsHist();
}