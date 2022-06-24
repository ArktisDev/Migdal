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
	void SetSourceShielding(G4String shieldingConfiguration);
	void AddSourceShieldLayer(G4String shieldingConfiguration);
	void SetDetectorShielding(G4String shieldingConfiguration);
	void AddDetectorShieldLayer(G4String shieldingConfiguration);
	void SetSourceShieldInitialOffset(G4double initialOffset);
	void SetDetectorShieldInitialXOffset(G4double initialOffset);
	void SetDetectorShieldInitialYOffset(G4double initialOffset);
	
	void SetMaterialLogFilename(G4String filename);
	
	G4double GetPressure() {return this->pressure;}
	
  private:
	DetectorMessenger*  detectorMessenger;
	G4Region* 			detectorRegion;
	G4Region*			shieldingRegion;
	
	G4double 	  	    pressure;
	G4double			sourceShieldInitialOffset;
	G4double			detectorShieldInitialXOffset;
	G4double			detectorShieldInitialYOffset;
	
	std::string materialLogFilename = std::string("../data/materials.log");
	
	std::vector<std::vector<std::string>> 	sourceShieldStructure;
	std::vector<std::vector<std::string>> 	detectorShieldStructure;
};

#endif // DETECTORCONSTRUCTION_H
