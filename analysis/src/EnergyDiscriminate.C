#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include <string>

#include "DataStructs.hh"

int main(int argc, char** argv) {
    std::string inputFilename = "../../geant/data/default/output.root";
    
    if (argc > 1) {
        inputFilename = std::string(argv[1]);
    }

    double energyCut = 2; // keV
    if (argc > 2) {
        energyCut = std::stod(argv[2]);
    }

    std::string outputFileName = inputFilename.substr(0, inputFilename.length() - 5).append("_discrim.root");

    TFile inFile(inputFilename.c_str());
    SetupDataStructs(&inFile);

    EnteringData enteringData;
    LeavingData leavingData;
    RecoilsData recoilData;
    EdepData edepData;
    MetaData metaData;

    TFile outFile(outputFileName.c_str(), "RECREATE");
    TTree* newEnteringTree = enteringData.tree->CloneTree(0);

    std::cout << "Now copying entering tree" << std::endl;
    newEnteringTree->CopyEntries(enteringData.tree);

    outFile.cd();
    outFile.Write();

    TTree* newLeavingTree = leavingData.tree->CloneTree(0);

    std::cout << "Now copying leaving tree" << std::endl;
    newLeavingTree->CopyEntries(leavingData.tree);
    
    outFile.cd();
    outFile.Write();

    TTree* newRecoilTree = recoilData.tree->CloneTree(0);

    std::cout << "Now copying recoil tree with energy cut" << std::endl;
    for (int i = 0; i < recoilData.entries; i++) {
        recoilData.ReadEntry(i);

        if (recoilData.energy > energyCut) {
            newRecoilTree->Fill();
        }
    }

    outFile.cd();
    outFile.Write();

    TTree* newEdepTree = edepData.tree->CloneTree(0);

    std::cout << "Now copying edep tree with energy cut" << std::endl;
    for (int i = 0; i < edepData.entries; i++) {
        edepData.ReadEntry(i);

        if (edepData.edep > energyCut) {
            newEdepTree->Fill();
        }
    }

    outFile.cd();
    outFile.Write();

    TTree* newMetaTree = metaData.tree->CloneTree(0);

    std::cout << "Now copying meta tree" << std::endl;
    newMetaTree->CopyEntries(metaData.tree);
    
    outFile.cd();
    outFile.Write();

    outFile.Close();
    inFile.Close();
}