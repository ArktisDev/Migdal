#ifndef DATASTRUCTS_H
#define DATASTRUCTS_H

#include "TROOT.h"
#include "TTree.h"
#include "TFile.h"

#include <iostream>

struct EnteringData {
    // For the data each struct object holds
    int eventID;
    int trackID;
    int parentID;
    double time;
    char particleName[12];
    double energy;
    double x;
    double y;
    double z;
    
    inline static TTree* tree = nullptr;
    inline static int entries = -1;
    
    // For the data just read off the tree
    inline static int feventID = -1;
    inline static int ftrackID = -1;
    inline static int fparentID = -1;
    inline static double ftime = -1;
    inline static char fparticleName[12] = "";
    inline static double fenergy = -1;
    inline static double fx = -1;
    inline static double fy = -1;
    inline static double fz = -1;
    
    EnteringData(const EnteringData& enteringData) {
        this->eventID = enteringData.eventID;
        this->trackID = enteringData.trackID;
        this->parentID = enteringData.parentID;
        this->time = enteringData.time;
        strcpy(this->particleName, enteringData.particleName);
        this->energy = enteringData.energy;
        this->x = enteringData.x;
        this->y = enteringData.y;
        this->z = enteringData.z;
    }
    
    EnteringData() {
        
    }
    
    static void SetupTree(TTree* tree) {
        EnteringData::tree = tree;
        EnteringData::entries = tree->GetEntries();
        tree->SetBranchAddress("Event ID", &(EnteringData::feventID));
        tree->SetBranchAddress("Track ID", &(EnteringData::ftrackID));
        tree->SetBranchAddress("Parent ID", &(EnteringData::fparentID));
        tree->SetBranchAddress("Global Time (ns)", &(EnteringData::ftime));
        tree->SetBranchAddress("Particle Name", &(EnteringData::fparticleName));
        tree->SetBranchAddress("Energy (keV)", &(EnteringData::fenergy));
        tree->SetBranchAddress("Position x (mm)", &(EnteringData::fx));
        tree->SetBranchAddress("Position y (mm)", &(EnteringData::fy));
        tree->SetBranchAddress("Position z (mm)", &(EnteringData::fz));
    }
    
    void ReadEntry(Long64_t entry) {
        if (tree) this->tree->GetEntry(entry);
        
        this->eventID = feventID;
        this->trackID = ftrackID;
        this->parentID = fparentID;
        this->time = ftime;
        strcpy(this->particleName, fparticleName);
        this->energy = fenergy;
        this->x = fx;
        this->y = fy;
        this->z = fz;
    }
};

std::ostream& operator<<(std::ostream& os, const EnteringData& enteringData) {
    os << "Tree Pointer: " << enteringData.tree << "\n";
    os << "Event ID: " << enteringData.eventID << "\n";
    os << "Track ID: " << enteringData.trackID << "\n";
    os << "Parent ID: " << enteringData.parentID << "\n";
    os << "Time (ns): " << enteringData.time << "\n";
    os << "Particle Name: " << enteringData.particleName << "\n";
    os << "Energy (keV): " << enteringData.energy << "\n";
    os << "x (mm): " << enteringData.x << "\n";
    os << "y (mm): " << enteringData.y << "\n";
    os << "z (mm): " << enteringData.z;
    return os;
}





struct LeavingData {
    // For the data each struct object holds
    int eventID;
    int trackID;
    int parentID;
    double time;
    char particleName[12];
    double energy;
    double x;
    double y;
    double z;
    
    inline static TTree* tree = nullptr;
    inline static int entries = -1;
    
    // For the data just read off the tree
    inline static int feventID = -1;
    inline static int ftrackID = -1;
    inline static int fparentID = -1;
    inline static double ftime = -1;
    inline static char fparticleName[12] = "";
    inline static double fenergy = -1;
    inline static double fx = -1;
    inline static double fy = -1;
    inline static double fz = -1;
    
    LeavingData(const LeavingData& leavingData) {
        this->eventID = leavingData.eventID;
        this->trackID = leavingData.trackID;
        this->parentID = leavingData.parentID;
        this->time = leavingData.time;
        strcpy(this->particleName, leavingData.particleName);
        this->energy = leavingData.energy;
        this->x = leavingData.x;
        this->y = leavingData.y;
        this->z = leavingData.z;
    }
    
    LeavingData() {
        
    }
    
    static void SetupTree(TTree* tree) {
        LeavingData::tree = tree;
        LeavingData::entries = tree->GetEntries();
        tree->SetBranchAddress("Event ID", &(LeavingData::feventID));
        tree->SetBranchAddress("Track ID", &(LeavingData::ftrackID));
        tree->SetBranchAddress("Parent ID", &(LeavingData::fparentID));
        tree->SetBranchAddress("Global Time (ns)", &(LeavingData::ftime));
        tree->SetBranchAddress("Particle Name", &(LeavingData::fparticleName));
        tree->SetBranchAddress("Energy (keV)", &(LeavingData::fenergy));
        tree->SetBranchAddress("Position x (mm)", &(LeavingData::fx));
        tree->SetBranchAddress("Position y (mm)", &(LeavingData::fy));
        tree->SetBranchAddress("Position z (mm)", &(LeavingData::fz));
    }
    
