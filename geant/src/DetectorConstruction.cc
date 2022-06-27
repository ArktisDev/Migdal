#include "DetectorConstruction.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4VisAttributes.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnitsTable.hh"
#include "G4MaterialTable.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"

#include "DetectorMessenger.hh"

#include <iostream>

const G4double inch = 2.54 * cm;

DetectorConstruction::DetectorConstruction()
: detectorMessenger(new DetectorMessenger(this)), detectorRegion(0), shieldingRegion(0), pressure(75), 
  sourceShieldInitialOffset(1 * cm), detectorSideShieldInitialXOffset(1 * cm), detectorSideShieldInitialYOffset(1 * cm),
  detectorBackShieldInitialOffset(1 * cm), detectorBackShieldWidthDeltaX(0), detectorBackShieldWidthDeltaY(0)
{
	
}

DetectorConstruction::~DetectorConstruction() 
{
	if (shieldingRegion) delete shieldingRegion;
	if (detectorRegion) delete detectorRegion;
}

void DetectorConstruction::DefineMaterials()
{
	// Get nist material manager
	G4NistManager* nist = G4NistManager::Instance();
	
	nist->FindOrBuildMaterial("G4_AIR");
	nist->FindOrBuildMaterial("G4_Cu");
	nist->FindOrBuildMaterial("G4_Al");
	nist->FindOrBuildMaterial("G4_MYLAR");
	nist->FindOrBuildMaterial("G4_Pb");
	nist->FindOrBuildMaterial("G4_Cd");
	nist->FindOrBuildMaterial("G4_KAPTON");
	nist->FindOrBuildMaterial("G4_POLYETHYLENE");
	
  	G4int ncomp, natoms;
	G4double density;

	density = pressure * 88 / 62.364 / 293 * g / (1000 * cm3);  // pM/RT in g/L
	G4Material* CF4 = new G4Material ("CF4", density, ncomp=2);
	CF4->AddElement(nist->FindOrBuildElement("C"), natoms=1);
	CF4->AddElement(nist->FindOrBuildElement("F"), natoms=1);
	
	G4double fracMass;
	
	density = 11.29*g/cm3;
	G4Material* newLead = new G4Material("Impure_Pb", density, ncomp=10);
	newLead->AddMaterial(nist->FindOrBuildMaterial("G4_Pb"),fracMass=99.9655*perCent);
  	newLead->AddMaterial(nist->FindOrBuildMaterial("G4_Sb"),fracMass=0.001*perCent);
  	newLead->AddMaterial(nist->FindOrBuildMaterial("G4_As"),fracMass=0.001*perCent);
  	newLead->AddMaterial(nist->FindOrBuildMaterial("G4_Sn"),fracMass=0.001*perCent);
  	newLead->AddMaterial(nist->FindOrBuildMaterial("G4_Cu"),fracMass=0.001*perCent);
  	newLead->AddMaterial(nist->FindOrBuildMaterial("G4_Bi"),fracMass=0.025*perCent);
  	newLead->AddMaterial(nist->FindOrBuildMaterial("G4_Fe"),fracMass=0.001*perCent);
  	newLead->AddMaterial(nist->FindOrBuildMaterial("G4_Ni"),fracMass=0.001*perCent);
  	newLead->AddMaterial(nist->FindOrBuildMaterial("G4_Ag"),fracMass=0.003*perCent);
  	newLead->AddMaterial(nist->FindOrBuildMaterial("G4_Zn"),fracMass=0.0005*perCent);
	
	density = 1.04;
	G4Material* boratedPoly = new G4Material("Borated Poly", density, ncomp=2);
	boratedPoly->AddMaterial(nist->FindOrBuildMaterial("G4_POLYETHYLENE"), fracMass = 95 * perCent);
	boratedPoly->AddMaterial(nist->FindOrBuildMaterial("G4_B"), fracMass = 5 * perCent);
	
	// Log the material table
	std::ofstream materialLog(materialLogFilename);
	
	materialLog << *(G4Material::GetMaterialTable());
		
	materialLog.close();
}

