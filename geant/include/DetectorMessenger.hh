#ifndef DETECTORMESSENGER_H
#define DETECTORMESSENGER_H

#include "G4UImessenger.hh"

#include "G4UIcmdWithADouble.hh"
#include "G4UIcmdWithAnInteger.hh"
#include "G4UIcmdWithAString.hh"

class DetectorConstruction;

class DetectorMessenger : public G4UImessenger
{
  public:
	DetectorMessenger(DetectorConstruction* detectorConstruction);
	~DetectorMessenger();
	
	void SetNewValue(G4UIcommand*, G4String);
	
  private:
	
	DetectorConstruction*	detector;
	
	G4UIdirectory*			expDir;
	G4UIcmdWithADouble*		pressureCmd;
	G4UIcmdWithAString*		setSourceShieldCmd;
	G4UIcmdWithAString*		addSourceShieldLayerCmd;
	G4UIcmdWithADouble*		setSourceShieldInitialOffsetCmd;
	G4UIcmdWithAString*		setMaterialLogFileNameCmd;
};



#endif // DETECTORMESSENGER_H
