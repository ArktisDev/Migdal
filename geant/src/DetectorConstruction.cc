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
#include "G4UnitsTable.hh"
#include "G4MaterialTable.hh"
#include "G4SubtractionSolid.hh"
#include "G4Tubs.hh"

#include "DetectorMessenger.hh"

#include <iostream>

DetectorConstruction::DetectorConstruction()
: detectorMessenger(new DetectorMessenger(this)), detectorRegion(0), shieldingRegion(0), pressure(75), windowRadius(1 * inch),
  sourceShieldInitialOffset(1 * cm), detectorSideShieldInitialXOffset(1 * cm), detectorSideShieldInitialYOffset(1 * cm),
  detectorSideShieldFWidthDeltaZ(0 * cm), detectorSideShieldBWidthDeltaZ(0 * cm),
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
	
	density = 11.29 * g / cm3;
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
	
	density = 1.04 * g / cm3;
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
	
	G4Color copperColor(0.72, 0.41, 0.20);
	
	if (detectorRegion) delete detectorRegion;
	if (shieldingRegion) delete shieldingRegion;
	
	detectorRegion = new G4Region("DetectorRegion");
	shieldingRegion = new G4Region("ShieldRegion");
	
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
	
	// Detector has interior volume of (9 in)^3, walls are 1 inch thick
	G4double 			detector_hx 		= 4.5 * inch;
	G4double 			detector_hy 		= 4.5 * inch;
	G4double 			detector_hz 		= 4.5 * inch;
	this->detector_hx 						= detector_hx;
	this->detector_hy 						= detector_hy;
	this->detector_hz 						= detector_hz;
	G4Material* 		detector_mat 		= CF4;
	G4VisAttributes*    detector_visAttr  	= new G4VisAttributes(G4Color::Cyan());

    G4Box*	            Detector_solid		= new G4Box("Detector_sol", detector_hx, detector_hy, detector_hz);
	G4LogicalVolume*	Detector_logical  	= new G4LogicalVolume(Detector_solid, detector_mat, "Detector_log");
	G4VPhysicalVolume*  Detector_physical 	= new G4PVPlacement(0, G4ThreeVector(0, 0, 0 * cm), Detector_logical, "Detector_phys", World_logical, false, 0, checkOverlaps);
	Detector_logical->SetVisAttributes(detector_visAttr);
	
	G4double 			wireplane_hx 		= 5.5 * cm;
	G4double 			wireplane_hy 		= 0.5 * cm;
	G4double 			wireplane_hz 		= 5.5 * cm;
	G4Material* 		wireplane_mat 		= copper;
	G4VisAttributes*    wireplane_visAttr  	= new G4VisAttributes(copperColor);
	G4ThreeVector       wireplane_pos       = G4ThreeVector(0, -0.4 * detector_hy, 0);
	
	G4Box*	            WirePlane_solid 	= new G4Box("WirePlane_sol", wireplane_hx, wireplane_hy, wireplane_hz);
	G4LogicalVolume*	WirePlane_logical	= new G4LogicalVolume(WirePlane_solid, wireplane_mat, "WirePlane_log");
	G4VPhysicalVolume*  Wireplane_physical	= new G4PVPlacement(0, wireplane_pos, WirePlane_logical, "WirePlane_phys", Detector_logical, false, 0, checkOverlaps);
    WirePlane_logical->SetVisAttributes(wireplane_visAttr);
	
	G4double 			fieldCage_squareRadius	= 5.5 * cm;
	G4double			fieldCage_torusRadius	= 0.08 * cm; // 16 AWG
	G4double 			fieldCage_spacing		= 1 * cm;
	G4int				fieldCage_levels		= 9;
	G4double			fieldCage_initialY		= wireplane_pos.y() + 1 * cm;
	G4Material* 		fieldCage_mat 			= copper;
	G4VisAttributes*    fieldCage_visAttr  		= new G4VisAttributes(copperColor);
	
	G4Box*				fieldCage_solid1		= new G4Box("FieldCage_sol1", fieldCage_squareRadius + fieldCage_torusRadius, fieldCage_torusRadius, fieldCage_squareRadius + fieldCage_torusRadius);
	G4Box*				fieldCage_solid2		= new G4Box("FieldCage_sol2", fieldCage_squareRadius - fieldCage_torusRadius, 2 * fieldCage_torusRadius, fieldCage_squareRadius - fieldCage_torusRadius);
	G4VSolid*			fieldCage_solid3		= new G4SubtractionSolid("FieldCage_sol3", fieldCage_solid1, fieldCage_solid2);
	G4LogicalVolume*	fieldCage_logical		= new G4LogicalVolume(fieldCage_solid3, fieldCage_mat, "FieldCage_log");
	fieldCage_logical->SetVisAttributes(fieldCage_visAttr);
	
	for (int i = 0; i < fieldCage_levels; i++) {
		G4ThreeVector fieldCage_pos = G4ThreeVector(0, fieldCage_initialY + fieldCage_spacing * i, 0);
		
		G4VPhysicalVolume*  fieldCage_physical = new G4PVPlacement(0, fieldCage_pos, fieldCage_logical, "FieldCage_phys", Detector_logical, true, i, checkOverlaps);
	}

	this->fieldCageCorner1 = G4ThreeVector(-wireplane_hx, wireplane_pos.y() + wireplane_hy, -wireplane_hz);
	this->fieldCageCorner2 = G4ThreeVector(wireplane_hx, fieldCage_initialY + (fieldCage_levels - 1) * fieldCage_spacing, wireplane_hz);

	G4double 			fieldCage_holder_hx		= 0.5 * cm;
	G4double 			fieldCage_holder_hy		= ((fieldCage_levels - 0.75) * fieldCage_spacing + fieldCage_initialY - wireplane_pos.y()) / 2;
	G4double			fieldCage_holder_hz		= 1.5 * cm;
	G4Material*			fieldCageHolder_mat		= G4Material::GetMaterial("G4_POLYETHYLENE");
	G4VisAttributes*    fieldCageHolder_visAttr = new G4VisAttributes(G4Color(0.4, 0.4, 0.4));

	G4Box*				fieldCageHolder_solid 	= new G4Box("FieldCageHolder_sol1", fieldCage_holder_hx, fieldCage_holder_hy, fieldCage_holder_hz);
	G4LogicalVolume*	fieldCageHolder_logical	= new G4LogicalVolume(fieldCageHolder_solid, fieldCageHolder_mat, "FieldCageHolder_log");
	G4VPhysicalVolume*  fieldcageHolder_phys1	= new G4PVPlacement(0, G4ThreeVector(fieldCage_squareRadius + fieldCage_holder_hx + fieldCage_torusRadius, wireplane_pos.y() + fieldCage_holder_hy, 0), fieldCageHolder_logical, "FieldCageHolder_phys", Detector_logical, true, 0, checkOverlaps);
	G4VPhysicalVolume*  fieldcageHolder_phys2	= new G4PVPlacement(0, G4ThreeVector(-(fieldCage_squareRadius + fieldCage_holder_hx + fieldCage_torusRadius), wireplane_pos.y() + fieldCage_holder_hy, 0), fieldCageHolder_logical, "FieldCageHolder_phys", Detector_logical, true, 1, checkOverlaps);
	fieldCageHolder_logical->SetVisAttributes(fieldCageHolder_visAttr);


	G4double			detectorCase_thickX 	= 1 * inch;
	G4double			detectorCase_thickY 	= 1 * inch;
	G4double			detectorCase_thickZ 	= 1 * inch;
	G4Material* 		detectorCase_mat 		= aluminum;
	G4VisAttributes*    detectorCase_visAttr	= new G4VisAttributes(G4Color::Gray());
	
	G4double            window_Radius			= windowRadius;
	G4double			window_Thickness			= 1 * mm;
	G4Material* 		window_mat 				= aluminum;
	G4VisAttributes*    window_visAttr 			= new G4VisAttributes(G4Color(0.1,0.2,0.5));
	
	G4Box*	            DetectorCase0_solid    	= new G4Box("DetectorCase0_sol", detector_hx + detectorCase_thickX, detector_hy + detectorCase_thickY, detector_hz + detectorCase_thickZ); //box
	G4VSolid*           DetectorCase1_solid    	= new G4SubtractionSolid("DetectorCase1_sol", DetectorCase0_solid, Detector_solid, 0, G4ThreeVector(0, 0, 0)); //minus detector
    G4Tubs*             DetectorCase2_solid    	= new G4Tubs("DetectorCase2_sol", 0, window_Radius, 2 * detectorCase_thickZ, 0, 360 * degree); //window cut out
    G4VSolid*           DetectorCase3_solid    	= new G4SubtractionSolid("DetectorCase3_sol", DetectorCase1_solid, DetectorCase2_solid, 0, G4ThreeVector(0, 0, detector_hz + 0.5 * detectorCase_thickZ)); //front window
    G4VSolid*           DetectorCase4_solid    	= new G4SubtractionSolid("DetectorCase4_sol", DetectorCase3_solid, DetectorCase2_solid, 0, G4ThreeVector(0, 0, -detector_hz - 0.5 * detectorCase_thickZ)); //back window
    G4LogicalVolume*	DetectorCase_logical   	= new G4LogicalVolume(DetectorCase4_solid, detectorCase_mat, "DetectorCase_log");
    G4VPhysicalVolume*  DetectorCase_physical  	= new G4PVPlacement(0, G4ThreeVector(0, 0, 0), DetectorCase_logical, "DetectorCase_phys", World_logical, false, 0, checkOverlaps);
	DetectorCase_logical->SetVisAttributes(detectorCase_visAttr);
    G4Tubs*             Window_solid      		= new G4Tubs("DetectorCaseWindow_sol", 0, window_Radius, 0.5 * window_Thickness, 0, 360 * degree); //window
    G4LogicalVolume*	Window_logical    		= new G4LogicalVolume(Window_solid, window_mat, "DetectorCaseWindow_sol");
    G4VPhysicalVolume*  WindowF_physical  		= new G4PVPlacement(0, G4ThreeVector(0, 0, detector_hz + 0.5 * window_Thickness), Window_logical, "DetectorCaseWindowF_phys", World_logical, false, 0, checkOverlaps);
    G4VPhysicalVolume*  WindowB_physical  		= new G4PVPlacement(0, G4ThreeVector(0, 0, -detector_hz - 0.5 * window_Thickness), Window_logical, "DetectorCaseWindowB_phys", World_logical, false, 0, checkOverlaps);
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
	G4VisAttributes*    boratedPoly_visAttr = new G4VisAttributes(G4Color(0.004, 0.19, 0.125));
	
	G4double total_offset = detector_hz + motherboard_dist + motherboard_shortSide + detectorSideShieldFWidthDeltaZ + sourceShieldInitialOffset;
	
	for (const std::vector<std::string>& layer : sourceShieldStructure) {
		G4double thickness = std::stod(layer[0]) * cm;
		G4double innerRadius = std::stod(layer[2]) * cm;
		G4double radius = std::stod(layer[3]) * cm;
		G4double spacing = std::stod(layer[4]) * cm;
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
	
	for (const std::vector<std::string>& layer : detectorSideShieldStructure) {
		G4double thickness = std::stod(layer[0]) * cm;
		G4double spacing = std::stod(layer[2]) * cm;
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
	
	for (const std::vector<std::string>& layer : detectorBackShieldStructure) {
		G4double thickness = std::stod(layer[0]) * cm;
		G4double innerRadius = std::stod(layer[2]) * cm; // for cutout in middle
		G4double spacing = std::stod(layer[3]) * cm;
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
		
		G4VSolid* DetectorBackShield_solid3;
		// Now to construct the shielding as plates
		G4Box* 			DetectorBackShield_solid1 = new G4Box("DetectorBackShield_sol1", detectorBackShieldHx, detectorBackShieldHy, thickness / 2);
		if (innerRadius > 0) {
			G4Tubs*			DetectorBackShield_solid2 = new G4Tubs("DetectorBackShield_sol2", 0, innerRadius, thickness / 2 + 1 * cm, 0, 360 * degree);
			DetectorBackShield_solid3 				  = new G4SubtractionSolid("DetectorBackShield_sol3", DetectorBackShield_solid1, DetectorBackShield_solid2);
		} else {
			DetectorBackShield_solid3 = DetectorBackShield_solid1;
		}
		G4LogicalVolume* DetectorBackShield_logical = new G4LogicalVolume(DetectorBackShield_solid3, shield_mat, "DetectorBackShield_logical");
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
	
	// Thickness
	if (strings.size() < 1) {
		strings.push_back("1");
	}
	
	// Material
	if (strings.size() < 2) {
		strings.push_back("Poly");	
	}
	
	// Inner hole size
	if (strings.size() < 3) {
		strings.push_back("0");
	}
	
	// Disk radius
	if (strings.size() < 4) {
		strings.push_back("50");
	}
	
	// Spacing
	if (strings.size() < 5) {
		strings.push_back("1");
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
	
	// Thickness
	if (strings.size() < 1) {
		strings.push_back("1");
	}
	
	// Material
	if (strings.size() < 2) {
		strings.push_back("Poly");
	}
	
	// Spacing
	if (strings.size() < 3) {
		strings.push_back("1");
	}
	
	detectorSideShieldStructure.push_back(strings);
}

void DetectorConstruction::AddDetectorBackShieldLayer(G4String shieldingConfiguration) {
	std::vector<std::string> strings = StringSplit(shieldingConfiguration, ' ');
	
	// Thickness
	if (strings.size() < 1) {
		strings.push_back("1");
	}
	
	// Material
	if (strings.size() < 2) {
		strings.push_back("Poly");
	}
	
	// Cutout in middle
	if (strings.size() < 3) {
		strings.push_back("0");
	}
	
	// Spacing
	if (strings.size() < 4) {
		strings.push_back("1");
	}
	
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

void DetectorConstruction::SetWindowRadius(G4double radius) {
	this->windowRadius = radius;
}