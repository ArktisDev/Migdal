#include "G4UIExecutive.hh"
#include "G4RunManagerFactory.hh"
#include "G4VisExecutive.hh"
#include "G4UImanager.hh"
#include "G4PhysListFactory.hh"
#include "G4SystemOfUnits.hh"

#include "Randomize.hh"

#include <chrono>

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "RunAction.hh"
#include "SteppingAction.hh"

int main(int argc, char** argv) 
{
    // Detect interactive mode (if no arguments) and define UI session
    G4UIExecutive* ui = nullptr;
    if ( argc == 1 || !strcmp(argv[1], "--ui")) {
        ui = new G4UIExecutive(argc, argv);
    }
    
    CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine);
    CLHEP::HepRandom::setTheSeed(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    
	// Create default run manager
	G4RunManager* runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::SerialOnly);
    
    // Initialize the detector
	DetectorConstruction* detector = new DetectorConstruction();
	runManager->SetUserInitialization(detector);
    
    // Initialize physics list
    G4PhysListFactory physListFactory;
    G4VModularPhysicsList* plist = physListFactory.GetReferencePhysList("QGSP_BIC_HP_EMZ"); 
    runManager->SetUserInitialization(plist);
    
    // Initialize primary generator action
    PrimaryGeneratorAction* primary = new PrimaryGeneratorAction();
    runManager->SetUserAction(primary);
    
    // Initialize run action
    RunAction* runAction = new RunAction();
    runManager->SetUserAction(runAction);
    
    // Initialize stepping action
    SteppingAction* steppingAction = new SteppingAction;
    runManager->SetUserAction(steppingAction);
    
    // Initialize visualization
    G4VisManager* visManager = new G4VisExecutive("warnings");
    visManager->Initialize();

    // Get the pointer to the User Interface manager
    G4UImanager* UImanager = G4UImanager::GetUIpointer();
    
    // Process macro or start UI session
    if ( !ui ) {
        // Batch mode
        G4String command = "/control/execute ";
        for (int i = 1; i < argc; i++) {
            G4String fileName = argv[i];
            UImanager->ApplyCommand(command + fileName);
        }
    }
    else {
        // Interactive mode, but run included pre-vis macros first
        G4String command = "/control/execute ";
        for (int i = 2; i < argc; i++) {
            G4String fileName = argv[i];
            UImanager->ApplyCommand(command + fileName);
        }
        UImanager->ApplyCommand("/control/execute macros/Vis.mac");
        ui->SessionStart();
        delete ui;
    }
    
    // Job termination
    // Free the store: user actions, physics_list and detector_description are
    // owned and deleted by the run manager, so they should not be deleted
    // in the main() program !
    delete visManager;
    delete runManager;
    
    return 0;
}