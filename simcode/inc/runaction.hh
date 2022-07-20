#pragma once 

#include "G4UserRunAction.hh"
#include "globals.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"
#include "G4Run.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"

#include "TString.h"

class G4Run;

class runAction : public G4UserRunAction {
  public:
    runAction();
    virtual ~runAction();

    virtual void BeginOfRunAction(const G4Run*);
    virtual void   EndOfRunAction(const G4Run*);

  private:
    G4AnalysisManager *man;
};
