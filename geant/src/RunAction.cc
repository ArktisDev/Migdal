#include "RunAction.hh"

#include <iostream>

#include "G4RootAnalysisManager.hh"

#include "RunMessenger.hh"
#include "DetectorConstruction.hh"

RunAction::RunAction(DetectorConstruction* detector)
: runMessenger( new RunMessenger(this) ), detector( detector ), filename( "output.root" )
{
    
}

RunAction::~RunAction()
{
    
}

void RunAction::BeginOfRunAction(const G4Run*)
{
    G4RootAnalysisManager* man = G4RootAnalysisManager::Instance();
    man->OpenFile(filename);
    man->Clear();
    
    man->CreateNtuple("Entering", "data");
    man->CreateNtupleIColumn("Event ID");
    man->CreateNtupleIColumn("Track ID");
    man->CreateNtupleIColumn("Parent ID");
    man->CreateNtupleDColumn("Global Time (ns)");
    man->CreateNtupleSColumn("Particle Name");
    man->CreateNtupleDColumn("Energy (keV)");
    man->CreateNtupleDColumn("Position x (mm)");
    man->CreateNtupleDColumn("Position y (mm)");
    man->CreateNtupleDColumn("Position z (mm)");
    man->FinishNtuple(0);
    
    man->CreateNtuple("Leaving", "data");
    man->CreateNtupleIColumn("Event ID");
    man->CreateNtupleIColumn("Track ID");
    man->CreateNtupleIColumn("Parent ID");
    man->CreateNtupleDColumn("Global Time (ns)");
    man->CreateNtupleSColumn("Particle Name");
    man->CreateNtupleDColumn("Energy (keV)");
    man->CreateNtupleDColumn("Position x (mm)");
    man->CreateNtupleDColumn("Position y (mm)");
    man->CreateNtupleDColumn("Position z (mm)");
    man->FinishNtuple(1);
    
    man->CreateNtuple("Recoils", "data");
    man->CreateNtupleIColumn("Event ID");
    man->CreateNtupleIColumn("Track ID");
    man->CreateNtupleIColumn("Parent ID");
    man->CreateNtupleDColumn("Global Time (ns)");
    man->CreateNtupleSColumn("Particle Name");
    man->CreateNtupleDColumn("Energy (keV)");
    man->CreateNtupleDColumn("Position x (mm)");
    man->CreateNtupleDColumn("Position y (mm)");
    man->CreateNtupleDColumn("Position z (mm)");
    man->FinishNtuple(2);
    
    man->CreateNtuple("Edep", "data");
    man->CreateNtupleIColumn("Event ID");
    man->CreateNtupleIColumn("Track ID");
    man->CreateNtupleIColumn("Parent ID");
    man->CreateNtupleDColumn("Global Time (ns)");
    man->CreateNtupleSColumn("Particle Name");
    man->CreateNtupleDColumn("Energy (keV)");
    man->CreateNtupleDColumn("Edep (keV)");
    man->CreateNtupleDColumn("End Position x (mm)");
    man->CreateNtupleDColumn("End Position y (mm)");
    man->CreateNtupleDColumn("End Position z (mm)");
    man->FinishNtuple(3);
    
    man->CreateNtuple("Meta", "data");
    man->CreateNtupleDColumn("Detector_hx (mm)");
    man->CreateNtupleDColumn("Detector_hy (mm)");
    man->CreateNtupleDColumn("Detector_hz (mm)");
    man->CreateNtupleDColumn("Pressure (torr)");
    man->FinishNtuple(4);
    
    G4int id = 0;
    G4int tupleID = 4;
    
    man->FillNtupleDColumn(tupleID, id++, detector->GetDetectorHx());
    man->FillNtupleDColumn(tupleID, id++, detector->GetDetectorHy());
    man->FillNtupleDColumn(tupleID, id++, detector->GetDetectorHz());
    man->FillNtupleDColumn(tupleID, id++, detector->GetPressure());
    man->AddNtupleRow(tupleID);
}

void RunAction::EndOfRunAction(const G4Run*)
{
    G4RootAnalysisManager* man = G4RootAnalysisManager::Instance();
    
    man->Write();
    man->CloseFile();
}

void RunAction::SetOutputFilename(const G4String& filename) 
{
    this->filename = filename;
}