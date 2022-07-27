#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include <string>

#include "DataStructs.hh"

bool InsideFieldCage(double x, double y, double z, double xmin, double xmax, double ymin, double ymax, double zmin, double zmax) {
    return (x > xmin) && (x < xmax) && (y > ymin) && (y < ymax) && (z > zmin) && (z < zmax);
}

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cout << "You need to specify a file" << std::endl;
        return 0;
    }
    
    std::string inputFilename = std::string(argv[1]);
    std::string strippedInputFilename = inputFilename.substr(0, inputFilename.length() - 5);
    
    TFile inFile(inputFilename.c_str());
    SetupDataStructs(&inFile);

    EnteringData enteringData;
    LeavingData leavingData;
    RecoilsData recoilData;
    EdepData edepData;
    MetaData metaData;
    metaData.ReadEntry(0);

    double cageXmin = metaData.cageXmin;
    double cageXmax = metaData.cageXmax;
    double cageYmin = metaData.cageYmin;
    double cageYmax = metaData.cageYmax;
    double cageZmin = metaData.cageZmin;
    double cageZmax = metaData.cageZmax;

    std::string outFileName = strippedInputFilename + std::string("_fieldCage.root");

    TFile outFile(outFileName.c_str(), "RECREATE");
    
    TTree* newEnteringTree = enteringData.tree->CloneTree(0);

    newEnteringTree->CopyEntries(enteringData.tree);

    outFile.cd();
    outFile.Write();

    TTree* newLeavingTree = leavingData.tree->CloneTree(0);

    newLeavingTree->CopyEntries(leavingData.tree);
    
    outFile.cd();
    outFile.Write();

    TTree* newRecoilTree = recoilData.tree->CloneTree(0);

    for (int i = 0; i < edepData.entries; i++) {
        edepData.ReadEntry(i);

        if (InsideFieldCage(edepData.x, edepData.y, edepData.z, cageXmin, cageXmax, cageYmin, cageYmax, cageZmin, cageZmax)) {
            newRecoilTree->Fill();
        }
    }

    outFile.cd();
    outFile.Write();

    TTree* newEdepTree = edepData.tree->CloneTree(0);

    for (int i = 0; i < edepData.entries; i++) {
        edepData.ReadEntry(i);

        if (InsideFieldCage(edepData.x, edepData.y, edepData.z, cageXmin, cageXmax, cageYmin, cageYmax, cageZmin, cageZmax)) {
            newEdepTree->Fill();
        }
    }

    outFile.cd();
    outFile.Write();
}