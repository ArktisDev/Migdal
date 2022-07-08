#include "SteppingAction.hh"

#include "G4RootAnalysisManager.hh"
#include "G4Step.hh"
#include "G4StepStatus.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

SteppingAction::SteppingAction()
{
    prevEventID = -1;
    prevTrackID = -1;
    prevVolume = nullptr;
    generatedVolume = std::unordered_map<G4int, G4VPhysicalVolume *>();
}

SteppingAction::~SteppingAction()
{
}

void SteppingAction::UserSteppingAction(const G4Step *step)
{
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int trackID = step->GetTrack()->GetTrackID();
    G4int parentID = step->GetTrack()->GetParentID();

    G4VPhysicalVolume *curVolume = step->GetPreStepPoint()->GetPhysicalVolume();
    G4String curVolumeName = curVolume->GetName();

    // If we moved on to another event, no need to remember the volume map
    if (eventID != prevEventID)
    {
        prevEventID = eventID;
        prevVolume = nullptr;
        generatedVolume.clear();
    }

    // If we are now looking at a different particle and it is not the 1st particle, look up the parent volume
    if (trackID != prevTrackID)
    {
        prevTrackID = trackID;
        if (trackID != 1)
        {
            prevVolume = generatedVolume.at(parentID);
        }
    }

    if (step->GetNumberOfSecondariesInCurrentStep() > 0)
    {
        // Make note of the volume this track was generated in so it can be remembered in the future by the secondaries
        generatedVolume.emplace(trackID, curVolume);
    }

    G4double energy = step->GetPreStepPoint()->GetKineticEnergy();

    // Only discriminate on energy now, since particles like positrons have to take another step after losing all their energy
    // During that zero energy step is when they produce their secondaries, so it needs to be included in above calculation of parent volumes
    if (energy <= 0) {
        prevVolume = curVolume;
        return;
    };
    
    G4StepPoint* prePt = step->GetPreStepPoint();
    G4ThreeVector prePos = prePt->GetPosition();
    
    G4StepPoint* postPt = step->GetPostStepPoint();
    G4ThreeVector postPos = postPt->GetPosition();
    
    G4String particleName = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetParticleName();
    G4int particlePDG = step->GetTrack()->GetDynamicParticle()->GetDefinition()->GetPDGEncoding();
    G4bool isFirst = step->IsFirstStepInVolume();
    G4bool prePtGeomBoundary = prePt->GetStepStatus() == G4StepStatus::fGeomBoundary;
    G4bool isLast = step->IsLastStepInVolume();
    G4bool postPtGeomBoundary = postPt->GetStepStatus() == G4StepStatus::fGeomBoundary;
    
    G4double energyf = step->GetPostStepPoint()->GetKineticEnergy();
    G4double edep = step->GetTotalEnergyDeposit();
    G4double preTime = prePt->GetGlobalTime();
    G4double postTime = postPt->GetGlobalTime();
    
    G4String prevVolName = "NULL";
    if (prevVolume) prevVolName = prevVolume->GetName();
    
    G4RootAnalysisManager *man = G4RootAnalysisManager::Instance();
    
    if (curVolumeName.contains("Detector_phys"))
    {
        // To calculate flux coming into the detector, but not from parts inside detector
        if (isFirst && !prevVolName.contains("WirePlane") && !prevVolName.contains("FieldCage") && prePtGeomBoundary) {
            G4int id = 0;
            G4int tupleId = 0;
            man->FillNtupleIColumn(tupleId, id++, eventID);
            man->FillNtupleIColumn(tupleId, id++, trackID);
            man->FillNtupleIColumn(tupleId, id++, parentID);
            man->FillNtupleDColumn(tupleId, id++, preTime / ns);
            man->FillNtupleSColumn(tupleId, id++, particleName);
            man->FillNtupleDColumn(tupleId, id++, energy / keV);
            man->FillNtupleDColumn(tupleId, id++, prePos.x() / mm);
            man->FillNtupleDColumn(tupleId, id++, prePos.y() / mm);
            man->FillNtupleDColumn(tupleId, id++, prePos.z() / mm);
            man->AddNtupleRow(tupleId);
        }
        
        // To calculate flux coming out of the detector
        if (isLast && !postPt->GetPhysicalVolume()->GetName().contains("WirePlane") && postPtGeomBoundary) {
            G4int id = 0;
            G4int tupleId = 1;
            man->FillNtupleIColumn(tupleId, id++, eventID);
            man->FillNtupleIColumn(tupleId, id++, trackID);
            man->FillNtupleIColumn(tupleId, id++, parentID);
            man->FillNtupleDColumn(tupleId, id++, postTime / ns);
            man->FillNtupleSColumn(tupleId, id++, particleName);
            man->FillNtupleDColumn(tupleId, id++, energyf / keV);
            man->FillNtupleDColumn(tupleId, id++, postPos.x() / mm);
            man->FillNtupleDColumn(tupleId, id++, postPos.y() / mm);
            man->FillNtupleDColumn(tupleId, id++, postPos.z() / mm);
            man->AddNtupleRow(tupleId);
        }
        
        // All recoils inside detector
        // Electron (11), positron (-11), nuclear (> 1E5) recoils, but not tracks
        if (isFirst && energy > 0 && (particlePDG == 11 || particlePDG == -11 || particlePDG > 1E5)) {
            G4int id = 0;
            G4int tupleId = 2;
            man->FillNtupleIColumn(tupleId, id++, eventID);
            man->FillNtupleIColumn(tupleId, id++, trackID);
            man->FillNtupleIColumn(tupleId, id++, parentID);
            man->FillNtupleDColumn(tupleId, id++, preTime / ns);
            man->FillNtupleSColumn(tupleId, id++, particleName);
            man->FillNtupleDColumn(tupleId, id++, energy / keV);
            man->FillNtupleDColumn(tupleId, id++, prePos.x() / mm);
            man->FillNtupleDColumn(tupleId, id++, prePos.y() / mm);
            man->FillNtupleDColumn(tupleId, id++, prePos.z() / mm);
            man->AddNtupleRow(tupleId);
        }
        
        // All energy deposits (tracks) inside the detector
        if (edep > 0) {
            G4int id = 0;
            G4int tupleId = 3;
            man->FillNtupleIColumn(tupleId, id++, eventID);
            man->FillNtupleIColumn(tupleId, id++, trackID);
            man->FillNtupleIColumn(tupleId, id++, parentID);
            man->FillNtupleDColumn(tupleId, id++, preTime / ns);
            man->FillNtupleSColumn(tupleId, id++, particleName);
            man->FillNtupleSColumn(tupleId, id++, curVolumeName);
            man->FillNtupleDColumn(tupleId, id++, energy / keV);
            man->FillNtupleDColumn(tupleId, id++, edep / keV);
            man->FillNtupleDColumn(tupleId, id++, postPos.x() / mm);
            man->FillNtupleDColumn(tupleId, id++, postPos.y() / mm);
            man->FillNtupleDColumn(tupleId, id++, postPos.z() / mm);
            man->AddNtupleRow(tupleId);
        }
    }
    
    prevVolume = curVolume;
}