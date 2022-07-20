#include "detcon.hh"
#include "actioninit.hh"

#ifdef G4MULTITHREADED
#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "Shielding.hh"
#include "G4SteppingVerbose.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

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
  runManager->SetUserInitialization(new Shielding());
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
