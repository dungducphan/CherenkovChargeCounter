#include "detcon.hh"
#include "actioninit.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "FTFP_BERT.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4OpticalPhysics.hh"

#include "TH1D.h"
#include "TFile.h"

int main(int argc, char** argv) {
  G4UIExecutive* ui = nullptr;
  if (argc == 1) ui = new G4UIExecutive(argc, argv);

  G4int precision = 4;
  G4SteppingVerbose::UseBestUnit(precision);

#ifdef G4MULTITHREADED
  G4MTRunManager* runManager = new G4MTRunManager();
#else
  G4RunManager* runManager = new G4RunManager();
#endif

  runManager->SetUserInitialization(new detcon());

  G4VModularPhysicsList* physicsList = new FTFP_BERT;
  physicsList->ReplacePhysics(new G4EmStandardPhysics_option4());
  G4OpticalPhysics* opticalPhysics = new G4OpticalPhysics();
  auto opticalParams = G4OpticalParameters::Instance();
  opticalParams->SetCerenkovMaxBetaChange(0.1);
  opticalParams->SetCerenkovMaxPhotonsPerStep(100000.);
  opticalParams->SetCerenkovTrackSecondariesFirst(true);

  physicsList->RegisterPhysics(opticalPhysics);
  runManager->SetUserInitialization(physicsList);
  runManager->SetUserInitialization(new actioninit());

  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();

  G4UImanager* UImanager = G4UImanager::GetUIpointer();
  
  if (!ui) {
    G4String command = "/control/execute ";
    G4String fileName = argv[1];
    UImanager->ApplyCommand(command+fileName);
  } else {
    UImanager->ApplyCommand("/control/execute vis.mac");
    ui->SessionStart();
    delete ui;
  }

  delete visManager;
  delete runManager;
}
