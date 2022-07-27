#include "generator.hh"

generator::generator() {
  fGeneralParticleSource = new G4GeneralParticleSource();

  G4SingleParticleSource *sps = fGeneralParticleSource->GetCurrentSource();
  G4ParticleDefinition *particle = G4ParticleTable::GetParticleTable()->FindParticle("e-");

  sps->SetParticleDefinition(particle);
  sps->SetNumberOfParticles(5000);

  sps->GetPosDist()->SetPosDisType("Point"); // Point, Beam, Plane, Surface, Volume
  sps->GetPosDist()->SetCentreCoords(G4ThreeVector(0., 0., -13 * cm));
  sps->GetPosDist()->ConfineSourceToVolume("NULL");

  sps->GetAngDist()->SetAngDistType("iso"); // Isotropic, Cosine-law, Beam, User-defined
  sps->GetAngDist()->SetMinTheta(TMath::Pi() * rad);
  sps->GetAngDist()->SetMaxTheta(TMath::Pi() - (TMath::Pi() / 1000.) * rad);
  sps->GetAngDist()->SetMinPhi(0 * rad);
  sps->GetAngDist()->SetMaxPhi(TMath::Pi() * 2. * rad);

  sps->GetEneDist()->SetEnergyDisType("Mono");// Mono, Lin, Pow, Exp, Gaus, Brem, BBody, Cdg (cosmic diffuse gamma), User, Arb, Epn (energy per nucleon)
  sps->GetEneDist()->SetMonoEnergy(2 * GeV);
}

generator::~generator() {
  delete fGeneralParticleSource;
}

void generator::GeneratePrimaries(G4Event *anEvent) {
  fGeneralParticleSource->GeneratePrimaryVertex(anEvent);
}
