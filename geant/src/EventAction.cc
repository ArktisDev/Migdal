#include "EventAction.hh"

EventAction::EventAction(RunAction* runAction)
: runAction( runAction )
{
    
}
EventAction::~EventAction() {
    
}

void EventAction::BeginOfEventAction(const G4Event* event) {
    
}
void   EventAction::EndOfEventAction(const G4Event* event) {
    runAction->SetProgress(event->GetEventID() + 1);
}