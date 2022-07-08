#ifndef MACRO_H
#define MACRO_H

#include <iostream>
#include <fstream>

#include "ThreadPool.hpp"

std::string SetupMacro(std::string baseMacroFile, std::string outputDir, int collimatorRadius, int nParticles) {
    double inches_to_cm = 2.54;
    
    std::string runString = std::to_string(collimatorRadius / 4.) + "_" + std::to_string(nParticles);
    std::string macroFile = outputDir + "Setup_" + runString + ".mac";
    
    std::ifstream ifs(baseMacroFile);
    std::ofstream ofs(macroFile);
    
    std::string line;
    while (std::getline(ifs, line)) {
        ofs << line << "\n";
    }
    ifs.close();
    
    int polyThickness = 4;
    int leadThickness = 4;
    
    ofs << "/exp/addSourceShieldLayer " + std::to_string(polyThickness * inches_to_cm) + " BPoly " << std::to_string(collimatorRadius * inches_to_cm / 4) << " 50 0"  << "\n";
    ofs << "/exp/addSourceShieldLayer " + std::to_string(leadThickness * inches_to_cm) + " Pb " << std::to_string(collimatorRadius * inches_to_cm / 4) << " 50 0" << "\n";
    
    ofs << "/exp/setWindowRadius " + std::to_string(5 * inches_to_cm) << "\n";
    
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
    
    // Radius will be divided by 4 later
    for (int radius = 1; radius <= 20; radius += 1) {
        std::string macroFile = SetupMacro(baseMacroFile, outputDir, radius, nParticles);
        
        // run the macro with geant through tasking
        auto result = ThreadMgr.addTask(RunMacro, macroFile, migdalExeFile);
        result_vec.emplace_back(std::move(result));
        
        // So we get good random generation at least
        std::this_thread::sleep_for(std::chrono::seconds(1));
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

#endif // MACRO_H
