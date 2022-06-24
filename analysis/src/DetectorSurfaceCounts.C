#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include <string>

#include "DataStructs.hh"
#include "DetectorSurfaceCounts.hh"

int main(int argc, char** argv)
{
    std::string inputFilename = "../../geant/data/default/output.root";
    
    if (argc == 2) {
        inputFilename = std::string(argv[1]);
    }
    
    std::string outputFileName = inputFilename.substr(0, inputFilename.length() - 5).append("_counts.root");;
    
    TFile inFile(inputFilename.c_str());
    SetupDataStructs(&inFile);
    
    // Create per particle counts of each surface and totals
    std::unordered_map<std::string, int> total;
    std::unordered_map<std::string, int> negx;
    std::unordered_map<std::string, int> negy;
    std::unordered_map<std::string, int> negz;
    std::unordered_map<std::string, int> posx;
    std::unordered_map<std::string, int> posy;
    std::unordered_map<std::string, int> posz;
    
    TotalSurfaceEnteringCounts(&total);
    PerSurfaceEnteringCounts(&posx, &negx, &posy, &negy, &posz, &negz);
    
    TFile outFile = TFile(outputFileName.c_str(), "RECREATE");
    outFile.cd();
    
    int count;
    char name[12];
    char side[6];
    
    TTree* tree = new TTree("Entering", "Title");
    tree->Branch("Side", &side, "side[6]/C");
    tree->Branch("Name", &name, "name[12]/C");
    tree->Branch("Count", &count, "Count/I");
    
    strcpy(side, "Total");
    for (const std::pair<std::string, int>& pair : total) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Posx");
    for (const std::pair<std::string, int>& pair : posx) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Negx");
    for (const std::pair<std::string, int>& pair : negx) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Posy");
    for (const std::pair<std::string, int>& pair : posy) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Negy");
    for (const std::pair<std::string, int>& pair : negy) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Posz");
    for (const std::pair<std::string, int>& pair : posz) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Negz");
    for (const std::pair<std::string, int>& pair : negz) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    tree->Write();
    tree->Delete();
    
    inFile.cd();
    
    TotalSurfaceLeavingCounts(&total);
    PerSurfaceLeavingCounts(&posx, &negx, &posy, &negy, &posz, &negz);
    
    inFile.Close();
    outFile.cd();
    
    tree = new TTree("Leaving", "Title");
    tree->Branch("Side", &side, "side[6]/C");
    tree->Branch("Name", &name, "name[12]/C");
    tree->Branch("Count", &count, "Count/I");
    
    strcpy(side, "Total");
    for (const std::pair<std::string, int>& pair : total) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Posx");
    for (const std::pair<std::string, int>& pair : posx) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Negx");
    for (const std::pair<std::string, int>& pair : negx) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Posy");
    for (const std::pair<std::string, int>& pair : posy) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Negy");
    for (const std::pair<std::string, int>& pair : negy) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Posz");
    for (const std::pair<std::string, int>& pair : posz) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    strcpy(side, "Negz");
    for (const std::pair<std::string, int>& pair : negz) {
        strcpy(name, pair.first.c_str());
        count = pair.second;
        
        tree->Fill();
    }
    
    tree->Write();
    tree->Delete();

    outFile.Close();
}