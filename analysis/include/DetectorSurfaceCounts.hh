#ifndef DETECTORSURFACECOUNTS_H
#define DETECTORSURFACECOUNTS_H

#include "TROOT.h"
#include "TH1D.h"
#include "TPad.h"
#include "TCanvas.h"

#include "DataStructs.hh"
#include "StringHistogram.hh"

#include <unordered_map>
#include <string>

// This function expects you to have initialized already the datastructs, otherwise the output is just garbage
void TotalSurfaceCounts(std::unordered_map<std::string, int>* enteringCounts, std::unordered_map<std::string, int>* leavingCounts) {
    if (enteringCounts) {
        enteringCounts->clear();
        EnteringData data;

        for (int i = 0; i < data.entries; i++) {
            data.ReadEntry(i);
            
            if (enteringCounts->count(data.particleName) == 0) {
                (*enteringCounts)[data.particleName] = 1;
            } else {
                enteringCounts->at(data.particleName)++;
            }
        }
    }
    
    if (leavingCounts) {
        leavingCounts->clear();
        LeavingData data;
        
        for (int i = 0; i < data.entries; i++) {
            data.ReadEntry(i);
            
            if (leavingCounts->count(data.particleName) == 0) {
                (*leavingCounts)[data.particleName] = 1;
            } else {
                leavingCounts->at(data.particleName)++;
            }
        }
    }
}

#endif // DETECTORSURFACECOUNTS_H
