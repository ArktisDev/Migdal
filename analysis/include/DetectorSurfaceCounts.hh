#ifndef DETECTORSURFACECOUNTS_H
#define DETECTORSURFACECOUNTS_H

#include "TROOT.h"

#include "DataStructs.hh"

#include <unordered_map>
#include <string>
#include <cmath>

// These functions expect you to have initialized already the datastructs for reading data from the root file, otherwise the output is just garbage


void TotalSurfaceEnteringCounts(std::unordered_map<std::string, int>* enteringCounts) {
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

void TotalSurfaceLeavingCounts(std::unordered_map<std::string, int>* leavingCounts) {
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

void PerSurfaceEnteringCounts(  std::unordered_map<std::string, int>* posx, std::unordered_map<std::string, int>* negx,
                                std::unordered_map<std::string, int>* posy, std::unordered_map<std::string, int>* negy,
                                std::unordered_map<std::string, int>* posz, std::unordered_map<std::string, int>* negz) 
{
    posx->clear();
    posy->clear();
    posz->clear();
    negx->clear();
    negy->clear();
    negz->clear();
    EnteringData data;
    MetaData metadata; 
    metadata.ReadEntry(0);
    
    std::unordered_map<std::string, int>* side;
    
    for (int i = 0; i < data.entries; i++) {
        data.ReadEntry(i);
        
        // Classify which side the particle is from, if fail to classify -> increase tolerance level until you find some side
        double tol = 1e-8;
        while(true) {
            // Try to find a surface within tolerance
            if        (abs(data.x - metadata.detector_hx) < tol) {
                side = posx;
                break;
            } else if (abs(data.x + metadata.detector_hx) < tol) {
                side = negx;
                break;
            } else if (abs(data.y - metadata.detector_hy) < tol) {
                side = posy;
                break;
            } else if (abs(data.y + metadata.detector_hy) < tol) {
                side = negy;
                break;
            } else if (abs(data.z - metadata.detector_hz) < tol) {
                side = posz;
                break;
            } else if (abs(data.z + metadata.detector_hz) < tol) {
                side = negz;
                break;
            }
            tol *= 10;
        }
        
        if (side->count(data.particleName) == 0) {
            (*side)[data.particleName] = 1;
        } else {
            side->at(data.particleName)++;
        }
    }
}

void PerSurfaceLeavingCounts(   std::unordered_map<std::string, int>* posx, std::unordered_map<std::string, int>* negx,
                                std::unordered_map<std::string, int>* posy, std::unordered_map<std::string, int>* negy,
                                std::unordered_map<std::string, int>* posz, std::unordered_map<std::string, int>* negz) 
{
    posx->clear();
    posy->clear();
    posz->clear();
    negx->clear();
    negy->clear();
    negz->clear();
    LeavingData data;
    MetaData metadata;
    metadata.ReadEntry(0);
    
    std::unordered_map<std::string, int>* side;
    
    for (int i = 0; i < data.entries; i++) {
        data.ReadEntry(i);
        
        // Classify which side the particle is from, if fail to classify -> increase tolerance level until you find some side
        double tol = 1e-8;
        while(true) {
            // Try to find a surface within tolerance
            if        (abs(data.x - metadata.detector_hx) < tol) {
                side = posx;
                break;
            } else if (abs(data.x + metadata.detector_hx) < tol) {
                side = negx;
                break;
            } else if (abs(data.y - metadata.detector_hy) < tol) {
                side = posy;
                break;
            } else if (abs(data.y + metadata.detector_hy) < tol) {
                side = negy;
                break;
            } else if (abs(data.z - metadata.detector_hz) < tol) {
                side = posz;
                break;
            } else if (abs(data.z + metadata.detector_hz) < tol) {
                side = negz;
                break;
            }
            tol *= 10;
        }
        
        if (side->count(data.particleName) == 0) {
            (*side)[data.particleName] = 1;
        } else {
            side->at(data.particleName)++;
        }
    }
}

#endif // DETECTORSURFACECOUNTS_H
