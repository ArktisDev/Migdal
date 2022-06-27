#include "DetectorMessenger.hh"

#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"

DetectorMessenger::DetectorMessenger(DetectorConstruction* detector)
: detector( detector )
{
	expDir = new G4UIdirectory("/exp/");
	expDir->SetGuidance("Experiment control");
	
	pressureCmd = new G4UIcmdWithADouble("/exp/setPressure", this);
  	pressureCmd->SetGuidance("Set Pressure in Torr");
  	pressureCmd->SetParameterName("Pressure", false);
  	pressureCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
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
	
	addDetectorSideShieldLayerCmd  = new G4UIcmdWithAString("/exp/addDetectorSideShieldLayer", this);
	addDetectorSideShieldLayerCmd->SetGuidance("Add detector shielding layer in cm");
	addDetectorSideShieldLayerCmd->SetParameterName("Shielding configuration", false);
	addDetectorSideShieldLayerCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	setDetectorSideShieldInitialXOffsetCmd = new G4UIcmdWithADouble("/exp/setDetectorSideShieldInitialXOffset", this);
	setDetectorSideShieldInitialXOffsetCmd->SetGuidance("Set initial offset in cm");
	setDetectorSideShieldInitialXOffsetCmd->SetParameterName("Initial offset (cm)", false);
	setDetectorSideShieldInitialXOffsetCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	setDetectorSideShieldInitialYOffsetCmd = new G4UIcmdWithADouble("/exp/setDetectorSideShieldInitialYOffset", this);
	setDetectorSideShieldInitialYOffsetCmd->SetGuidance("Set initial offset in cm");
	setDetectorSideShieldInitialYOffsetCmd->SetParameterName("Initial offset (cm)", false);
	setDetectorSideShieldInitialYOffsetCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	setDetectorSideShieldBWidthDeltaZCmd = new G4UIcmdWithADouble("/exp/setDetectorSideShieldBWidthDeltaZ", this);
	setDetectorSideShieldBWidthDeltaZCmd->SetGuidance("Set backward delta width in cm");
	setDetectorSideShieldBWidthDeltaZCmd->SetParameterName("Delta width (cm)", false);
	setDetectorSideShieldBWidthDeltaZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	setDetectorSideShieldFWidthDeltaZCmd = new G4UIcmdWithADouble("/exp/setDetectorSideShieldFWidthDeltaZ", this);
	setDetectorSideShieldFWidthDeltaZCmd->SetGuidance("Set forward delta width in cm");
	setDetectorSideShieldFWidthDeltaZCmd->SetParameterName("Delta width (cm)", false);
	setDetectorSideShieldFWidthDeltaZCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	addDetectorBackShieldLayerCmd  = new G4UIcmdWithAString("/exp/addDetectorBackShieldLayer", this);
	addDetectorBackShieldLayerCmd->SetGuidance("Add detector shielding layer in cm");
	addDetectorBackShieldLayerCmd->SetParameterName("Shielding configuration", false);
	addDetectorBackShieldLayerCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	setDetectorBackShieldInitialOffsetCmd = new G4UIcmdWithADouble("/exp/setDetectorBackShieldInitialOffset", this);
	setDetectorBackShieldInitialOffsetCmd->SetGuidance("Set initial offset in cm");
	setDetectorBackShieldInitialOffsetCmd->SetParameterName("Initial offset (cm)", false);
	setDetectorBackShieldInitialOffsetCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	setDetectorBackShieldWidthDeltaXCmd = new G4UIcmdWithADouble("/exp/setDetectorBackShieldWidthDeltaX", this);
	setDetectorBackShieldWidthDeltaXCmd->SetGuidance("Set delta width in cm");
	setDetectorBackShieldWidthDeltaXCmd->SetParameterName("Delta width (cm)", false);
	setDetectorBackShieldWidthDeltaXCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
	
	setDetectorBackShieldWidthDeltaYCmd = new G4UIcmdWithADouble("/exp/setDetectorBackShieldWidthDeltaY", this);
	setDetectorBackShieldWidthDeltaYCmd->SetGuidance("Set delta width in cm");
	setDetectorBackShieldWidthDeltaYCmd->SetParameterName("Delta width (cm)", false);
	setDetectorBackShieldWidthDeltaYCmd->AvailableForStates(G4State_PreInit, G4State_Idle);
}

