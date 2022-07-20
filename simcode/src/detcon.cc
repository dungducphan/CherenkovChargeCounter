#include "detcon.hh"

detcon::detcon() : G4VUserDetectorConstruction() {}

detcon::~detcon() {}

G4VPhysicalVolume* detcon::Construct() {
  G4NistManager* nist = G4NistManager::Instance();
  G4bool checkOverlaps = true;

  // World
  G4double worldSize = 5*m;
  G4Material* worldMat = nist->FindOrBuildMaterial("G4_Galactic");
  G4Box* solidWorld = new G4Box("solidWorld", 0.5*worldSize, 0.5*worldSize, 0.5*worldSize);
  G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
  G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0, checkOverlaps);        

  return physWorld;
}

