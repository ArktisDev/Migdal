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

// outputData is a lookup table per particle name, to a lookup table per detector side, to a list of chosen data per step into detector
void PerParticleEnteringFlux(std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::vector<float>>>>* outputData) 
{
    outputData->clear();
    
    EnteringData data;
    MetaData metadata;
    metadata.ReadEntry(0);
    
    for (int i = 0; i < data.entries; i++) {
        data.ReadEntry(i);
        
        std::string particleName = data.particleName;
        
        // If the particle doesn't exist in map yet, initialize it
        if (outputData->count(particleName) == 0) {
            (*outputData)[particleName] = std::unordered_map<std::string, std::vector<std::vector<float>>>();
            (*outputData)[particleName]["posx"] = std::vector<std::vector<float>>();
            (*outputData)[particleName]["posy"] = std::vector<std::vector<float>>();
            (*outputData)[particleName]["posz"] = std::vector<std::vector<float>>();
            (*outputData)[particleName]["negx"] = std::vector<std::vector<float>>();
            (*outputData)[particleName]["negy"] = std::vector<std::vector<float>>();
            (*outputData)[particleName]["negz"] = std::vector<std::vector<float>>();
        }
        
        std::string side;
        std::vector<float> eventData = std::vector<float>();
        
        // Classify which side the particle is from, if fail to classify -> increase tolerance level until you find some side
        double tol = 1e-8;
        while(true) {
            // Try to find a surface within tolerance
            if        (abs(data.x - metadata.detector_hx) < tol) {
                side = "posx";
                eventData.emplace_back(data.y);
                eventData.emplace_back(data.z);
                break;
            } else if (abs(data.x + metadata.detector_hx) < tol) {
                side = "negx";
                eventData.emplace_back(data.y);
                eventData.emplace_back(data.z);
                break;
            } else if (abs(data.y - metadata.detector_hy) < tol) {
                side = "posy";
                eventData.emplace_back(data.x);
                eventData.emplace_back(data.z);
                break;
            } else if (abs(data.y + metadata.detector_hy) < tol) {
                side = "negy";
                eventData.emplace_back(data.x);
                eventData.emplace_back(data.z);
                break;
            } else if (abs(data.z - metadata.detector_hz) < tol) {
                side = "posz";
                eventData.emplace_back(data.x);
                eventData.emplace_back(data.y);
                break;
            } else if (abs(data.z + metadata.detector_hz) < tol) {
                side = "negz";
                eventData.emplace_back(data.x);
                eventData.emplace_back(data.y);
                break;
            }
            tol *= 10;
        }
        eventData.emplace_back(data.energy);
        
        // Add data to list of data for particle for side and then go to next particle
        (*outputData)[particleName][side].emplace_back(eventData);
    }
}

// outputData is a lookup table per particle name, to a lookup table per detector side, to a list of chosen data per step into detector
void PerParticleLeavingFlux(std::unordered_map<std::string, std::unordered_map<std::string, std::vector<std::vector<float>>>>* outputData) 
{
    outputData->clear();
    
    LeavingData data;
    MetaData metadata;
    metadata.ReadEntry(0);
    
    for (int i = 0; i < data.entries; i++) {
        data.ReadEntry(i);
        
        std::string particleName = data.particleName;
        
        // If the particle doesn't exist in map yet, initialize it
        if (outputData->count(particleName) == 0) {
            (*outputData)[particleName] = std::unordered_map<std::string, std::vector<std::vector<float>>>();
            (*outputData)[particleName]["posx"] = std::vector<std::vector<float>>();
            (*outputData)[particleName]["posy"] = std::vector<std::vector<float>>();
            (*outputData)[particleName]["posz"] = std::vector<std::vector<float>>();
            (*outputData)[particleName]["negx"] = std::vector<std::vector<float>>();
            (*outputData)[particleName]["negy"] = std::vector<std::vector<float>>();
            (*outputData)[particleName]["negz"] = std::vector<std::vector<float>>();
        }
        
        std::string side;
        std::vector<float> eventData = std::vector<float>();
        
        // Classify which side the particle is from, if fail to classify -> increase tolerance level until you find some side
        double tol = 1e-8;
        while(true) {
            // Try to find a surface within tolerance
            if        (abs(data.x - metadata.detector_hx) < tol) {
                side = "posx";
                eventData.emplace_back(data.y);
                eventData.emplace_back(data.z);
                break;
            } else if (abs(data.x + metadata.detector_hx) < tol) {
                side = "negx";
                eventData.emplace_back(data.y);
                eventData.emplace_back(data.z);
                break;
            } else if (abs(data.y - metadata.detector_hy) < tol) {
                side = "posy";
                eventData.emplace_back(data.x);
                eventData.emplace_back(data.z);
                break;
            } else if (abs(data.y + metadata.detector_hy) < tol) {
                side = "negy";
                eventData.emplace_back(data.x);
                eventData.emplace_back(data.z);
                break;
            } else if (abs(data.z - metadata.detector_hz) < tol) {
                side = "posz";
                eventData.emplace_back(data.x);
                eventData.emplace_back(data.y);
                break;
            } else if (abs(data.z + metadata.detector_hz) < tol) {
                side = "negz";
                eventData.emplace_back(data.x);
                eventData.emplace_back(data.y);
                break;
            }
            tol *= 10;
        }
        eventData.emplace_back(data.energy);
        
        // Add data to list of data for particle for side and then go to next particle
        (*outputData)[particleName][side].emplace_back(eventData);
    }
}

#endif // DETECTORSURFACECOUNTS_H
