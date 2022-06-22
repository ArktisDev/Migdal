#ifndef RUNMESSENGER_H
#define RUNMESSENGER_H

#include "G4UImessenger.hh"
#include "G4UIcmdWithAString.hh"

class RunAction;

class RunMessenger : public G4UImessenger
{
  public:
	RunMessenger(RunAction* runAction);
	~RunMessenger();
	
	void SetNewValue(G4UIcommand*, G4String);
	
  private:
	
	RunAction*              outputAction;
	
	G4UIdirectory*          fileDir;
	G4UIcmdWithAString*     setFileNameCmd;
};

#endif // RUNMESSENGER_H
