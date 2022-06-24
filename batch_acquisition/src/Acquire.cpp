#include <iostream>

#include "ThreadPool.hpp"

#include "CopyOutputScript.hpp"


#include "SourceShielding.hpp"

int main() {
    std::string migdalExeFile = "../../geant/build/Migdal";
    
    std::string outputDir = "../../geant/data/test/";
    std::string script = "../include/SourceShielding.hpp";
    std::string scriptCopyName = "SourceShielding.hpp";
    
    std::string baseMacroFile = "../../geant/data/test/Setup.mac";
    
    int nParticles = 100000;
    
    CopyOutputScript(outputDir, script, scriptCopyName);
    
    
    RunSourceShielding(baseMacroFile, outputDir, migdalExeFile, nParticles);
}