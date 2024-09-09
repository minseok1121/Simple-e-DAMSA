#include "PrimaryGeneratorAction.hh"

#include "DetectorConstruction.hh"

#include "G4Event.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"

#include "G4ParticleGun.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* myDC)
: myDetector(myDC)
{
  
  particleGun = new G4ParticleGun(1);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  
  // /*
  G4ParticleDefinition* electron = particleTable->FindParticle("e-");
  particleGun->SetParticleDefinition(electron);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(300*MeV);
  // */
  /*
  G4ParticleDefinition* gamma = particleTable->FindParticle("gamma");
  particleGun->SetParticleDefinition(gamma);
  particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));
  particleGun->SetParticleEnergy(10*MeV);
  */
}


PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
  delete particleGun;
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
  particleGun->SetParticlePosition(G4ThreeVector(0,0,-26*cm));
  //particleGun->SetParticlePosition(G4ThreeVector(0,0,0));
  particleGun->GeneratePrimaryVertex(anEvent);
}
