#include "RunMessenger.hh"

#include "RunAction.hh"

RunMessenger::RunMessenger(RunAction* manager)
: outputAction ( manager )
{
  	fileDir = new G4UIdirectory("/file/");
  	fileDir->SetGuidance("Output control");
	
  	setFileNameCmd = new G4UIcmdWithAString("/file/setFileName", this);
  	setFileNameCmd->SetGuidance("Set name for the root file");
	setFileNameCmd->SetParameterName("Filename", false);
	setFileNameCmd->AvailableForStates(G4State_PreInit, G4State_Idle);  
}

RunMessenger::~RunMessenger()
{
	delete fileDir;
  	delete setFileNameCmd;
}

void RunMessenger::SetNewValue(G4UIcommand* command, G4String newValue)
{
	if(command == setFileNameCmd) outputAction->SetOutputFilename(newValue);
}