    void ReadEntry(Long64_t entry) {
        if (tree) this->tree->GetEntry(entry);
        
        this->eventID = feventID;
        this->trackID = ftrackID;
        this->parentID = fparentID;
        this->time = ftime;
        strcpy(this->particleName, fparticleName);
        this->energy = fenergy;
        this->x = fx;
        this->y = fy;
        this->z = fz;
    }
};

std::ostream& operator<<(std::ostream& os, const LeavingData& leavingData) {
    os << "Tree Pointer: " << leavingData.tree << "\n";
    os << "Event ID: " << leavingData.eventID << "\n";
    os << "Track ID: " << leavingData.trackID << "\n";
    os << "Parent ID: " << leavingData.parentID << "\n";
    os << "Time (ns): " << leavingData.time << "\n";
    os << "Particle Name: " << leavingData.particleName << "\n";
    os << "Energy (keV): " << leavingData.energy << "\n";
    os << "x (mm): " << leavingData.x << "\n";
    os << "y (mm): " << leavingData.y << "\n";
    os << "z (mm): " << leavingData.z;
    return os;
}




struct RecoilsData {
    // For the data each struct object holds
    int eventID;
    int trackID;
    int parentID;
    double time;
    char particleName[12];
    double energy;
    double x;
    double y;
    double z;
    
    inline static TTree* tree = nullptr;
    inline static int entries = -1;
    
    // For the data just read off the tree
    inline static int feventID = -1;
    inline static int ftrackID = -1;
    inline static int fparentID = -1;
    inline static double ftime = -1;
    inline static char fparticleName[12] = "";
    inline static double fenergy = -1;
    inline static double fx = -1;
    inline static double fy = -1;
    inline static double fz = -1;
    
    RecoilsData(const RecoilsData& recoilsData) {
        this->eventID = recoilsData.eventID;
        this->trackID = recoilsData.trackID;
        this->parentID = recoilsData.parentID;
        this->time = recoilsData.time;
        strcpy(this->particleName, recoilsData.particleName);
        this->energy = recoilsData.energy;
        this->x = recoilsData.x;
        this->y = recoilsData.y;
        this->z = recoilsData.z;
    }
    
    RecoilsData() {
        
    }
    
    static void SetupTree(TTree* tree) {
        RecoilsData::tree = tree;
        RecoilsData::entries = tree->GetEntries();
        tree->SetBranchAddress("Event ID", &(RecoilsData::feventID));
        tree->SetBranchAddress("Track ID", &(RecoilsData::ftrackID));
        tree->SetBranchAddress("Parent ID", &(RecoilsData::fparentID));
        tree->SetBranchAddress("Global Time (ns)", &(RecoilsData::ftime));
        tree->SetBranchAddress("Particle Name", &(RecoilsData::fparticleName));
        tree->SetBranchAddress("Energy (keV)", &(RecoilsData::fenergy));
        tree->SetBranchAddress("Position x (mm)", &(RecoilsData::fx));
        tree->SetBranchAddress("Position y (mm)", &(RecoilsData::fy));
        tree->SetBranchAddress("Position z (mm)", &(RecoilsData::fz));
    }
    
    void ReadEntry(Long64_t entry) {
        if (tree) this->tree->GetEntry(entry);
        
        this->eventID = feventID;
        this->trackID = ftrackID;
        this->parentID = fparentID;
        this->time = ftime;
        strcpy(this->particleName, fparticleName);
        this->energy = fenergy;
        this->x = fx;
        this->y = fy;
        this->z = fz;
    }
};

std::ostream& operator<<(std::ostream& os, const RecoilsData& recoilsData) {
    os << "Tree Pointer: " << recoilsData.tree << "\n";
    os << "Event ID: " << recoilsData.eventID << "\n";
    os << "Track ID: " << recoilsData.trackID << "\n";
    os << "Parent ID: " << recoilsData.parentID << "\n";
    os << "Time (ns): " << recoilsData.time << "\n";
    os << "Particle Name: " << recoilsData.particleName << "\n";
    os << "Energy (keV): " << recoilsData.energy << "\n";
    os << "x (mm): " << recoilsData.x << "\n";
    os << "y (mm): " << recoilsData.y << "\n";
    os << "z (mm): " << recoilsData.z;
    return os;
}





struct EdepData {
    // For the data each struct object holds
    int eventID;
    int trackID;
    int parentID;
    double time;
    char particleName[12];
    double energy;
    double x;
    double y;
    double z;
    
    inline static TTree* tree = nullptr;
    inline static int entries = -1;
    
    // For the data just read off the tree
    inline static int feventID = -1;
    inline static int ftrackID = -1;
    inline static int fparentID = -1;
    inline static double ftime = -1;
    inline static char fparticleName[12] = "";
    inline static double fenergy = -1;
    inline static double fx = -1;
    inline static double fy = -1;
    inline static double fz = -1;
    
