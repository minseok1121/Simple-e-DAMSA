#include "globals.hh"
#include "G4ios.hh"

#include "SteppingActionDMG4.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "DetectorConstruction.hh"

#include "DarkMatter.hh"
#include "DarkPhotons.hh"
#include "DarkScalars.hh"
#include "ALP.hh"

#include "DMParticles/DMParticle.hh"

#include "G4SteppingManager.hh"
#include "G4VTouchable.hh"
#include "G4TouchableHistory.hh"

#include "G4ProcessManager.hh"
#include "G4ProcessType.hh"
#include "Randomize.hh"

#include "G4ParticleTypes.hh"
#include "G4DynamicParticle.hh"
#include "G4EventManager.hh"
#include "G4TrackVector.hh"
#include "G4SystemOfUnits.hh"

#include "G4Electron.hh"
#include "G4Positron.hh"
#include "G4Gamma.hh"

#include "G4ParticleTypes.hh"
#include "G4DynamicParticle.hh"
#include "G4EventManager.hh"
#include "G4TrackVector.hh"

#include "G4SystemOfUnits.hh"
#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"
#include "G4AccumulableManager.hh"

SteppingActionDMG4::SteppingActionDMG4(DetectorConstruction* myDC, EventAction* myEA, G4int clusterId, G4int procId, RunAction* runAction)
: eventAction(myEA), G4UserSteppingAction(), fInitialPosition(G4ThreeVector(0., 0., 0.)),
  fClusterId(clusterId), fProcId(procId), fRunAction(runAction)
{
  eventAction->SetSteppingAction(this);
}


void SteppingActionDMG4::UserSteppingAction(const G4Step* step)
{
if(step->GetPreStepPoint()->GetGlobalTime() > 1*CLHEP::second) return;
  //그리고 내가 준 조건들이 내가 원하는대로 잘 작동하는지 trackingverbose로 확인하면서.
  if(step->GetPreStepPoint()->GetPosition().z() < -15.6*cm && step->GetPostStepPoint()->GetPosition().z() >= -15.6*cm){
    G4String particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
    //G4cout << "Infinite Wall" << G4endl;
    if(particleName == "e-") fRunAction->StoreKineticEnergy(1, step->GetPreStepPoint()->GetKineticEnergy());
    else if(particleName == "e+") fRunAction->StoreKineticEnergy(2, step->GetPreStepPoint()->GetKineticEnergy());
    else if(particleName == "gamma") fRunAction->StoreKineticEnergy(3, step->GetPreStepPoint()->GetKineticEnergy());
    else if(particleName == "neutron") fRunAction->StoreKineticEnergy(4, step->GetPreStepPoint()->GetKineticEnergy());
    else{
  G4double Energy = step->GetPreStepPoint()->GetKineticEnergy();
  G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector direction = step->GetTrack()->GetMomentumDirection();
  G4VPhysicalVolume* currentVolume = step->GetPreStepPoint()->GetTouchable()->GetVolume();
  std::ofstream outFile("LittleDamsa_"+std::to_string(fClusterId)+"_"+std::to_string(fProcId)+"_"+(currentVolume->GetName())+"_"+particleName+".txt", std::ios_base::app);
  outFile << Energy << "     " << position.x() << "     " << position.y() << "     " << position.z() 
      << "     " << direction.x() << "     " << direction.y() << "     " << direction.z() << "     " << step->GetPreStepPoint()->GetGlobalTime() << "     " << eventAction->GetEventID() << "     " << std::endl;
      //outFile << particleName << " production at E = " << Energy << G4endl;
        outFile.close();
    }
  }

if(!(step->IsFirstStepInVolume())) return;

  G4Track* track = step->GetTrack();
  G4String particleName = track->GetParticleDefinition()->GetParticleName();
  G4VPhysicalVolume* currentVolume = step->GetPreStepPoint()->GetTouchable()->GetVolume();

  if(currentVolume->GetName() == "target" && particleName == "gamma"){
    fRunAction->StoreKineticEnergy(0, step->GetPreStepPoint()->GetKineticEnergy());
    //G4cout << "I'm produced  " << step->GetPreStepPoint()->GetKineticEnergy() << G4endl;
  }

if(currentVolume->GetName() == "ECAL"){
    //fRunAction->StoreKineticEnergy(0, step->GetPreStepPoint()->GetKineticEnergy());
    //G4cout << "In ECAL" << G4endl;
  G4double Energy = step->GetPreStepPoint()->GetKineticEnergy();
  G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector direction = track->GetMomentumDirection();
  std::ofstream outFile("LittleDamsa_"+std::to_string(fClusterId)+"_"+std::to_string(fProcId)+"_"+(currentVolume->GetName())+"_"+particleName+".txt", std::ios_base::app);
  outFile << Energy << "     " << position.x() << "     " << position.y() << "     " << position.z() 
      << "     " << direction.x() << "     " << direction.y() << "     " << direction.z() << "     " << step->GetPreStepPoint()->GetGlobalTime() << "     " << eventAction->GetEventID() << "     " << std::endl;
      //outFile << particleName << " production at E = " << Energy << G4endl;
        outFile.close();
  }
}


void SteppingActionDMG4::Reset()
{
  eventAction->GetDarkMatterPointer()->ResetNEmissions();
}


void SteppingActionDMG4::Finalize()
{
}