DetectorMessenger::~DetectorMessenger()
{
    if (expDir) 								delete expDir;
	if (pressureCmd) 							delete pressureCmd;
	
	if (addSourceShieldLayerCmd) 				delete addSourceShieldLayerCmd;
	if (setSourceShieldInitialOffsetCmd) 		delete setSourceShieldInitialOffsetCmd;
	
	if (addDetectorSideShieldLayerCmd) 			delete addDetectorSideShieldLayerCmd;
	if (setDetectorSideShieldInitialXOffsetCmd) delete setDetectorSideShieldInitialXOffsetCmd;
	if (setDetectorSideShieldInitialYOffsetCmd) delete setDetectorSideShieldInitialYOffsetCmd;
	if (setDetectorSideShieldBWidthDeltaZCmd) delete setDetectorSideShieldBWidthDeltaZCmd;
	if (setDetectorSideShieldFWidthDeltaZCmd) delete setDetectorSideShieldFWidthDeltaZCmd;
	
	if (addDetectorBackShieldLayerCmd) 			delete addDetectorBackShieldLayerCmd;
	if (setDetectorBackShieldInitialOffsetCmd) 	delete setDetectorBackShieldInitialOffsetCmd;
	if (setDetectorBackShieldWidthDeltaXCmd) 	delete setDetectorBackShieldWidthDeltaXCmd;
	if (setDetectorBackShieldWidthDeltaYCmd) 	delete setDetectorBackShieldWidthDeltaYCmd;
}

void DetectorMessenger::SetNewValue(G4UIcommand* cmd, G4String newValue)
{
	G4bool geometryChanged = false;
    if (cmd == pressureCmd) {
		detector->SetPressure(pressureCmd->GetNewDoubleValue(newValue));
		geometryChanged = true;
	} else
	if (cmd == addSourceShieldLayerCmd) {
		detector->AddSourceShieldLayer(newValue);
		geometryChanged = true;
	} else
	if (cmd == setSourceShieldInitialOffsetCmd) {
		detector->SetSourceShieldInitialOffset(setSourceShieldInitialOffsetCmd->GetNewDoubleValue(newValue) * cm);
		geometryChanged = true;
	} else
	if (cmd == setMaterialLogFileNameCmd) {
		detector->SetMaterialLogFilename(newValue);
	} else
	if (cmd == addDetectorSideShieldLayerCmd) {
		detector->AddDetectorSideShieldLayer(newValue);
		geometryChanged = true;
	} else
	if (cmd == setDetectorSideShieldInitialXOffsetCmd) {
		detector->SetDetectorSideShieldInitialXOffset(setDetectorSideShieldInitialXOffsetCmd->GetNewDoubleValue(newValue) * cm);
		geometryChanged = true;
	} else
	if (cmd == setDetectorSideShieldInitialYOffsetCmd) {
		detector->SetDetectorSideShieldInitialYOffset(setDetectorSideShieldInitialYOffsetCmd->GetNewDoubleValue(newValue) * cm);
		geometryChanged = true;
	} else
	if (cmd == setDetectorSideShieldFWidthDeltaZCmd) {
		detector->SetDetectorSideShieldFWidthDeltaZ(setDetectorSideShieldFWidthDeltaZCmd->GetNewDoubleValue(newValue) * cm);
		geometryChanged = true;
	} else
	if (cmd == setDetectorSideShieldBWidthDeltaZCmd) {
		detector->SetDetectorSideShieldBWidthDeltaZ(setDetectorSideShieldBWidthDeltaZCmd->GetNewDoubleValue(newValue) * cm);
		geometryChanged = true;
	} else
	if (cmd == addDetectorBackShieldLayerCmd) {
		detector->AddDetectorBackShieldLayer(newValue);
		geometryChanged = true;
	} else
	if (cmd == setDetectorBackShieldInitialOffsetCmd) {
		detector->SetDetectorBackShieldInitialOffset(setDetectorBackShieldInitialOffsetCmd->GetNewDoubleValue(newValue) * cm);
		geometryChanged = true;
	} else
	if (cmd == setDetectorBackShieldWidthDeltaXCmd) {
		detector->SetDetectorBackShieldWidthDeltaX(setDetectorBackShieldWidthDeltaXCmd->GetNewDoubleValue(newValue) * cm);
		geometryChanged = true;
	} else
	if (cmd == setDetectorBackShieldWidthDeltaYCmd) {
		detector->SetDetectorBackShieldWidthDeltaY(setDetectorBackShieldWidthDeltaYCmd->GetNewDoubleValue(newValue) * cm);
		geometryChanged = true;
	}
	
	if (geometryChanged) {
		// From documentation, it should be done like this - Lars
		
		G4RunManager* runManager = G4RunManager::GetRunManager();
		
		runManager->ReinitializeGeometry(true);
		
		// NOTE: GEANT4 BUG
		// When running in GUI mode, if you change the geometry, the vis system gets very confused.
		// This should not be happening, and it is fixed in a new beta release
		// https://geant4-forum.web.cern.ch/t/gui-crashed-after-modifying-geometry-with-ui-commands-in-geant4-11-example-anaex01/7960/4
	}
}