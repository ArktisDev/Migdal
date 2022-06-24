#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* detector)
: detector( detector )
{
	expDir = new G4UIdirectory("/exp/");
	expDir->SetGuidance("Experiment control");
	
	pressureCmd = new G4UIcmdWithADouble("/exp/setPressure", this);
  	pressureCmd->SetGuidance("Set Pressure in Torr");
  	pressureCmd->SetParameterName("Pressure", false);
  	pressureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	setSourceShieldCmd = new G4UIcmdWithAString("/exp/setSourceShield", this);
	setSourceShieldCmd->SetGuidance("Set source shielding structure in cm");
  	setSourceShieldCmd->SetParameterName("Shielding configuration", false);
  	setSourceShieldCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	addSourceShieldLayerCmd = new G4UIcmdWithAString("/exp/addSourceShieldLayer", this);
	addSourceShieldLayerCmd->SetGuidance("Add source shielding layer in cm");
  	addSourceShieldLayerCmd->SetParameterName("Shielding configuration", false);
  	addSourceShieldLayerCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	setSourceShieldInitialOffsetCmd = new G4UIcmdWithADouble("/exp/setSourceShieldInitialOffset", this);
	setSourceShieldInitialOffsetCmd->SetGuidance("Set initial offset in cm");
	setSourceShieldInitialOffsetCmd->SetParameterName("Initial offset (cm)", false);
	setSourceShieldInitialOffsetCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	setMaterialLogFileNameCmd = new G4UIcmdWithAString("/file/setMaterialLogFilename", this);
	setMaterialLogFileNameCmd->SetGuidance("Set material log file name");
	setMaterialLogFileNameCmd->SetParameterName("Filename", false);
	setMaterialLogFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
    delete expDir;
	delete pressureCmd;
	delete setSourceShieldCmd;
	delete addSourceShieldLayerCmd;
	delete setSourceShieldInitialOffsetCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
	G4bool geometryChanged = false;
    if (cmd == pressureCmd) {
		detector->SetPressure(pressureCmd->GetNewDoubleValue(newValue));
		geometryChanged = true;
	}
	if (cmd == setSourceShieldCmd) {
		detector->SetSourceShielding(newValue);
		geometryChanged = true;
	}
	if (cmd == addSourceShieldLayerCmd) {
		detector->AddSourceShieldLayer(newValue);
		geometryChanged = true;
	}
	if (cmd == setSourceShieldInitialOffsetCmd) {
		detector->SetSourceShieldInitialOffset(setSourceShieldInitialOffsetCmd->GetNewDoubleValue(newValue));
		geometryChanged = true;
	}
	if (cmd == setMaterialLogFileNameCmd) {
		detector->SetMaterialLogFilename(newValue);
	}
	
	if (geometryChanged) {
		// From documentation, it should be done like this - Lars
		
		G4RunManager* runManager = G4RunManager::GetRunManager();
		
		runManager->ReinitializeGeometry(true);
		
		// GEANT4 BUG
		// When running in GUI mode, if you change the geometry, the vis system gets very confused.
		// This should not be happening, and it is fixed in a new beta release
		// https://geant4-forum.web.cern.ch/t/gui-crashed-after-modifying-geometry-with-ui-commands-in-geant4-11-example-anaex01/7960/4
	}
}