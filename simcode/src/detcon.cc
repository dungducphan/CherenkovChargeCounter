#include "detcon.hh"

detcon::detcon() : G4VUserDetectorConstruction() {
    RadiationLength = 200 * mm;
    LightCollectionLength = 50 * mm;
    CrossSectionalSize = 30 * mm;
    WallThickness = 2 * mm;
}

detcon::~detcon() {}

G4SubtractionSolid *detcon::GetCasingSolid() {
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
    silverMat = nist->FindOrBuildMaterial("G4_Ag");
    aluminumMat = nist->FindOrBuildMaterial("G4_Al");
    potassiumMat = nist->FindOrBuildMaterial("G4_K");
    worldMat = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");

    G4bool checkOverlaps = true;

    // World
    G4double worldSize = 30 * cm;
    solidWorld = new G4Box("solidWorld", 0.5 * worldSize, 0.5 * worldSize, 0.5 * worldSize);
    logicWorld = new G4LogicalVolume(solidWorld, worldMat, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0,
                                  checkOverlaps);

    // Casing
    solidCasing = GetCasingSolid();
    logicCasing = new G4LogicalVolume(solidCasing, aluminumMat, "logicCasing");
    physCasing = new G4PVPlacement(0, G4ThreeVector(), logicCasing, "physCasing", logicWorld,
                                   false, 0, checkOverlaps);

    // Mirror
    solidMirror = new G4Box("solidMirror", 28 * 0.5 * mm, 40 * 0.5 * mm, 10 * 0.5 * um);
    logicMirror = new G4LogicalVolume(solidMirror, silverMat, "logicMirror");
    G4RotationMatrix* rotMat_mirror = new G4RotationMatrix (0., TMath::PiOver4(), 0.);
    G4ThreeVector mirror_pos(0., 0., RadiationLength * 0.5 - (CrossSectionalSize * 0.5 + WallThickness));
    physMirror = new G4PVPlacement(rotMat_mirror, mirror_pos, logicMirror, "physMirror", logicWorld, false, 0, checkOverlaps);

    // Beam window
    G4double WindowSize = CrossSectionalSize + 2 * WallThickness;
    solidWindow = new G4Box("solidWindow", WindowSize * 0.5, WindowSize * 0.5, 10 * 0.5 * um);
    logicWindow = new G4LogicalVolume(solidWindow, aluminumMat, "logicWindow");
    physWindow_1 = new G4PVPlacement(0, G4ThreeVector(0., 0., - RadiationLength * 0.5 - 5 * um), logicWindow, "physWindow_1", logicWorld, false, 0, checkOverlaps);
    physWindow_2 = new G4PVPlacement(0, G4ThreeVector(0., 0., + RadiationLength * 0.5 + 5 * um), logicWindow, "physWindow_2", logicWorld, false, 0, checkOverlaps);


    // PMT
    solidPMT = new G4Box("solidPMT", CrossSectionalSize * 0.5, 20 * 0.5 * um,CrossSectionalSize * 0.5);
    logicPMT = new G4LogicalVolume(solidPMT, potassiumMat, "logicPMT");
    physPMT = new G4PVPlacement(0, G4ThreeVector(0., CrossSectionalSize * 2, RadiationLength * 0.5 - (CrossSectionalSize * 0.5 + WallThickness)), logicPMT, "physPMT", logicWorld, false, 0, checkOverlaps);

    return physWorld;
}

