#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"

#include <string>

#include "DataStructs.hh"
#include "DetectorSurfaceFlux.hh"

int main(int argc, char** argv)
{
    std::string inputFilename = "../../geant/data/default/output.root";
    
    if (argc == 2) {
        inputFilename = std::string(argv[1]);
    }
    
    std::string outputFileName = inputFilename.substr(0, inputFilename.length() - 5).append("_flux.root");;
    
    TFile inFile(inputFilename.c_str());
    SetupDataStructs(&inFile);
    
    // Create per particle fluxes of each surface and totals
    std::unordered_map<std::string, std::vector<std::pair<double, double>>> total;
    std::unordered_map<std::string, std::vector<std::pair<double, double>>> negx;
    std::unordered_map<std::string, std::vector<std::pair<double, double>>> negy;
    std::unordered_map<std::string, std::vector<std::pair<double, double>>> negz;
    std::unordered_map<std::string, std::vector<std::pair<double, double>>> posx;
    std::unordered_map<std::string, std::vector<std::pair<double, double>>> posy;
    std::unordered_map<std::string, std::vector<std::pair<double, double>>> posz;
    
    //TotalSurfaceEnteringFlux(&total);
    PerSurfaceEnteringFlux(&posx, &negx, &posy, &negy, &posz, &negz);
    
    TFile outFile = TFile(outputFileName.c_str(), "RECREATE");
    outFile.cd();
    
    double coord1;
    double coord2;
    char name[12];
    
    // This will be a ragged tree
    TTree* tree = new TTree("Entering", "Title");
    tree->Branch("Name", &name, "name[12]/C");
    tree->Branch("+x_y", &coord1, "+x_y/D");
    tree->Branch("+x_z", &coord2, "+x_z/D");
    tree->Branch("-x_y", &coord1, "-x_y/D");
    tree->Branch("-x_z", &coord2, "-x_z/D");
    tree->Branch("+y_x", &coord1, "+y_x/D");
    tree->Branch("+y_z", &coord2, "+y_z/D");
    tree->Branch("-y_x", &coord1, "-y_x/D");
    tree->Branch("-y_z", &coord2, "-y_z/D");
    tree->Branch("+z_x", &coord1, "+z_x/D");
    tree->Branch("+z_y", &coord2, "+z_y/D");
    tree->Branch("-z_x", &coord1, "-z_x/D");
    tree->Branch("-z_y", &coord2, "-z_y/D");
    
    // Turn all branches off, only enable when you need them for writing
    tree->SetBranchStatus("*", false);
    tree->SetBranchStatus("Name", true);
    
    // for (const std::pair<std::string, std::vector<std::pair<double, double>>>& pair : total) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    tree->SetBranchStatus("+x_y", true);
    tree->SetBranchStatus("+x_z", true);
    for (const std::pair<std::string, std::vector<std::pair<double, double>>>& pair : posx) {
        strcpy(name, pair.first.c_str());
        
        for (const std::pair<double, double>& coords : pair.second) {
            coord1 = coords.first;
            coord2 = coords.second;
            tree->Fill();
        }
    }
    tree->SetBranchStatus("+x_y", false);
    tree->SetBranchStatus("+x_z", false);
    
    // strcpy(side, "Negx");
    // for (const std::pair<std::string, int>& pair : negx) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // strcpy(side, "Posy");
    // for (const std::pair<std::string, int>& pair : posy) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // strcpy(side, "Negy");
    // for (const std::pair<std::string, int>& pair : negy) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // strcpy(side, "Posz");
    // for (const std::pair<std::string, int>& pair : posz) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // strcpy(side, "Negz");
    // for (const std::pair<std::string, int>& pair : negz) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // tree->Write();
    // tree->Delete();
    
    // inFile.cd();
    
    // //TotalSurfaceLeavingFlux(&total);
    // PerSurfaceLeavingFlux(&posx, &negx, &posy, &negy, &posz, &negz);
    
    // inFile.Close();
    // outFile.cd();
    
    // tree = new TTree("Leaving", "Title");
    // tree->Branch("Side", &side, "side[6]/C");
    // tree->Branch("Name", &name, "name[12]/C");
    // tree->Branch("Count", &count, "Count/I");
    
    // strcpy(side, "Total");
    // for (const std::pair<std::string, int>& pair : total) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // strcpy(side, "Posx");
    // for (const std::pair<std::string, int>& pair : posx) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // strcpy(side, "Negx");
    // for (const std::pair<std::string, int>& pair : negx) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // strcpy(side, "Posy");
    // for (const std::pair<std::string, int>& pair : posy) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // strcpy(side, "Negy");
    // for (const std::pair<std::string, int>& pair : negy) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // strcpy(side, "Posz");
    // for (const std::pair<std::string, int>& pair : posz) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // strcpy(side, "Negz");
    // for (const std::pair<std::string, int>& pair : negz) {
    //     strcpy(name, pair.first.c_str());
    //     count = pair.second;
        
    //     tree->Fill();
    // }
    
    // tree->Write();
    // tree->Delete();

    outFile.Close();
}