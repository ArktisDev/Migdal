#ifndef RUNACTION_H
#define RUNACTION_H

#include "G4UserRunAction.hh"
#include "G4String.hh"
#include "ProgressBar.hh"

class RunMessenger;
class DetectorConstruction;

class RunAction : public G4UserRunAction
{
  public:
	RunAction(DetectorConstruction* detector);
	~RunAction() override;
	
	void BeginOfRunAction(const G4Run* run) override;
	void   EndOfRunAction(const G4Run* run) override;
	
	void SetOutputFilename(const G4String& filename);
	
	void SetProgress(int progress);
	
  private:
	RunMessenger* runMessenger;
	DetectorConstruction* detector;
	
	ProgressBar progressBar;
	
	G4String filename;
};

#endif // RUNACTION_H
