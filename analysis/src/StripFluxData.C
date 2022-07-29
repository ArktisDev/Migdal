#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include <string>

#include "DataStructs.hh"

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

    std::string outFileName = strippedInputFilename + std::string("_stripped.root");

    TFile outFile(outFileName.c_str(), "RECREATE");
    
    TTree* newRecoilTree = recoilData.tree->CloneTree(0);
    std::cout << "Copying recoil tree" << std::endl;
    newRecoilTree->CopyEntries(recoilData.tree);

    outFile.cd();
    outFile.Write();

    TTree* newEdepTree = edepData.tree->CloneTree(0);
    std::cout << "Copying edep tree" << std::endl;
    newEdepTree->CopyEntries(edepData.tree);

    outFile.cd();
    outFile.Write();

    TTree* newMetaTree = metaData.tree->CloneTree(0);

    std::cout << "Copying metadata tree" << std::endl;
    newMetaTree->CopyEntries(metaData.tree);

    outFile.cd();
    outFile.Write();
}