G4VPhysicalVolume* DetectorConstruction::DefineVolumes()
{
	G4bool checkOverlaps = true;
	
	G4Material* air 		= G4Material::GetMaterial("G4_AIR");
	G4Material* copper 		= G4Material::GetMaterial("G4_Cu");
	G4Material* CF4 		= G4Material::GetMaterial("CF4");
	G4Material* aluminum 	= G4Material::GetMaterial("G4_Al");
	G4Material* mylar 		= G4Material::GetMaterial("G4_MYLAR");
	G4Material* kapton		= G4Material::GetMaterial("G4_KAPTON");
	
	if (detectorRegion) delete detectorRegion;
	if (shieldingRegion) delete shieldingRegion;
	
	detectorRegion = new G4Region("DetectorRegion");
	shieldingRegion = new G4Region("ShieldRegion");
	
	// TODO : APPLY CUTS
	G4double 			world_hx 		= 5 * m;
	G4double 			world_hy 		= 5 * m;
	G4double 			world_hz 		= 5 * m;
	G4Material* 		world_mat 		= air;
	G4VisAttributes* 	world_visAttr	= new G4VisAttributes(true, G4Color(1, 1, 1, 0.1));
	world_visAttr->SetForceWireframe();
	
	G4Box*	    		World_solid  	= new G4Box("World", world_hx, world_hy, world_hz);
	G4LogicalVolume*	World_logical 	= new G4LogicalVolume(World_solid, world_mat, "World_log");
	G4VPhysicalVolume*	World_physical 	= new G4PVPlacement(0, G4ThreeVector(), World_logical, "World_phys", 0, false, 0, checkOverlaps);
	World_logical->SetVisAttributes(world_visAttr);
	
	G4double 			detector_hx 		= 10 * cm;
	G4double 			detector_hy 		= 10 * cm;
	G4double 			detector_hz 		= 10 * cm;
	G4Material* 		detector_mat 		= CF4;
	G4VisAttributes*    detector_visAttr  	= new G4VisAttributes(G4Color::Cyan());

    G4Box*	            Detector_solid		= new G4Box("Detector_sol", detector_hx, detector_hy, detector_hz);
	G4LogicalVolume*	Detector_logical  	= new G4LogicalVolume(Detector_solid, detector_mat, "Detector_log");
	G4VPhysicalVolume*  Detector_physical 	= new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * cm), Detector_logical, "Detector_phys", World_logical, false, 0, checkOverlaps);
	Detector_logical->SetVisAttributes(detector_visAttr);
	
	G4double 			wireplane_hx 		= 8 * cm;
	G4double 			wireplane_hy 		= 0.5 * cm;
	G4double 			wireplane_hz 		= 8 * cm;
	G4Material* 		wireplane_mat 		= copper;
	G4VisAttributes*    wireplane_visAttr  	= new G4VisAttributes(G4Color(0.72, 0.41, 0.20));
	G4ThreeVector       wireplane_pos       = G4ThreeVector(0, -0.45 * detector_hy, 0);
	
	G4Box*	            WirePlane_solid 	= new G4Box("WirePlane_sol", wireplane_hx, wireplane_hy, wireplane_hz);
	G4LogicalVolume*	WirePlane_logical	= new G4LogicalVolume(WirePlane_solid, wireplane_mat, "WirePlane_log");
	G4VPhysicalVolume*  Wireplane_physical	= new G4PVPlacement(0, wireplane_pos, WirePlane_logical, "WirePlane_phys", Detector_logical, false, 0, checkOverlaps);
    WirePlane_logical->SetVisAttributes(wireplane_visAttr);
	
	G4double			detectorCase_thickX 	= 1 * cm;
	G4double			detectorCase_thickY 	= 1 * cm;
	G4double			detectorCase_thickZ 	= 1 * cm;
	G4Material* 		detectorCase_mat 		= aluminum;
	G4VisAttributes*    detectorCase_visAttr	= new G4VisAttributes(G4Color::Gray());
	
	G4double            windowRadius			= 2.5 * cm;
	G4double			windowThickness			= 1 * mm;
	G4Material* 		window_mat 				= mylar;
	G4VisAttributes*    window_visAttr 			= new G4VisAttributes(G4Color(0.1,0.2,0.5));
	
	G4Box*	            DetectorCase0_solid    	= new G4Box("DetectorCase0_sol", detector_hx + detectorCase_thickX, detector_hy + detectorCase_thickY, detector_hz + detectorCase_thickZ); //box
	G4VSolid*           DetectorCase1_solid    	= new G4SubtractionSolid("DetectorCase1_sol", DetectorCase0_solid, Detector_solid, 0, G4ThreeVector(0, 0, 0)); //minus detector
    G4Tubs*             DetectorCase2_solid    	= new G4Tubs("DetectorCase2_sol", 0, windowRadius, 2 * detectorCase_thickZ, 0, 360 * degree); //window cut out
    G4VSolid*           DetectorCase3_solid    	= new G4SubtractionSolid("DetectorCase3_sol", DetectorCase1_solid, DetectorCase2_solid, 0, G4ThreeVector(0, 0, detector_hz + 0.5 * detectorCase_thickZ)); //front window
    G4VSolid*           DetectorCase4_solid    	= new G4SubtractionSolid("DetectorCase4_sol", DetectorCase3_solid, DetectorCase2_solid, 0, G4ThreeVector(0, 0, -detector_hz - 0.5 * detectorCase_thickZ)); //back window
    G4LogicalVolume*	DetectorCase_logical   	= new G4LogicalVolume(DetectorCase4_solid, detectorCase_mat, "DetectorCase_log");
    G4VPhysicalVolume*  DetectorCase_physical  	= new G4PVPlacement(0, G4ThreeVector(0, 0, 0), DetectorCase_logical, "DetectorCase_phys", World_logical, false, 0, checkOverlaps);
	DetectorCase_logical->SetVisAttributes(detectorCase_visAttr);
    G4Tubs*             Window_solid      		= new G4Tubs("DetectorCaseWindow_sol", 0, windowRadius, 0.5 * windowThickness, 0, 360 * degree); //window
    G4LogicalVolume*	Window_logical    		= new G4LogicalVolume(Window_solid, window_mat, "DetectorCaseWindow_sol");
    G4VPhysicalVolume*  WindowF_physical  		= new G4PVPlacement(0, G4ThreeVector(0, 0, detector_hz + 0.5 * windowThickness), Window_logical, "DetectorCaseWindowF_phys", World_logical, false, 0, checkOverlaps);
    G4VPhysicalVolume*  WindowB_physical  		= new G4PVPlacement(0, G4ThreeVector(0, 0, -detector_hz - 0.5 * windowThickness), Window_logical, "DetectorCaseWindowB_phys", World_logical, false, 0, checkOverlaps);
    Window_logical->SetVisAttributes(window_visAttr);
	
	G4double 			motherboard_longSide	= 20 * cm;
	G4double 			motherboard_hy			= 0.2 * cm;
	G4double			motherboard_shortSide	= 10 * cm;
	G4Material*			motherboard_mat			= kapton;
	G4VisAttributes*    motherboard_visAttr		= new G4VisAttributes(G4Color::Green());
	G4double			motherboard_yOffset		= -1 * cm;
	G4double			motherboard_dist		= 15 * cm;
	
	G4Box*	            Motherboard1_solid    	= new G4Box("Motherboard1_sol", motherboard_longSide, motherboard_hy, motherboard_shortSide);
	G4LogicalVolume*	Motherboard1_logical  	= new G4LogicalVolume(Motherboard1_solid, motherboard_mat, "Motherboard1_sol");
    G4VPhysicalVolume*  Motherboard1A_physical	= new G4PVPlacement(0, G4ThreeVector(0, wireplane_pos.y() + motherboard_yOffset, motherboard_dist + detector_hz), Motherboard1_logical, "Motherboard1a_phys", World_logical, false, 0, checkOverlaps);
    G4VPhysicalVolume*  Motherboard1B_physical	= new G4PVPlacement(0, G4ThreeVector(0, wireplane_pos.y() + motherboard_yOffset, -(motherboard_dist + detector_hz)), Motherboard1_logical, "Motherboard1b_phys", World_logical, false, 0, checkOverlaps);
    G4Box*	            Motherboard2_solid    	= new G4Box("Motherboard2_sol", motherboard_shortSide, motherboard_hy, motherboard_longSide);
    G4LogicalVolume*	Motherboard2_logical  	= new G4LogicalVolume(Motherboard2_solid, motherboard_mat, "Motherboard2_sol");
    G4VPhysicalVolume*  Motherboard2A_physical	= new G4PVPlacement(0, G4ThreeVector(motherboard_dist + detector_hx, wireplane_pos.y() - motherboard_yOffset, 0), Motherboard2_logical, "Motherboard2a_phys", World_logical, false, 0, checkOverlaps);
    G4VPhysicalVolume*  Motherboard2B_physical	= new G4PVPlacement(0, G4ThreeVector(-(motherboard_dist + detector_hx), wireplane_pos.y() - motherboard_yOffset, 0), Motherboard2_logical, "Motherboard2b_phys", World_logical, false, 0, checkOverlaps);
    Motherboard1_logical->SetVisAttributes(motherboard_visAttr);
    Motherboard2_logical->SetVisAttributes(motherboard_visAttr);
	
	detectorRegion->AddRootLogicalVolume(Detector_logical);
	detectorRegion->AddRootLogicalVolume(WirePlane_logical);
	detectorRegion->AddRootLogicalVolume(DetectorCase_logical);
	detectorRegion->AddRootLogicalVolume(Window_logical);
	
	// Start source shielding
	
	G4Material* 		poly 			= G4Material::GetMaterial("G4_POLYETHYLENE");
	G4Material* 		boratedPoly 	= G4Material::GetMaterial("Borated Poly");
	G4Material* 		lead 			= G4Material::GetMaterial("G4_Pb");
	G4Material* 		cadmium 		= G4Material::GetMaterial("G4_Cd");
	G4Material* 		impureLead 		= G4Material::GetMaterial("Impure_Pb");
	
	G4VisAttributes*    lead_visAttr 		= new G4VisAttributes(G4Color::Yellow());
	G4VisAttributes*    cadmium_visAttr 	= new G4VisAttributes(G4Color::Blue());
	G4VisAttributes*    poly_visAttr 		= new G4VisAttributes(G4Color::Brown());
	G4VisAttributes*    boratedPoly_visAttr = new G4VisAttributes(G4Color::Red());
	
	G4double radius = 50 * cm;
	G4double spacing = 1 * cm;
	
	G4double total_offset = detector_hz + motherboard_dist + motherboard_shortSide + detectorSideShieldFWidthDeltaZ + sourceShieldInitialOffset;
	
	for (const std::vector<std::string>& layer : sourceShieldStructure) {
		G4double thickness = std::stod(layer[0]) * cm;
		G4double innerRadius = std::stod(layer[2]) * cm;
		G4Material* shield_mat = nullptr;
		G4VisAttributes* visAttr = nullptr;
		
		if (layer[1] == "Poly") {
			shield_mat = poly;
			visAttr = poly_visAttr;
		} else if (layer[1] == "Cd") {
			shield_mat = cadmium;
			visAttr = cadmium_visAttr;
		} else if (layer[1] == "Pb") {
			shield_mat = lead;
			visAttr = lead_visAttr;
		} else if (layer[1] == "BPoly") {
			shield_mat = boratedPoly;
			visAttr = boratedPoly_visAttr;
		} else {
			G4Exception("DetectorConstruction::DefineVolumes()", "-1", G4ExceptionSeverity::FatalException, 
				std::string("Material ").append(" is not recognized").c_str());
		}
		
		G4Tubs* SourceShield_solid = new G4Tubs("SourceShield_sol", innerRadius, radius, thickness / 2, 0, 360 * deg);
		G4LogicalVolume* SourceShield_logical = new G4LogicalVolume(SourceShield_solid, shield_mat, "SourceShield_logical");
		G4VPhysicalVolume* SourceShield_physical = new G4PVPlacement(0, G4ThreeVector(0, 0, total_offset + thickness / 2), SourceShield_logical, "SourceShield_phys", World_logical, false, 0, checkOverlaps);
		SourceShield_logical->SetVisAttributes(visAttr);
		
		shieldingRegion->AddRootLogicalVolume(SourceShield_logical);
		
		total_offset += thickness + spacing;
	}
	
	// Keep materials and visattributes from source shielding and use them to construct detector side shielding
	
	G4double totalXoffset = detector_hx + motherboard_dist + motherboard_shortSide + detectorSideShieldInitialXOffset;
	G4double totalYoffset = detector_hy + detectorCase_thickY + detectorSideShieldInitialYOffset;
	G4double detectorSideShieldFHz = detector_hz + motherboard_dist + motherboard_shortSide + detectorSideShieldFWidthDeltaZ;
	G4double detectorSideShieldBHz = detector_hz + motherboard_dist + motherboard_shortSide + detectorSideShieldBWidthDeltaZ;
	G4double detectorSideShieldHz = (detectorSideShieldFHz + detectorSideShieldBHz) / 2;
	G4double detectorZpos = detectorSideShieldFHz - detectorSideShieldHz;
	
	spacing = 1 * cm;
	
	for (const std::vector<std::string>& layer : detectorSideShieldStructure) {
		G4double thickness = std::stod(layer[0]) * cm;
		G4Material* shield_mat = nullptr;
		G4VisAttributes* visAttr = nullptr;
		
		if (layer[1] == "Poly") {
			shield_mat = poly;
			visAttr = poly_visAttr;
		} else if (layer[1] == "Cd") {
			shield_mat = cadmium;
			visAttr = cadmium_visAttr;
		} else if (layer[1] == "Pb") {
			shield_mat = lead;
			visAttr = lead_visAttr;
		} else if (layer[1] == "BPoly") {
			shield_mat = boratedPoly;
			visAttr = boratedPoly_visAttr;
		} else {
			G4Exception("DetectorConstruction::DefineVolumes()", "-1", G4ExceptionSeverity::FatalException, 
				std::string("Material ").append(" is not recognized").c_str());
		}
		
		// Now to construct the shielding as concentric boxes
		G4Box* 		DetectorSideShield_solid1 = new G4Box("DetectorSideShield_sol", totalXoffset + thickness, totalYoffset + thickness, detectorSideShieldHz);
		G4Box* 		DetectorSideShield_cutout = new G4Box("DetectorSideShield_cutout", totalXoffset, totalYoffset, detectorSideShieldHz + 1 * cm);
		G4VSolid* 	DetectorSideShield_solid2 = new G4SubtractionSolid("DetectorSideShield_sol2", DetectorSideShield_solid1, DetectorSideShield_cutout);
		G4LogicalVolume* DetectorSideShield_logical = new G4LogicalVolume(DetectorSideShield_solid2, shield_mat, "DetectorSideShield_logical");
		G4VPhysicalVolume* DetectorSideShield_physical = new G4PVPlacement(0, G4ThreeVector(0, 0, detectorZpos), DetectorSideShield_logical, "DetectorSideShield_phys", World_logical, false, 0, checkOverlaps);
		DetectorSideShield_logical->SetVisAttributes(visAttr);
		
		shieldingRegion->AddRootLogicalVolume(Detector_logical);
		
		totalXoffset += thickness + spacing;
		totalYoffset += thickness + spacing;
	}
	
	// Keep materials and vis attributes again and use them to construct detector back shielding
	
	total_offset = detector_hz + motherboard_dist + motherboard_shortSide + detectorSideShieldBWidthDeltaZ + detectorBackShieldInitialOffset;
	G4double detectorBackShieldHx = detector_hx + motherboard_dist + motherboard_shortSide + detectorBackShieldWidthDeltaX;
	G4double detectorBackShieldHy = detector_hy + detectorCase_thickY + detectorBackShieldWidthDeltaY;
	
	spacing = 1 * cm;
	
	for (const std::vector<std::string>& layer : detectorBackShieldStructure) {
		G4double thickness = std::stod(layer[0]) * cm;
		G4Material* shield_mat = nullptr;
		G4VisAttributes* visAttr = nullptr;
		
		if (layer[1] == "Poly") {
			shield_mat = poly;
			visAttr = poly_visAttr;
		} else if (layer[1] == "Cd") {
			shield_mat = cadmium;
			visAttr = cadmium_visAttr;
		} else if (layer[1] == "Pb") {
			shield_mat = lead;
			visAttr = lead_visAttr;
		} else if (layer[1] == "BPoly") {
			shield_mat = boratedPoly;
			visAttr = boratedPoly_visAttr;
		} else {
			G4Exception("DetectorConstruction::DefineVolumes()", "-1", G4ExceptionSeverity::FatalException, 
				std::string("Material ").append(" is not recognized").c_str());
		}
		
		// Now to construct the shielding as plates
		G4Box* 			DetectorBackShield_solid = new G4Box("DetectorBackShield_sol", detectorBackShieldHx, detectorBackShieldHy, thickness / 2);
		G4LogicalVolume* DetectorBackShield_logical = new G4LogicalVolume(DetectorBackShield_solid, shield_mat, "DetectorBackShield_logical");
		G4VPhysicalVolume* DetectorBackShield_physical = new G4PVPlacement(0, G4ThreeVector(0, 0, - total_offset - thickness / 2), DetectorBackShield_logical, "DetectorBackShield_phys", World_logical, false, 0, checkOverlaps);
		DetectorBackShield_logical->SetVisAttributes(visAttr);
		
		shieldingRegion->AddRootLogicalVolume(DetectorBackShield_logical);
		
		total_offset += thickness + spacing;
	}
	
	return World_physical;
}