    EdepData(const EdepData& edepData) {
        this->eventID = edepData.eventID;
        this->trackID = edepData.trackID;
        this->parentID = edepData.parentID;
        this->time = edepData.time;
        strcpy(this->particleName, edepData.particleName);
        this->energy = edepData.energy;
        this->x = edepData.x;
        this->y = edepData.y;
        this->z = edepData.z;
    }
    
    EdepData() {
        
    }
    
    static void SetupTree(TTree* tree) {
        EdepData::tree = tree;
        EdepData::entries = tree->GetEntries();
        tree->SetBranchAddress("Event ID", &(EdepData::feventID));
        tree->SetBranchAddress("Track ID", &(EdepData::ftrackID));
        tree->SetBranchAddress("Parent ID", &(EdepData::fparentID));
        tree->SetBranchAddress("Global Time (ns)", &(EdepData::ftime));
        tree->SetBranchAddress("Particle Name", &(EdepData::fparticleName));
        tree->SetBranchAddress("Energy (keV)", &(EdepData::fenergy));
        tree->SetBranchAddress("End Position x (mm)", &(EdepData::fx));
        tree->SetBranchAddress("End Position y (mm)", &(EdepData::fy));
        tree->SetBranchAddress("End Position z (mm)", &(EdepData::fz));
    }
    
    void ReadEntry(Long64_t entry) {
        if (tree) this->tree->GetEntry(entry);
        
        this->eventID = feventID;
        this->trackID = ftrackID;
        this->parentID = fparentID;
        this->time = ftime;
        strcpy(this->particleName, fparticleName);
        this->energy = fenergy;
        this->x = fx;
        this->y = fy;
        this->z = fz;
    }
};

std::ostream& operator<<(std::ostream& os, const EdepData& edepData) {
    os << "Tree Pointer: " << edepData.tree << "\n";
    os << "Event ID: " << edepData.eventID << "\n";
    os << "Track ID: " << edepData.trackID << "\n";
    os << "Parent ID: " << edepData.parentID << "\n";
    os << "Time (ns): " << edepData.time << "\n";
    os << "Particle Name: " << edepData.particleName << "\n";
    os << "Energy (keV): " << edepData.energy << "\n";
    os << "x (mm): " << edepData.x << "\n";
    os << "y (mm): " << edepData.y << "\n";
    os << "z (mm): " << edepData.z;
    return os;
}




struct MetaData {
    // For the data each struct object holds
    double detector_hx;
    double detector_hy;
    double detector_hz;
    double pressure;
    
    inline static TTree* tree = nullptr;
    
    // For the data just read off the tree
    inline static double fdetector_hx = -1;
    inline static double fdetector_hy = -1;
    inline static double fdetector_hz = -1;
    inline static double fpressure = -1;
    
    MetaData(const MetaData& metaData) {
        this->detector_hx = metaData.detector_hx;
        this->detector_hy = metaData.detector_hy;
        this->detector_hz = metaData.detector_hz;
        this->pressure = metaData.pressure;
    }
    
    MetaData() {
        
    }
    
    static void SetupTree(TTree* tree) {
        MetaData::tree = std::move(tree);
        tree->SetBranchAddress("Detector_hx (mm)", &(MetaData::fdetector_hx));
        tree->SetBranchAddress("Detector_hy (mm)", &(MetaData::fdetector_hy));
        tree->SetBranchAddress("Detector_hz (mm)", &(MetaData::fdetector_hz));
        tree->SetBranchAddress("Pressure (torr)", &(MetaData::fpressure));
    }
    
    void ReadEntry(Long64_t entry) {
        if (tree) this->tree->GetEntry(entry);
        
        this->detector_hx = fdetector_hx;
        this->detector_hy = fdetector_hy;
        this->detector_hz = fdetector_hz;
        this->pressure = fpressure;
    }
};

std::ostream& operator<<(std::ostream& os, const MetaData& metaData) {
    os << "Detector_hx (mm): " << metaData.detector_hx << "\n";
    os << "Detector_hy (mm): " << metaData.detector_hy << "\n";
    os << "Detector_hz (mm): " << metaData.detector_hz << "\n";
    os << "Pressure (torr): " << metaData.pressure;
    return os;
}

void SetupDataStructs(TFile* tfile) {
    TTree* enteringTree;
    enteringTree = (TTree*) tfile->Get("Entering");
    EnteringData::SetupTree(enteringTree);

    TTree* leavingTree;
    leavingTree = (TTree*) tfile->Get("Leaving");
    LeavingData::SetupTree(leavingTree);

    TTree* recoilsTree;
    recoilsTree = (TTree*) tfile->Get("Recoils");
    RecoilsData::SetupTree(recoilsTree);

    TTree* edepTree;
    edepTree = (TTree*) tfile->Get("Edep");
    EdepData::SetupTree(edepTree);

    TTree* metaTree;
    metaTree = (TTree*) tfile->Get("Meta");
    MetaData::SetupTree(metaTree);
    
    // Read in metadata
    MetaData metadata; metadata.ReadEntry(0);
}

#endif // DATASTRUCTS_H
