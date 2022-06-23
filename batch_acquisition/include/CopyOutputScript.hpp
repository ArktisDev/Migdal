#ifndef COPYOUTPUTSCRIPT_H
#define COPYOUTPUTSCRIPT_H

#include <iostream>
#include <fstream>
#include <string>

void CopyOutputScript(std::string outputDir, std::string scriptFileName, std::string scriptCopyName) {
    std::ifstream ifs(scriptFileName);
    std::ofstream ofs(outputDir + scriptCopyName);
    
    std::string line;
    while(std::getline(ifs, line)) {
        ofs << line << "\n";
    }
    
    ofs.flush();
    
    ifs.close();
    ofs.close();
}

#endif // COPYOUTPUTSCRIPT_H
