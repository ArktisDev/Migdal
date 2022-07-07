#ifndef SOURCESHIELDING_H
#define SOURCESHIELDING_H

#include <iostream>
#include <fstream>

#include "ThreadPool.hpp"

std::string SetupMacro(std::string baseMacroFile, std::string outputDir, int leadThickness, int polyThickness, int nParticles) {
    double inches_to_cm = 2.54;
    
    std::string runString = std::to_string(leadThickness) + "_" + std::to_string(polyThickness) + "_" + std::to_string(nParticles);
    std::string macroFile = outputDir + "Setup_" + runString + ".mac";
    
    std::ifstream ifs(baseMacroFile);
    std::ofstream ofs(macroFile);
    
    std::string line;
    while (std::getline(ifs, line)) {
        ofs << line << "\n";
    }
    ifs.close();
    
    ofs << "/exp/addSourceShieldLayer " + std::to_string(polyThickness * inches_to_cm) + " BPoly 2.5 50 0"  << "\n";
    ofs << "/exp/addSourceShieldLayer " + std::to_string(leadThickness * inches_to_cm) + " Pb 2.5 50 0" << "\n";
    
    ofs << "/file/setFileName " + outputDir + "output_" + runString + ".root" << "\n";
    ofs << "/file/setMaterialLogFilename " + outputDir + "materials.log" << "\n";
    
    ofs << "/run/initialize" << "\n";
    ofs << "/run/setCut 1 mm" << "\n";
    ofs << "/run/setCutForRegion DetectorRegion 0.01 mm" << "\n";
    ofs << "/run/setCutForRegion ShieldRegion 2 mm" << "\n";
    
    ofs << "/run/beamOn " << std::to_string(nParticles) << "\n";

    ofs.flush();
    
    ofs.close();
    
    return macroFile;
}

int RunMacro(std::string macroFile, std::string migdalExeFile) {
    system((migdalExeFile + " " + macroFile).c_str());
    
    return 0;
}

void RunSourceShielding(std::string baseMacroFile, std::string outputDir, std::string migdalExeFile, int nParticles) {
    ThreadPool::ThreadPoolMgr ThreadMgr;
    
    ThreadMgr.initialize(6);
    
    std::vector<std::unique_ptr<ThreadPool::ThreadResult<int>>> result_vec;
    
    for (int lead = 2; lead <= 10; lead += 2) {
        for (int poly = 2; poly <= 10; poly += 2) {
            std::string macroFile = SetupMacro(baseMacroFile, outputDir, lead, poly, nParticles);
            
            // run the macro with geant through tasking
            auto result = ThreadMgr.addTask(RunMacro, macroFile, migdalExeFile);
            result_vec.emplace_back(std::move(result));
            
            // So we get good random generation at least
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    
    // Wait for all threads to finish by retrieving results
    try {
        for (auto& v : result_vec) {
            v->get();
        }
    }
    catch (std::exception& e) {
        std::cout << e.what() << std::endl;
    }
}

#endif // SOURCESHIELDING_H