G4VPhysicalVolume* DetectorConstruction::Construct()
{   
	DefineMaterials();
	
	return DefineVolumes();
}

void DetectorConstruction::SetPressure(G4double pressure)
{
	this->pressure = pressure;
}

void DetectorConstruction::SetMaterialLogFilename(G4String filename) {
	this->materialLogFilename = filename;
}

std::vector<std::string> StringSplit(std::string input, char delim = ' ') {
	std::vector<std::string> strings;
	
	std::istringstream f(input);
    std::string s;
	
    while (std::getline(f, s, delim)) {
		if (!s.empty()) strings.push_back(s);
    }
	
	return strings;
}

void DetectorConstruction::AddSourceShieldLayer(G4String shieldingConfiguration) {
	std::vector<std::string> strings = StringSplit(shieldingConfiguration, ' ');
	
	if (strings.size() < 3) {
		strings.push_back("0");
	}
	
	sourceShieldStructure.push_back(strings);
}

void DetectorConstruction::SetSourceShieldInitialOffset(G4double initialOffset) {
	this->sourceShieldInitialOffset = initialOffset;
}

void DetectorConstruction::SetDetectorSideShieldInitialXOffset(G4double initialOffset) {
	this->detectorSideShieldInitialXOffset = initialOffset;
}

