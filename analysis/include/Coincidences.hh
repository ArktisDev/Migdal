#ifndef COINCIDENCES_H
#define COINCIDENCES_H

#include "TROOT.h"

#include "DataStructs.hh"
#include "ProgressBar.hh"

#include <string>
#include <iostream>
#include <random>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <thread>
#include <chrono>

// From https://www.gormanalysis.com/blog/random-numbers-in-cpp/
std::vector<int> SampleWithoutReplacement(int k, int N){
    // Sample k elements from the range [1, N] without replacement
    // k should be <= N
    
    // Per thread random number generation with different seeds
    static thread_local std::mt19937 gen = std::mt19937(std::chrono::high_resolution_clock::now().time_since_epoch().count() + static_cast<long long>(std::hash<std::thread::id>()(std::this_thread::get_id())));
    
    // Create an unordered set to store the samples
    std::unordered_set<int> samples;
    
    // Sample and insert values into samples
    for (int r = N - k + 1; r < N + 1; ++r) {
        int v = std::uniform_int_distribution<int>(1, r)(gen);
        if (!samples.insert(v).second) samples.insert(r);
    }
    
    // Copy samples into vector
    std::vector<int> result(samples.begin(), samples.end());
    
    // Shuffle vector
    std::shuffle(result.begin(), result.end(), gen);
    
    return result;
};

void CalculateRecoilCoincidences(TFile* inFile, const std::string& outFileName, 
    int iterations, int samplesPerIteration, int nEvents, double minEnergy = 2) 
{
    RecoilsData data;
    RecoilsData data2;
    
    TFile outFile = TFile(outFileName.c_str(), "RECREATE");
    
    // This uses a lot of RAM, but it means random access is fast
    // Without this ROOT will read from disk for every data it accesses - Lars
    // In a file with 600k entries I am seeing a factor of 1000x speedup with
    // accessing data from RAM
    data.tree->LoadBaskets();
    
    int iter = 0;
    double dist = -1;
    char particle1[12] = "NULL";
    char particle2[12] = "NULL";
    double energy1 = -1;
    double energy2 = -1;
    
    TTree* outTree = new TTree("Pairwise Distances", "Title");
    outTree->Branch("Iteration", &iter, "Iteration/I");
    outTree->Branch("Distance (mm)", &dist, "Distance (mm)/D");
    outTree->Branch("Particle 1", &particle1, "Particle 1[12]/C");
    outTree->Branch("Particle 2", &particle2, "Particle 2[12]/C");
    outTree->Branch("Energy 1 (keV)", &energy1, "Energy 1 (keV)/D");
    outTree->Branch("Energy 2 (keV)", &energy2, "Energy 2 (keV)/D");
    
    inFile->cd();
    
    // To know which rows to read for a particular recoil event
    std::unordered_map<int, std::vector<int>> recoilRows;
    
    for (int i = 0; i < data.entries; i++) {
        data.ReadEntry(i);
        
        if (recoilRows.count(data.eventID) == 0) recoilRows[data.eventID] = std::vector<int>();
        recoilRows[data.eventID].emplace_back(i);
    }
    
    ProgressBar progressBar(iterations);
    std::cout << "Will do " << iterations << " iterations" << std::endl;
    
    for (; iter < iterations; iter++) {
        progressBar.UpdateProgress(iter);
        progressBar.ShowBar();
        
        std::vector<int> events = SampleWithoutReplacement(samplesPerIteration, nEvents);
        
        for (int i = 0; i < events.size(); i++) {
            int event1 = events[i];
            if (recoilRows.count(event1) == 0) continue;
            
            for (int j = i + 1; j < events.size(); j++) {
                int event2 = events[j];
                if (recoilRows.count(event2) == 0) continue;
                
                // Now we know both event i and j had a recoil, analyze all pairs of recoils for that event
                
                for (int k = 0; k < recoilRows[event1].size(); k++) {
                    for (int l = 0; l < recoilRows[event2].size(); l++) {
                        inFile->cd();
                        
                        Timer timer;
                        timer.start();
                        data.ReadEntry(recoilRows[event1][k]);
                        data2.ReadEntry(recoilRows[event2][l]);
                        
                        // Discard events with same particle types
                        if (strcmp(data2.particleName, data.particleName) == 0) continue;
                        
                        // Discard events with below threshold energy
                        if (data.energy < minEnergy) continue;
                        if (data2.energy < minEnergy) continue;
                        
                        strcpy(particle1, data.particleName);
                        strcpy(particle2, data2.particleName);
                        energy1 = data.energy;
                        energy2 = data2.energy;
                        
                        // Recalculate distance for this event
                        dist = 0;
                        dist += std::pow(data.x - data2.x, 2);
                        dist += std::pow(data.y - data2.y, 2);
                        dist += std::pow(data.z - data2.z, 2);
                        dist = std::sqrt(dist);
                        
                        // Write to file the distance
                        outFile.cd();
                        outTree->Fill();
                    }
                }
            }
        }
    }
    
    progressBar.UpdateProgress(iter);
    progressBar.ShowBar();
    progressBar.CompleteBar();
    
    outFile.cd();
    outTree->Write();
    outTree->Delete();
    
    outFile.Write();
    outFile.Close();
}

