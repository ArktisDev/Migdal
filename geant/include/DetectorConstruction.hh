#ifndef DETECTORCONSTRUCTION_H
#define DETECTORCONSTRUCTION_H

#include "G4VUserDetectorConstruction.hh"
#include "G4Region.hh"

#include <string>
#include <vector>

class DetectorMessenger;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:
	DetectorConstruction();
	~DetectorConstruction() override;

	void DefineMaterials();
	G4VPhysicalVolume* DefineVolumes();
	
	G4VPhysicalVolume* Construct() override;
	
	void SetPressure(G4double pressure); // Set pressure in torr
	
	void AddSourceShieldLayer(G4String shieldingConfiguration);
	void SetSourceShieldInitialOffset(G4double initialOffset);
	
	void AddDetectorSideShieldLayer(G4String shieldingConfiguration);
	void SetDetectorSideShieldInitialXOffset(G4double initialOffset);
	void SetDetectorSideShieldInitialYOffset(G4double initialOffset);
	void SetDetectorSideShieldFWidthDeltaZ(G4double forwardDeltaZ);
	void SetDetectorSideShieldBWidthDeltaZ(G4double backwardDeltaZ);
	
	void AddDetectorBackShieldLayer(G4String shieldingConfiguration);
	void SetDetectorBackShieldInitialOffset(G4double initialOffset);
	void SetDetectorBackShieldWidthDeltaX(G4double deltaX);
	void SetDetectorBackShieldWidthDeltaY(G4double deltaY);
	
	void SetMaterialLogFilename(G4String filename);
	
	G4double GetPressure() {return this->pressure;}
	
  private:
	DetectorMessenger*  detectorMessenger;
	G4Region* 			detectorRegion;
	G4Region*			shieldingRegion;
	
	G4double 	  	    pressure;
	
	std::vector<std::vector<std::string>> 	sourceShieldStructure;
	G4double								sourceShieldInitialOffset;
	
	std::vector<std::vector<std::string>> 	detectorSideShieldStructure;
	G4double								detectorSideShieldInitialXOffset;
	G4double								detectorSideShieldInitialYOffset;
	G4double								detectorSideShieldFWidthDeltaZ;
	G4double								detectorSideShieldBWidthDeltaZ;
	
	std::vector<std::vector<std::string>> 	detectorBackShieldStructure;
	G4double								detectorBackShieldInitialOffset;
	G4double								detectorBackShieldWidthDeltaX;
	G4double								detectorBackShieldWidthDeltaY;
	
	std::string materialLogFilename = std::string("../data/materials.log");
};

#endif // DETECTORCONSTRUCTION_H
