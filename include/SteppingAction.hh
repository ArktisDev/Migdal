#ifndef STEPPINGACTION_H
#define STEPPINGACTION_H

#include "G4UserSteppingAction.hh"

#include <unordered_map>

#include "G4VPhysicalVolume.hh"

class SteppingAction : public G4UserSteppingAction {
  public:
    SteppingAction();
    ~SteppingAction();

    // method from the base class
    void UserSteppingAction(const G4Step*) override;

   
  private:
    G4int prevEventID;
    G4int prevTrackID;
    
    G4VPhysicalVolume* prevVolume;
    
    // So that if a daughter is generated, it can look up the volume it was generated in
    std::unordered_map<G4int, G4VPhysicalVolume*> generatedVolume;
};

#endif // STEPPINGACTION_H
