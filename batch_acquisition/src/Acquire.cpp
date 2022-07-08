#include <iostream>

#include "ThreadPool.hpp"

#include "CopyOutputScript.hpp"


#include "Macro.hpp"

int main() {
    std::string migdalExeFile = "../../geant/build/Migdal";
    
    std::string outputDir = "../../geant/data/collimator1/";
    std::string script = "../include/Macro.hpp";
    std::string scriptCopyName = "Macro.hpp";
    
    std::string baseMacroFile = "../../geant/data/collimator1/Basemac";
    
    int nParticles = 100000;
    
    CopyOutputScript(outputDir, script, scriptCopyName);
    
    
    RunSourceShielding(baseMacroFile, outputDir, migdalExeFile, nParticles);
}