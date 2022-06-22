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
	const static std::string materialLogFilename;
	
	DetectorConstruction();
	~DetectorConstruction() override;

	void DefineMaterials();
	G4VPhysicalVolume* DefineVolumes();
	
	G4VPhysicalVolume* Construct() override;
	
	void SetPressure(G4double pressure); // Set pressure in torr
	void SetSourceShielding(G4String shieldingConfiguration);
	void AddSourceShieldLayer(G4String shieldingConfiguration);
	void SetSourceShieldInitialOffset(G4double initialOffset);
	
	G4double GetPressure() {return this->pressure;}
	
  private:
	DetectorMessenger*  detectorMessenger;
	G4Region* 			detectorRegion;
	G4Region*			shieldingRegion;
	
	G4double 	  	    pressure;
	G4double			sourceShieldInitialOffset;
	
	
	std::vector<std::vector<std::string>> 	sourceShieldStructure;
};

#endif // DETECTORCONSTRUCTION_H
