#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include <string>

#include "DataStructs.hh"
#include "Timing.hh"
#include "Coincidences.hh"

int main(int argc, char** argv)
{
    std::string inputFilename = "../../geant/data/output.root";
    
    if (argc == 2) {
        inputFilename = std::string(argv[1]);
    }
    
    std::string strippedInputFilename = inputFilename.substr(0, inputFilename.length() - 5);
    
    TFile inFile(inputFilename.c_str());
    SetupDataStructs(&inFile);
    
    Timer timer;
    
    timer.start();
    std::string outFileName3DTTTP = strippedInputFilename + std::string("_3D_TTrack_TParticle.root");
    CC_3D_TTrack_TParticle(&inFile, outFileName3DTTTP);
    std::cout << "3D_TTrack_TParticle took " << timer.elapsedMilli() << "ms" << std::endl;
    
    timer.start();
    std::string outFileName2DTTTP = strippedInputFilename + std::string("_2D_xz_TTrack_TParticle.root");
    CC_2D_TTrack_TParticle(&inFile, outFileName2DTTTP, true, false, true);
    std::cout << "2d_xz_TTrack_TParticle took " << timer.elapsedMilli() << "ms" << std::endl;
    
    timer.start();
    std::string outFileName3DTTFP = strippedInputFilename + std::string("_3D_TTrack_FParticle.root");
    CC_3D_TTrack_FParticle(&inFile, outFileName3DTTFP);
    std::cout << "3D_TTrack_FParticle took " << timer.elapsedMilli() << "ms" << std::endl;
    
    timer.start();
    std::string outFileName2DTTFP = strippedInputFilename + std::string("_2D_xz_TTrack_FParticle.root");
    CC_2D_TTrack_FParticle(&inFile, outFileName2DTTFP, true, false, true);
    std::cout << "2d_xz_TTrack_FParticle took " << timer.elapsedMilli() << "ms" << std::endl;
    
    inFile.Close();
}