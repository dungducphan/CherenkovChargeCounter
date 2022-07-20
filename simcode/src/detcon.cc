#include "detcon.hh"

detcon::detcon() : G4VUserDetectorConstruction() {}

detcon::~detcon() {}

G4SubtractionSolid *detcon::GetCasingSolid() {
    G4double RadiationLength = 200 * mm;
    G4double LightCollectionLength = 50 * mm;
    G4double CrossSectionalSize = 30 * mm;
    G4double WallThickness = 2 * mm;

    G4Box *box_0_0 = new G4Box("box_0_0",
                               CrossSectionalSize * 0.5 + WallThickness,
                               CrossSectionalSize * 0.5 + WallThickness,
                               RadiationLength * 0.5);
    G4Box *box_0_1 = new G4Box("box_0_1",
                               CrossSectionalSize * 0.5 + WallThickness,
                               LightCollectionLength * 0.5,
                               CrossSectionalSize * 0.5 + WallThickness);
    G4ThreeVector translation_0(0,
                                CrossSectionalSize * 0.5 + WallThickness +
                                LightCollectionLength * 0.5,
                                RadiationLength * 0.5 - (CrossSectionalSize * 0.5 + WallThickness));
    G4UnionSolid *union_0 = new G4UnionSolid("union_0",
                                             box_0_0,
                                             box_0_1,
                                             0,
                                             translation_0);

    G4Box *box_1_0 = new G4Box("box_1_0",
                               CrossSectionalSize * 0.5,
                               CrossSectionalSize * 0.5,
                               RadiationLength * 0.5);
    G4Box *box_1_1 = new G4Box("box_1_1",
                               CrossSectionalSize * 0.5,
                               (LightCollectionLength + WallThickness) * 0.5,
                               CrossSectionalSize * 0.5);
    G4ThreeVector translation_1(0,
                                (CrossSectionalSize + LightCollectionLength + WallThickness) * 0.5,
                                (RadiationLength - CrossSectionalSize) * 0.5 - WallThickness);
    G4UnionSolid *union_1 = new G4UnionSolid("union_1",
                                             box_1_0,
                                             box_1_1,
                                             0,
                                             translation_1);

    G4SubtractionSolid *subtractionSolid = new G4SubtractionSolid("solidCasing",
                                                                  union_0,
                                                                  union_1);

    return subtractionSolid;
}

G4VPhysicalVolume *detcon::Construct() {
    nist = G4NistManager::Instance();
    G4bool checkOverlaps = true;

    // World
    G4double worldSize = 30 * cm;
    worldMat = nist->FindOrBuildMaterial("G4_Galactic");
    solidWorld = new G4Box("solidWorld", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0,
                                  checkOverlaps);

    solidCasing = GetCasingSolid();
    logicCasing = new G4LogicalVolume(solidCasing, worldMat, "logicCasing");
    physCasing = new G4PVPlacement(0, G4ThreeVector(), logicCasing, "physCasing", logicWorld,
                                   false, 0, checkOverlaps);

    return physWorld;
}

