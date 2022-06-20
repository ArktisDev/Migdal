#include "TROOT.h"
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"

#include <iostream>
#include <string>

int main(int argc, char** argv)
{
    std::string filename = "../../build/output.root";
    
    if (argc == 2) {
        filename = std::string(argv[0]);
    }
    
    TFile tfile(filename.c_str());
    
    TTree* enteringTree;
    enteringTree = (TTree*) tfile.Get("Entering");
    
    TTree* leavingTree;
    leavingTree = (TTree*) tfile.Get("leaving");
    
    
}