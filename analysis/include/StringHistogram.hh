#ifndef STRINGHISTOGRAM_H
#define STRINGHISTOGRAM_H

#include "TROOT.h"
#include "TH1D.h"

#include <unordered_map>

TH1D* CreateStringHistogram(std::unordered_map<std::string, int> counts, std::string name, std::string title) {
    // This is number of keys in the map
    int nx = counts.size();
    
    TH1D* h = new TH1D(name.c_str(), title.c_str(), nx, 0, nx);
    
    int i = 0;
    for (const std::pair<std::string, int>& pair : counts) {
        // Bins start at 1
        h->GetXaxis()->SetBinLabel(i + 1, pair.first.c_str());
        h->Fill(i++, pair.second);
    }
    
    return h;
}

#endif // STRINGHISTOGRAM_H
