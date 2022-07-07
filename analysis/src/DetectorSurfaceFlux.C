#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TDirectory.h"

#include <string>

#include "DataStructs.hh"
#include "DetectorSurfaceCounts.hh"

int main(int argc, char** argv)
{
    std::string inputFilename = "../../geant/data/default/output.root";
    
    if (argc == 2) {
        inputFilename = std::string(argv[1]);
    }
    
    std::string outputFileName = inputFilename.substr(0, inputFilename.length() - 5).append("_flux.root");;
    
    TFile inFile(inputFilename.c_str());
    SetupDataStructs(&inFile);
    
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::vector<float>>>> outputData;
    PerParticleEnteringFlux(&outputData);
    
    TFile outFile = TFile(outputFileName.c_str(), "RECREATE");
    outFile.cd();
    
    TDirectory* enteringDir = outFile.mkdir("Entering Flux");
    enteringDir->cd();
    
    for (const std::pair<std::string, std::unordered_map<std::string, std::vector<std::vector<float>>>>& pair : outputData) {
        std::string particleName = pair.first;
        
        // Make new directory for a specific particle
        TDirectory* particleDir = enteringDir->mkdir(particleName.c_str());
        particleDir->cd();
        
        // Iterate over different sides
        for (const std::pair<std::string, std::vector<std::vector<float>>>& pair : pair.second) {
            std::string side = pair.first;
            
            double coord1;
            double coord2;
            double energy;
            
            TTree* tree = new TTree(side.c_str(), "Title");
    
            tree->Branch("coord1", &coord1, "coord1/D");
            tree->Branch("coord2", &coord2, "coord2/D");
            tree->Branch("energy", &energy, "energy/D");
            
            // enter all rows of data into tree
            for (const std::vector<float>& row : pair.second) {
                coord1 = row[0];
                coord2 = row[1];
                energy = row[2];
                
                tree->Fill();
            }
            
            // Done filling up tree with data, write it
            tree->Write();
            tree->Delete();
        }
    }
    
    // Repeat everything for leaving flux
    
    inFile.cd();
    
    PerParticleLeavingFlux(&outputData);
    
    outFile.cd();
    
    TDirectory* leavingDir = outFile.mkdir("Leaving Flux");
    leavingDir->cd();
    
    for (const std::pair<std::string, std::unordered_map<std::string, std::vector<std::vector<float>>>>& pair : outputData) {
        std::string particleName = pair.first;
        
        // Make new directory for a specific particle
        TDirectory* particleDir = leavingDir->mkdir(particleName.c_str());
        particleDir->cd();
        
        // Iterate over different sides
        for (const std::pair<std::string, std::vector<std::vector<float>>>& pair : pair.second) {
            std::string side = pair.first;
            
            double coord1;
            double coord2;
            double energy;
            
            TTree* tree = new TTree(side.c_str(), "Title");
    
            tree->Branch("coord1", &coord1, "coord1/D");
            tree->Branch("coord2", &coord2, "coord2/D");
            tree->Branch("energy", &energy, "energy/D");
            
            // enter all rows of data into tree
            for (const std::vector<float>& row : pair.second) {
                coord1 = row[0];
                coord2 = row[1];
                energy = row[2];
                
                tree->Fill();
            }
            
            // Done filling up tree with data, write it
            tree->Write();
            tree->Delete();
        }
    }
    
    inFile.Close();
    outFile.Close();
}