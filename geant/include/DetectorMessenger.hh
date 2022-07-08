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
	G4UIcmdWithADouble*		windowRadiusCmd;
	
	G4UIcmdWithAString*		addSourceShieldLayerCmd;
	G4UIcmdWithADouble*		setSourceShieldInitialOffsetCmd;
	
	G4UIcmdWithAString*		addDetectorSideShieldLayerCmd;
	G4UIcmdWithADouble*		setDetectorSideShieldInitialXOffsetCmd;
	G4UIcmdWithADouble*		setDetectorSideShieldInitialYOffsetCmd;
	G4UIcmdWithADouble*		setDetectorSideShieldFWidthDeltaZCmd;
	G4UIcmdWithADouble*		setDetectorSideShieldBWidthDeltaZCmd;
	
	G4UIcmdWithAString*		addDetectorBackShieldLayerCmd;
	G4UIcmdWithADouble*		setDetectorBackShieldInitialOffsetCmd;
	G4UIcmdWithADouble*		setDetectorBackShieldWidthDeltaXCmd;
	G4UIcmdWithADouble*		setDetectorBackShieldWidthDeltaYCmd;
	
	G4UIcmdWithAString*		setMaterialLogFileNameCmd;
};



#endif // DETECTORMESSENGER_H