template<bool forceAppropriateRecoil, bool distXAxis, bool distYAxis, bool distZAxis>
void CalculateEdepRecoilCoincidences(TFile* inFile, const std::string& outFileName, 
    int iterations, int samplesPerIteration, int nEvents, double minEnergy = 2) 
{
    RecoilsData recoildata;
    EdepData edepdata;
    
    TFile outFile = TFile(outFileName.c_str(), "RECREATE");
    
    // This uses a lot of RAM, but it means random access is fast
    // Without this ROOT will read from disk for every data it accesses - Lars
    // In a file with 600k entries I am seeing a factor of 1000x speedup with
    // accessing data from RAM
    recoildata.tree->LoadBaskets();
    edepdata.tree->LoadBaskets();
    
    int iter = 0;
    double dist = -1;
    char particle1[12] = "NULL";
    char particle2[12] = "NULL";
    double energy = -1;
    double edep = -1;
    
    TTree* outTree = new TTree("Pairwise Distances", "Title");
    outTree->Branch("Iteration", &iter, "Iteration/I");
    outTree->Branch("Distance (mm)", &dist, "Distance (mm)/D");
    outTree->Branch("Particle 1", &particle1, "Particle 1[12]/C");
    outTree->Branch("Particle 2", &particle2, "Particle 2[12]/C");
    outTree->Branch("Energy 1 (keV)", &energy, "Energy 1 (keV)/D");
    outTree->Branch("Edep 2 (keV)", &edep, "Energy 2 (keV)/D");
    
    inFile->cd();
    
    // To know which rows to read for a particular recoil event
    std::unordered_map<int, std::vector<int>> recoilRows;
    // Same but for edeps
    std::unordered_map<int, std::vector<int>> edepRows;
    
    for (int i = 0; i < recoildata.entries; i++) {
        recoildata.ReadEntry(i);
        
        // Add an entry for this event if not there yet
        if (recoilRows.count(recoildata.eventID) == 0) recoilRows[recoildata.eventID] = std::vector<int>();
        
        // Add this current row in the data to belonging to this event
        recoilRows[recoildata.eventID].emplace_back(i);
    }
    
    for (int i = 0; i < edepdata.entries; i++) {
        edepdata.ReadEntry(i);
        
        if (edepRows.count(edepdata.eventID) == 0) edepRows[edepdata.eventID] = std::vector<int>();
        edepRows[edepdata.eventID].emplace_back(i);
    }
    
    ProgressBar progressBar(iterations);
    std::cout << "Will do " << iterations << " iterations" << std::endl;
    
    for (; iter < iterations; iter++) {
        progressBar.UpdateProgress(iter);
        progressBar.ShowBar();
        std::vector<int> events = SampleWithoutReplacement(samplesPerIteration, nEvents);
        
        for (int i = 0; i < events.size(); i++) {
            int event1 = events[i];
            if (recoilRows.count(event1) == 0) continue;
            
            for (int j = 0; j < events.size(); j++) {
                int event2 = events[j];
                if (edepRows.count(event2) == 0) continue;
                
                // Now we know both event i and j had something, now analyze all pairs of recoils and edeps for that event
                
                for (int k = 0; k < recoilRows[event1].size(); k++) {
                    for (int l = 0; l < edepRows[event2].size(); l++) {
                        inFile->cd();
                
                        recoildata.ReadEntry(recoilRows[event1][k]);
                        edepdata.ReadEntry(edepRows[event2][l]);
                        
                        // Discard events with same particle types
                        if (strcmp(recoildata.particleName, edepdata.particleName) == 0) continue;

                        if constexpr (forceAppropriateRecoil) {
                            // strstr returns nullptr if not found
                            if (strstr(recoildata.particleName, "e")) continue;
                        }
                        
                        // Discard events with below threshold energy
                        if (recoildata.energy < minEnergy) continue;
                        if (edepdata.energy < minEnergy) continue;
                        
                        strcpy(particle1, recoildata.particleName);
                        strcpy(particle2, edepdata.particleName);
                        energy = recoildata.energy;
                        edep = edepdata.edep;
                        
                        // Recalculate distance for this event
                        dist = 0;
                        if constexpr (distXAxis) dist += std::pow(recoildata.x - edepdata.x, 2);
                        if constexpr (distYAxis)dist += std::pow(recoildata.y - edepdata.y, 2);
                        if constexpr (distZAxis)dist += std::pow(recoildata.z - edepdata.z, 2);
                        dist = std::sqrt(dist);
                        
                        // Write to file the distance
                        outFile.cd();
                        outTree->Fill();
                    }
                }
            }
        }
    }
    
    progressBar.UpdateProgress(iter);
    progressBar.ShowBar();
    progressBar.CompleteBar();
    
    outFile.cd();
    outTree->Write();
    outTree->Delete();
    
    outFile.Write();
    outFile.Close();
}

#endif // COINCIDENCES_H
