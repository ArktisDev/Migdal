#ifndef COINCIDENCES_H
#define COINCIDENCES_H

#include "TROOT.h"

#include "DataStructs.hh"

#include <string>

// Calculate Coincidences with distinguish track start and distinguish particles
void CC_3D_TTrack_TParticle(TFile* inFile, const std::string& outFileName) {
    RecoilsData data;
    RecoilsData data2;
    
    double dist;
    
    TFile outFile = TFile(outFileName.c_str(), "RECREATE");
    
    TTree* outTree = new TTree("Pairwise Distances", "Title1");
    outTree->Branch("Distance (mm)", &dist, "Distance (mm)/D");
    
    
    // Iterate over all seen recoils
    for (int i = 0; i < data.entries; i++) {
        inFile->cd();
        data.ReadEntry(i);
        
        // Now iterate over all other recoils in the dataset
        for (int j = i + 1; j < data.entries; j++) {
            inFile->cd();
            data2.ReadEntry(j);
            
            // Calculate distance between those two recoils if they are different particles
            
            if (strcmp(data2.particleName, data.particleName) != 0) {
                dist = std::sqrt(std::pow(data.x - data2.x, 2) + std::pow(data.y - data2.y, 2) + std::pow(data.z - data2.z, 2));
                
                // Write distance to root tree
                outFile.cd();
                outTree->Fill();
            }
        }
    }
    
    // Finish writing tree to file
    outFile.cd();
    outTree->Write();
    
    outTree->Delete();
    
    outFile.Close();
}

// Calculate Coincidences with distinguish track start and distinguish particles
void CC_2D_TTrack_TParticle(TFile* inFile, const std::string& outFileName, bool xAxis, bool yAxis, bool zAxis) {
    RecoilsData data;
    RecoilsData data2;
    
    double dist;
    
    TFile outFile = TFile(outFileName.c_str(), "RECREATE");
    
    TTree* outTree = new TTree("Pairwise Distances", "Title2");
    outTree->Branch("Distance (mm)", &dist, "Distance (mm)/D");
    
    
    // Iterate over all seen recoils
    for (int i = 0; i < data.entries; i++) {
        inFile->cd();
        data.ReadEntry(i);
        
        // Now iterate over all other recoils in the dataset
        for (int j = i + 1; j < data.entries; j++) {
            inFile->cd();
            data2.ReadEntry(j);
            
            // Calculate distance between those two recoils if they are different particles
            
            if (strcmp(data2.particleName, data.particleName) != 0) {
                dist = 0;
                
                if (xAxis) dist += std::pow(data.x - data2.x, 2);
                if (yAxis) dist += std::pow(data.y - data2.y, 2);
                if (zAxis) dist += std::pow(data.z - data2.z, 2);
                dist = std::sqrt(dist);
                
                // Write distance to root tree
                outFile.cd();
                outTree->Fill();
            }
        }
    }
    
    // Finish writing tree to file
    outFile.cd();
    outTree->Write();
    
    outTree->Delete();
    
    outFile.Close();
}



// Calculate Coincidences with distinguish track start but not distinguish particles
void CC_3D_TTrack_FParticle(TFile* inFile, const std::string& outFileName) {
    RecoilsData data;
    RecoilsData data2;
    
    double dist;
    
    TFile outFile = TFile(outFileName.c_str(), "RECREATE");
    
    TTree* outTree = new TTree("Pairwise Distances", "Title3");
    outTree->Branch("Distance (mm)", &dist, "Distance (mm)/D");
    
    
    // Iterate over all seen recoils
    for (int i = 0; i < data.entries; i++) {
        inFile->cd();
        data.ReadEntry(i);
        
        // Now iterate over all other recoils in the dataset
        for (int j = i + 1; j < data.entries; j++) {
            inFile->cd();
            data2.ReadEntry(j);
            
            // Calculate distance between those two recoils no matter what particles
            dist = std::sqrt(std::pow(data.x - data2.x, 2) + std::pow(data.y - data2.y, 2) + std::pow(data.z - data2.z, 2));
            
            // Write distance to root tree
            outFile.cd();
            outTree->Fill();
        }
    }
    
    // Finish writing tree to file
    outFile.cd();
    outTree->Write();
    
    outTree->Delete();
    
    outFile.Close();
}

// Calculate Coincidences with distinguish track start but not distinguish particles
void CC_2D_TTrack_FParticle(TFile* inFile, const std::string& outFileName, bool xAxis, bool yAxis, bool zAxis) {
    RecoilsData data;
    RecoilsData data2;
    
    double dist;
    
    TFile outFile = TFile(outFileName.c_str(), "RECREATE");
    
    TTree* outTree = new TTree("Pairwise Distances", "Title4");
    outTree->Branch("Distance (mm)", &dist, "Distance (mm)/D");\
    
    
    // Iterate over all seen recoils
    for (int i = 0; i < data.entries; i++) {
        inFile->cd();
        data.ReadEntry(i);
        
        // Now iterate over all other recoils in the dataset
        for (int j = i + 1; j < data.entries; j++) {
            inFile->cd();
            data2.ReadEntry(j);
            
            // Calculate distance between those two recoils no matter what particles
            dist = 0;
            
            if (xAxis) dist += std::pow(data.x - data2.x, 2);
            if (yAxis) dist += std::pow(data.y - data2.y, 2);
            if (zAxis) dist += std::pow(data.z - data2.z, 2);
            dist = std::sqrt(dist);
            
            // Write distance to root tree
            outFile.cd();
            outTree->Fill();
        }
    }
    
    // Finish writing tree to file
    outFile.cd();
    outTree->Write();
    
    outTree->Delete();
    
    outFile.Close();
}

#endif // COINCIDENCES_H
