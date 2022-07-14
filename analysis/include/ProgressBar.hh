#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H

#include <iostream>
#include <string>

class ProgressBar {
  public:
    std::ostream& os;
    
    int barWidth;
    
    int maxProgress;
    int progress;
    
    std::string label;
    
    ProgressBar(int maxProgress, std::ostream& os = std::cout, int barWidth = 40) 
    : os(os), barWidth(barWidth), maxProgress(maxProgress), progress(0), label("Progress:")
    {
        
    }
    
    void UpdateProgress(int progress) {
        this->progress = progress;
    }
    
    void ShowBar() {
        int chars = barWidth * progress / maxProgress;
        int percent = 100 * progress / maxProgress;
        
        os << "\r" + label + " [";
        for (int i = 0; i < chars; i++) {
            os << "*";
        }
        for (int i = chars; i < barWidth; i++) {
            os << "-";
        }
        os << "] " << percent << "%" << std::flush;
    }
    
    void CompleteBar() {
        os << std::endl;
    }
};

#endif // PROGRESSBAR_H
