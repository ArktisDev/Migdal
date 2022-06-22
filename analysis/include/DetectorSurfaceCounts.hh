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

void DrawTotalCountsHist() {
    std::unordered_map<std::string, int> enteringCounts;
    std::unordered_map<std::string, int> leavingCounts;
    TotalSurfaceCounts(&enteringCounts, &leavingCounts);
    
    {
        TCanvas *c = new TCanvas();
        c->cd(1);
            
        c->SetLogy(true);
        c->SetGrid(true);
            
        TH1D* h = CreateStringHistogram(enteringCounts, "Entering Counts", "Entering Counts");
        h->SetTitle("Entering Counts; Particle; Counts");
        h->GetXaxis()->CenterTitle(true);
        h->GetYaxis()->CenterTitle(true);
        h->SetMinimum(0.7);
        h->SetStats(false);
        h->SetFillColor(kBlue);
        h->SetBarOffset(0.1);
        h->SetBarWidth(0.8);
        h->Draw("hist bar");
        
        c->Print("../total_entering_counts.pdf");
    }
    
    {
        TCanvas *c = new TCanvas();
        c->cd(1);
        
        c->SetLogy(true);
        c->SetGrid(true);
        
        TH1D* h = CreateStringHistogram(leavingCounts, "Leaving Counts", "Leaving Counts");
        h->SetTitle("Leaving Counts; Particle; Counts");
        h->GetXaxis()->CenterTitle(true);
        h->GetYaxis()->CenterTitle(true);
        h->SetMinimum(0.7);
        h->SetStats(false);
        h->SetFillColor(kGreen);
        h->SetBarOffset(0.1);
        h->SetBarWidth(0.8);
        h->Draw("hist bar");
        
        c->Print("../total_exiting_counts.pdf");
    }
}

#endif // DETECTORSURFACECOUNTS_H
