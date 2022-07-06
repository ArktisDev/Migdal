#ifndef DETECTORSURFACEFLUX_H
#define DETECTORSURFACEFLUX_H

#include "DataStructs.hh"

#include <unordered_map>
#include <vector>
#include <string>

// std::unordered_map<std::string, std::vector<std::pair<double, double>>>*
// is a map of fluxes per particle. Flux is vector of coord position pairs

void PerSurfaceEnteringFlux(    std::unordered_map<std::string, std::vector<std::pair<double, double>>>* posx, 
                                std::unordered_map<std::string, std::vector<std::pair<double, double>>>* negx,
                                std::unordered_map<std::string, std::vector<std::pair<double, double>>>* posy, 
                                std::unordered_map<std::string, std::vector<std::pair<double, double>>>* negy,
                                std::unordered_map<std::string, std::vector<std::pair<double, double>>>* posz, 
                                std::unordered_map<std::string, std::vector<std::pair<double, double>>>* negz)  
{
    EnteringData data;
    MetaData metadata;
    metadata.ReadEntry(0);
    
    std::unordered_map<std::string, std::vector<std::pair<double, double>>>* side;
    
    for (int i = 0; i < data.entries; i++) {
        data.ReadEntry(i);
        
        // Classify which side the particle is from, if fail to classify -> increase tolerance level until you find some side
        double tol = 1e-8;
        while(true) {
            // Try to find a surface within tolerance
            if        (abs(data.x - metadata.detector_hx) < tol) {
                side = posx;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.y, data.z));
                
                break;
            } else if (abs(data.x + metadata.detector_hx) < tol) {
                side = negx;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.y, data.z));
                
                break;
            } else if (abs(data.y - metadata.detector_hy) < tol) {
                side = posy;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.x, data.z));
                
                break;
            } else if (abs(data.y + metadata.detector_hy) < tol) {
                side = negy;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.x, data.z));
                
                break;
            } else if (abs(data.z - metadata.detector_hz) < tol) {
                side = posz;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.x, data.y));
                
                break;
            } else if (abs(data.z + metadata.detector_hz) < tol) {
                side = negz;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.x, data.y));
                
                break;
            }
            tol *= 10;
        }
    }
}

void PerSurfaceLeavingFlux(    std::unordered_map<std::string, std::vector<std::pair<double, double>>>* posx, 
                                std::unordered_map<std::string, std::vector<std::pair<double, double>>>* negx,
                                std::unordered_map<std::string, std::vector<std::pair<double, double>>>* posy, 
                                std::unordered_map<std::string, std::vector<std::pair<double, double>>>* negy,
                                std::unordered_map<std::string, std::vector<std::pair<double, double>>>* posz, 
                                std::unordered_map<std::string, std::vector<std::pair<double, double>>>* negz)  
{
    LeavingData data;
    MetaData metadata;
    metadata.ReadEntry(0);
    
    std::unordered_map<std::string, std::vector<std::pair<double, double>>>* side;
    
    for (int i = 0; i < data.entries; i++) {
        data.ReadEntry(i);
        
        // Classify which side the particle is from, if fail to classify -> increase tolerance level until you find some side
        double tol = 1e-8;
        while(true) {
            // Try to find a surface within tolerance
            if        (abs(data.x - metadata.detector_hx) < tol) {
                side = posx;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.y, data.z));
                
                break;
            } else if (abs(data.x + metadata.detector_hx) < tol) {
                side = negx;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.y, data.z));
                
                break;
            } else if (abs(data.y - metadata.detector_hy) < tol) {
                side = posy;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.x, data.z));
                
                break;
            } else if (abs(data.y + metadata.detector_hy) < tol) {
                side = negy;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.x, data.z));
                
                break;
            } else if (abs(data.z - metadata.detector_hz) < tol) {
                side = posz;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.x, data.y));
                
                break;
            } else if (abs(data.z + metadata.detector_hz) < tol) {
                side = negz;
                
                if (side->count(data.particleName) == 0) {
                    (*side)[data.particleName] = std::vector<std::pair<double, double>>();
                }
                
                side->at(data.particleName).emplace_back(std::pair<double, double>(data.x, data.y));
                
                break;
            }
            tol *= 10;
        }
    }
}

#endif // DETECTORSURFACEFLUX_H
