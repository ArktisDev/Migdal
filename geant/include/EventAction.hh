#ifndef EVENTACTION_H
#define EVENTACTION_H

#include "G4UserEventAction.hh"
#include "G4Event.hh"

#include "RunAction.hh"

class EventAction : public G4UserEventAction {
  public:
    EventAction(RunAction* runAction);
	~EventAction() override;
    
    void BeginOfEventAction(const G4Event* event) override;
	void   EndOfEventAction(const G4Event* event) override;
  private:
    RunAction* runAction;
};

#endif // EVENTACTION_H