void DetectorConstruction::SetDetectorSideShieldInitialYOffset(G4double initialOffset) {
	this->detectorSideShieldInitialYOffset = initialOffset;
}

void DetectorConstruction::AddDetectorSideShieldLayer(G4String shieldingConfiguration) {
	std::vector<std::string> strings = StringSplit(shieldingConfiguration, ' ');
	
	detectorSideShieldStructure.push_back(strings);
}

void DetectorConstruction::AddDetectorBackShieldLayer(G4String shieldingConfiguration) {
	std::vector<std::string> strings = StringSplit(shieldingConfiguration, ' ');
	
	detectorBackShieldStructure.push_back(strings);
}

void DetectorConstruction::SetDetectorBackShieldInitialOffset(G4double initialOffset) {
	this->detectorBackShieldInitialOffset = initialOffset;
}

void DetectorConstruction::SetDetectorBackShieldWidthDeltaX(G4double deltaX) {
	this->detectorBackShieldWidthDeltaX = deltaX;
}

void DetectorConstruction::SetDetectorBackShieldWidthDeltaY(G4double deltaY) {
	this->detectorBackShieldWidthDeltaY = deltaY;
}

void DetectorConstruction::SetDetectorSideShieldFWidthDeltaZ(G4double forwardDeltaZ) {
	this->detectorSideShieldFWidthDeltaZ = forwardDeltaZ;
}

void DetectorConstruction::SetDetectorSideShieldBWidthDeltaZ(G4double backwardDeltaZ) {
	this->detectorSideShieldBWidthDeltaZ = backwardDeltaZ;
}