#include "pmtsd.hh"

PMTSD::PMTSD(const G4String &name) : G4VSensitiveDetector(name) {
    G4AnalysisManager* man = G4AnalysisManager::Instance();
    histID = man->GetH1Id("Wavelength");
}

PMTSD::~PMTSD() {}

G4bool PMTSD::ProcessHits(G4Step *aStep, G4TouchableHistory *) {
    G4Track *track = aStep->GetTrack();

    if (track->GetDefinition() != G4ParticleTable::GetParticleTable()->FindParticle("opticalphoton")) {
        return false;
    }

    auto prestep = aStep->GetPreStepPoint();


    G4AnalysisManager* man = G4AnalysisManager::Instance();
    man->FillH1(histID, 1239.8 / (prestep->GetTotalEnergy() / eV));
    // std::cout << "A hit:" << 1239.8 / (prestep->GetTotalEnergy() / eV) << std::endl;

    return true;
}