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
    
    TTree* tree = new TTree("Aggregate Entering", "Title");
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
    
    outFile.cd();
    
    tree = new TTree("Aggregate Leaving", "Title");
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
    
    std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::vector<float>>>> outputData;
    PerParticleEnteringFlux(&outputData);
    
    outFile.cd();
    
    TTree* parentTree = new TTree("Flux Entering", "Title");
    
    for (const std::pair<std::string, std::unordered_map<std::string, std::vector<std::vector<float>>>>& pair : outputData) {
        std::string particleName = pair.first;
        tree = new TTree(particleName.c_str(), "Title");
        // // Friend new tree so we see it as "branch" of parent
        //parentTree->AddFriend(particleName.c_str());
        
        struct DataStruct {
            double coord1;
            double coord2;
            double energy;
        };
        
        // Iterate over different sides
        for (const std::pair<std::string, std::vector<std::vector<float>>>& pair : pair.second) {
            std::string side = pair.first;
            
            DataStruct data;
            
            // Add a new one and turn it on
            tree->Branch(side.c_str(), &data, "coord1/D:coord2/D:energy/D");
            
            // turn off all branches
            tree->SetBranchStatus("*", false);
            
            // turn on the one we want
            tree->SetBranchStatus(side.c_str(), true);
            
            // enter all rows of data into branch
            for (const std::vector<float>& row : pair.second) {
                data.coord1 = row[0];
                data.coord2 = row[1];
                data.energy = row[2];
                
                tree->Fill();
            }
        }
        
        // Turn all branches on now
        tree->SetBranchStatus("*", true);
        
        // Done filling up tree with data, write it
        tree->Write();
    }
    
    std::cout << "Got here" << std::endl;
    
    //parentTree->Write();
    //parentTree->Delete();
    
    // // Repeat everything for leaving flux
    
    // inFile.cd();
    // PerParticleLeavingFlux(&outputData);
    
    // outFile.cd();
    
    // parentTree = new TTree("Flux Leaving", "Title");
    
    // for (const std::pair<std::string, std::unordered_map<std::string, std::vector<std::vector<float>>>>& pair : outputData) {
    //     std::string particleName = pair.first;
    //     tree = new TTree(particleName.c_str(), "Title");
    //     // Friend new tree so we see it as "branch" of parent
    //     parentTree->AddFriend(particleName.c_str());
        
    //     struct DataStruct {
    //         double coord1;
    //         double coord2;
    //         double energy;
    //     };
        
    //     // Iterate over different sides
    //     for (const std::pair<std::string, std::vector<std::vector<float>>>& pair : pair.second) {
    //         std::string side = pair.first;
            
    //         DataStruct data;
            
    //         // turn off all branches
    //         tree->SetBranchStatus("*", false);
            
    //         // Add a new one and turn it on
    //         tree->Branch(side.c_str(), &data, "coord1/D:coord2/D:energy/D");
    //         tree->SetBranchStatus(side.c_str(), true);
            
    //         // enter all rows of data into branch
    //         for (const std::vector<float>& row : pair.second) {
    //             data.coord1 = row[0];
    //             data.coord2 = row[1];
    //             data.energy = row[2];
                
    //             tree->Fill();
    //         }
    //     }
        
    //     // Done filling up tree with data, write it and delete
    //     tree->Write();
    //     tree->Delete();
    // }
    
    // parentTree->Write();
    // parentTree->Delete();
    
    inFile.Close();
    outFile.Close();
}