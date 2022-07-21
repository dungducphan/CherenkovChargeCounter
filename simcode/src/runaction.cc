#include "runaction.hh"

runAction::runAction() : G4UserRunAction() {
  man = G4AnalysisManager::Instance();
  man->CreateH1("Wavelength", "Wavelength", 1600, 100, 900);
  // FIXME:
  // man->CreateH2("Image", ";X (cellID);Y (cellID)", 60, 0, 60, 60, 0, 60);
}

runAction::~runAction() {}

void runAction::BeginOfRunAction(const G4Run* run) {
  man = G4AnalysisManager::Instance();
  man->OpenFile(Form("Ana_R%05i.root", (int) run->GetRunID()));
}

void runAction::EndOfRunAction(const G4Run*) {
  man = G4AnalysisManager::Instance();
  man->Write();
  man->CloseFile();
}

