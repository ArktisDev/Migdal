#ifndef COINCIDENCES_H
#define COINCIDENCES_H

#include "TROOT.h"

#include "DataStructs.hh"

#include <string>

template<bool CanTellBetweenParticles, bool CanTellBetweenStartAndEndOfTrack, bool xAxis, bool yAxis, bool zAxis>
void CalculateCoincidences(TFile* inFile, const std::string& outFileName) {
    RecoilsData data;
    RecoilsData data2;
    
    double dist;
    
    TFile outFile = TFile(outFileName.c_str(), "RECREATE");
    
    TTree* outTree = new TTree("Pairwise Distances", "Title");
    outTree->Branch("Distance (mm)", &dist, "Distance (mm)/D");\
    
    
    // Iterate over all seen recoils
    for (int i = 0; i < data.entries; i++) {
        inFile->cd();
        data.ReadEntry(i);
        
        // Now iterate over all other recoils in the dataset
        for (int j = i + 1; j < data.entries; j++) {
            inFile->cd();
            data2.ReadEntry(j);
            
            // If you can't tell between particles, or you have different particles, calculate distance
            if (!CanTellBetweenParticles || strcmp(data2.particleName, data.particleName) != 0) {
                // Calculate distance between those two recoils
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

#endif // COINCIDENCES_H
