#pragma once

#include <utility>
#include <tuple>
#include <fstream>

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Tubs.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4SDManager.hh"
#include "G4UnionSolid.hh"
#include "G4SubtractionSolid.hh"
#include "G4IntersectionSolid.hh"

#include "TMath.h"
#include "TString.h"

#include "pmtsd.hh"

class G4VPhysicalVolume;

class G4LogicalVolume;

class detcon : public G4VUserDetectorConstruction {
public:
    detcon();

    virtual ~detcon();

    virtual G4VPhysicalVolume *Construct();
    void ConstructSDandField();

    void GetRefractiveIndex(std::string filename, std::vector<G4double>& photonEnergy, std::vector<G4double>& refractiveIndex);
    void GetQuantumEfficiency(std::vector<G4double>& photonEnergy, std::vector<G4double>& reflectivity, std::vector<G4double>& quantumEfficiency);

    G4SubtractionSolid *GetCasingSolid();
    G4UnionSolid *GetCasingMiniWorldSolid();
    G4double RadiationLength;
    G4double LightCollectionLength;
    G4double CrossSectionalSize;
    G4double WallThickness;

    G4NistManager *nist;
    G4Material *vacuumMat;
    G4Material *carbonDioxideMat;
    G4Material *silverMat;
    G4Material *aluminumMat;
    G4Material *potassiumMat;

    G4Box *solidWorld;
    G4LogicalVolume *logicWorld;
    G4VPhysicalVolume *physWorld;

    G4SubtractionSolid *solidCasing;
    G4UnionSolid *solidCasingMiniWorld;
    G4LogicalVolume *logicCasing;
    G4LogicalVolume *logicCasingMiniWorld;
    G4VPhysicalVolume *physCasing;
    G4VPhysicalVolume *physCasingMiniWorld;

    G4Box *solidMirror;
    G4LogicalVolume *logicMirror;
    G4VPhysicalVolume *physMirror;
    G4LogicalSkinSurface *logicalSkinSurfaceMirror;

    G4Box *solidWindow;
    G4LogicalVolume *logicWindow;
    G4VPhysicalVolume *physWindow_1;
    G4VPhysicalVolume *physWindow_2;

    G4Box *solidPMT;
    G4LogicalVolume *logicPMT;
    G4VPhysicalVolume *physPMT;
};
