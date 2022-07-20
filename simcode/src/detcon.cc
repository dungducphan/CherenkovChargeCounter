#include "detcon.hh"

detcon::detcon() : G4VUserDetectorConstruction() {
    RadiationLength = 200 * mm;
    LightCollectionLength = 50 * mm;
    CrossSectionalSize = 30 * mm;
    WallThickness = 2 * mm;
}

detcon::~detcon() {}

void detcon::GetRefractiveIndex(std::string filename, std::vector<G4double>& photonEnergy, std::vector<G4double>& refractiveIndex) {
    std::ifstream infile(Form("RefractiveIndex%s.csv", filename.c_str()));
    double tmp_E;
    double tmp_n;
    while (infile >> tmp_E >> tmp_n) {
        photonEnergy.push_back(tmp_E * eV);
        refractiveIndex.push_back(tmp_n);
    }
    infile.close();
}

G4UnionSolid *detcon::GetCasingMiniWorldSolid() {
    G4Box *box_2_0 = new G4Box("box_2_0",
                               CrossSectionalSize * 0.5 + WallThickness,
                               CrossSectionalSize * 0.5 + WallThickness,
                               RadiationLength * 0.5);
    G4Box *box_2_1 = new G4Box("box_2_1",
                               CrossSectionalSize * 0.5 + WallThickness,
                               LightCollectionLength * 0.5,
                               CrossSectionalSize * 0.5 + WallThickness);
    G4ThreeVector translation_0(0,
                                CrossSectionalSize * 0.5 + WallThickness +
                                LightCollectionLength * 0.5,
                                RadiationLength * 0.5 - (CrossSectionalSize * 0.5 + WallThickness));
    G4UnionSolid *union_0 = new G4UnionSolid("union_0",
                                             box_2_0,
                                             box_2_1,
                                             0,
                                             translation_0);

    return union_0;
}

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

    // Aluminum
    aluminumMat = nist->FindOrBuildMaterial("G4_Al");
    std::vector<G4double> photonEnergy_Aluminum;
    std::vector<G4double> refractiveIndex_Aluminum;
    GetRefractiveIndex("Aluminum", photonEnergy_Aluminum, refractiveIndex_Aluminum);
    G4MaterialPropertiesTable* mpt_Aluminum = new G4MaterialPropertiesTable();
    G4int numEntries_Aluminum = photonEnergy_Aluminum.size();
    std::cout << "Number of entries: " << numEntries_Aluminum << std::endl;
    mpt_Aluminum->AddProperty ("RINDEX", &photonEnergy_Aluminum[0], &refractiveIndex_Aluminum[0], numEntries_Aluminum);
    //aluminumMat->SetMaterialPropertiesTable(mpt_Aluminum);

    potassiumMat = nist->FindOrBuildMaterial("G4_K");

    // CO2
    carbonDioxideMat = nist->FindOrBuildMaterial("G4_CARBON_DIOXIDE");
    std::vector<G4double> photonEnergy_CO2;
    std::vector<G4double> refractiveIndex_CO2;
    GetRefractiveIndex("CO2", photonEnergy_CO2, refractiveIndex_CO2);
    G4MaterialPropertiesTable* mpt_CO2 = new G4MaterialPropertiesTable();
    G4int numEntries_CO2 = photonEnergy_CO2.size();
    mpt_CO2->AddProperty ("RINDEX", &photonEnergy_CO2[0], &refractiveIndex_CO2[0], numEntries_CO2);
    carbonDioxideMat->SetMaterialPropertiesTable(mpt_CO2);

    // Vacuum
    vacuumMat = nist->FindOrBuildMaterial("G4_Galactic");

    G4bool checkOverlaps = true;

    // World
    G4double worldSize_X = 10 * cm;
    G4double worldSize_Y = 15 * cm;
    G4double worldSize_Z = 30 * cm;
    solidWorld = new G4Box("solidWorld", 0.5 * worldSize_X, 0.5 * worldSize_Y, 0.5 * worldSize_Z);
    logicWorld = new G4LogicalVolume(solidWorld, vacuumMat, "logicWorld");
    physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "physWorld", 0, false, 0,
                                  checkOverlaps);

    // Casing
    solidCasingMiniWorld = GetCasingMiniWorldSolid();
    solidCasing = GetCasingSolid();
    logicCasingMiniWorld = new G4LogicalVolume(solidCasingMiniWorld, carbonDioxideMat, "logicCasingMiniWorld");
    logicCasing = new G4LogicalVolume(solidCasing, aluminumMat, "logicCasing");
    physCasingMiniWorld = new G4PVPlacement(0, G4ThreeVector(), logicCasingMiniWorld, "physCasingMiniWorld", logicWorld,
                                            false, 0, checkOverlaps);
    physCasing = new G4PVPlacement(0, G4ThreeVector(), logicCasing, "physCasing", logicCasingMiniWorld,
                                   false, 0, checkOverlaps);


    // Mirror
    solidMirror = new G4Box("solidMirror", 28 * 0.5 * mm, 40 * 0.5 * mm, 10 * 0.5 * um);
    logicMirror = new G4LogicalVolume(solidMirror, silverMat, "logicMirror");
    G4RotationMatrix* rotMat_mirror = new G4RotationMatrix (0., TMath::PiOver4(), 0.);
    G4ThreeVector mirror_pos(0., 0., RadiationLength * 0.5 - (CrossSectionalSize * 0.5 + WallThickness));
    physMirror = new G4PVPlacement(rotMat_mirror, mirror_pos, logicMirror, "physMirror", logicCasingMiniWorld, false, 0, checkOverlaps);

    // Beam window
    G4double WindowSize = CrossSectionalSize + 2 * WallThickness;
    solidWindow = new G4Box("solidWindow", WindowSize * 0.5, WindowSize * 0.5, 10 * 0.5 * um);
    logicWindow = new G4LogicalVolume(solidWindow, aluminumMat, "logicWindow");
    physWindow_1 = new G4PVPlacement(0, G4ThreeVector(0., 0., - RadiationLength * 0.5 - 5 * um), logicWindow, "physWindow_1", logicWorld, false, 0, checkOverlaps);
    physWindow_2 = new G4PVPlacement(0, G4ThreeVector(0., 0., + RadiationLength * 0.5 + 5 * um), logicWindow, "physWindow_2", logicWorld, false, 0, checkOverlaps);

    // PMT
    solidPMT = new G4Box("solidPMT", CrossSectionalSize * 0.5, 20 * 0.5 * um,CrossSectionalSize * 0.5);
    logicPMT = new G4LogicalVolume(solidPMT, potassiumMat, "logicPMT");
    physPMT = new G4PVPlacement(0,
                                G4ThreeVector(0., CrossSectionalSize * 2, RadiationLength * 0.5 - (CrossSectionalSize * 0.5 + WallThickness)),
                                logicPMT, "physPMT", logicCasingMiniWorld, false, 0, checkOverlaps);

    //-----------
    // Mirror Surfaces

    G4OpticalSurface* opSurf_Mirror = new G4OpticalSurface("MirrorOpSurface");
    opSurf_Mirror->SetType(dielectric_metal);
    opSurf_Mirror->SetFinish(polished);
    opSurf_Mirror->SetModel(unified);
    std::vector<G4double> photonEnergy_Mirror = {1 * eV, 6 * eV};
    std::vector<G4double> reflectivity_Mirror = {0.9500, 0.9500};
    std::vector<G4double> efficiency_Mirror   = {0.0500, 0.0500};
    G4MaterialPropertiesTable* mpt_Mirror = new G4MaterialPropertiesTable();
    mpt_Mirror->AddProperty("REFLECTIVITY", photonEnergy_Mirror, reflectivity_Mirror);
    mpt_Mirror->AddProperty("EFFICIENCY", photonEnergy_Mirror, efficiency_Mirror);
    opSurf_Mirror->SetMaterialPropertiesTable(mpt_Mirror);
    G4LogicalBorderSurface* surf_Mirror = new G4LogicalBorderSurface("MirrorSurface", physCasingMiniWorld, physMirror, opSurf_Mirror);
    G4OpticalSurface* opticalSurface = dynamic_cast<G4OpticalSurface*>(surf_Mirror->GetSurface(physCasingMiniWorld, physMirror)->GetSurfaceProperty());
    if (opticalSurface)
        opticalSurface->DumpInfo();
    //----------

    return physWorld;
}

