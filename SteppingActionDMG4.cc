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
#include "G4Event.hh"
#include "G4Step.hh"

SteppingActionDMG4::SteppingActionDMG4(DetectorConstruction* myDC, EventAction* myEA, G4int clusterId, G4int procId, RunAction* runAction)
: eventAction(myEA), G4UserSteppingAction(), fInitialPosition(G4ThreeVector(0., 0., 0.)),
  fClusterId(clusterId), fProcId(procId), fRunAction(runAction)
{
  eventAction->SetSteppingAction(this);
}


void SteppingActionDMG4::UserSteppingAction(const G4Step* step)
{
if(step->GetPreStepPoint()->GetGlobalTime() > 1*CLHEP::second) return;
//G4cout << step->GetPreStepPoint()->GetGlobalTime() << G4endl;
  //그리고 내가 준 조건들이 내가 원하는대로 잘 작동하는지 trackingverbose로 확인하면서.

  if(step->GetPreStepPoint()->GetPosition().z() < -15.6*cm && step->GetPostStepPoint()->GetPosition().z() >= -15.6*cm){
    G4String particleName = step->GetTrack()->GetParticleDefinition()->GetParticleName();
    //G4cout << "Infinite Wall" << G4endl;
/*
    auto analysisManager = G4AnalysisManager::Instance();
// 입자 이름에 따라 각기 다른 Ntuple에 데이터 기록
if (particleName == "e-") {
    analysisManager->FillNtupleDColumn(1, 0, step->GetPreStepPoint()->GetKineticEnergy());
    analysisManager->AddNtupleRow(1);  // ElectronData Ntuple에 데이터 추가
}
else if (particleName == "e+") {
    analysisManager->FillNtupleDColumn(2, 0, step->GetPreStepPoint()->GetKineticEnergy());
    analysisManager->AddNtupleRow(2);  // PositronData Ntuple에 데이터 추가
}
else if (particleName == "gamma") {
    analysisManager->FillNtupleDColumn(3, 0, step->GetPreStepPoint()->GetKineticEnergy());
    analysisManager->AddNtupleRow(3);  // GammaEnergyData Ntuple에 데이터 추가
}
else if (particleName == "neutron") {
    analysisManager->FillNtupleDColumn(4, 0, step->GetPreStepPoint()->GetKineticEnergy());
    analysisManager->AddNtupleRow(4);  // NeutronData Ntuple에 데이터 추가
    //cout << step->GetPreStepPoint()->GetKineticEnergy() << endl;
}
else if (particleName == "DMParticleALP") {
    analysisManager->FillNtupleDColumn(5, 0, step->GetPreStepPoint()->GetKineticEnergy());
    analysisManager->AddNtupleRow(5);  // ALPData Ntuple에 데이터 추가
}
*/
    if(particleName == "e-") fRunAction->StoreKineticEnergy(1, step->GetPreStepPoint()->GetKineticEnergy());
    else if(particleName == "e+") fRunAction->StoreKineticEnergy(2, step->GetPreStepPoint()->GetKineticEnergy());
    else if(particleName == "gamma") fRunAction->StoreKineticEnergy(3, step->GetPreStepPoint()->GetKineticEnergy());
    else if(particleName == "neutron") fRunAction->StoreKineticEnergy(4, step->GetPreStepPoint()->GetKineticEnergy());
    else if(particleName == "DMParticleALP") fRunAction->StoreKineticEnergy(5, step->GetPreStepPoint()->GetKineticEnergy());
    /*
      G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  G4double Energy = step->GetPreStepPoint()->GetKineticEnergy();
  G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector direction = step->GetTrack()->GetMomentumDirection();
  G4VPhysicalVolume* currentVolume = step->GetPreStepPoint()->GetTouchable()->GetVolume();
  std::ofstream outFile("LittleDamsa_"+std::to_string(fClusterId)+"_"+std::to_string(fProcId)+"_stage1_"+particleName+".txt", std::ios_base::app);
  outFile << step->GetTrack()->GetCreatorProcess()->GetProcessName() << "   " << Energy << "     " << position.x() << "     " << position.y() << "     " << position.z() 
      << "     " << direction.x() << "     " << direction.y() << "     " << direction.z() << "     " << step->GetPreStepPoint()->GetGlobalTime() << "     " << eventID << "     " << std::endl;
      //outFile << particleName << " production at E = " << Energy << G4endl;
        outFile.close();
        */
  /*
        // Get particle name and kinetic energy
        G4String particleName = step->GetTrack()->GetDefinition()->GetParticleName();
      std::string filename = "Little";
    if(particleName == "e-") filename = "Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "Stage1_e-.txt";
    else if(particleName == "e+") filename = "Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "Stage1_e+.txt";
    else if(particleName == "gamma") filename = "Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "Stage1_gamma.txt";
    else if(particleName == "neutron") filename = "Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "Stage1_neutron.txt";
    std::ifstream inputFile(filename);
    std::vector<std::pair<int, int>> energyData;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        int bin, count;
        if (iss >> bin >> count) {
            energyData.push_back({bin, count});
        }
    }
    inputFile.close();
    G4int energyBin = static_cast<G4int>(std::floor(step->GetPreStepPoint()->GetKineticEnergy())); 
      energyData[energyBin].second += 1;
    std::ofstream outputFile(filename);
    for (const auto& entry : energyData) {
        outputFile << entry.first << " " << entry.second << "\n";
    }
    outputFile.close();
    */
  }

if(!(step->IsFirstStepInVolume())) return;

  G4Track* track = step->GetTrack();
  G4String particleName = track->GetParticleDefinition()->GetParticleName();
  G4VPhysicalVolume* currentVolume = step->GetPreStepPoint()->GetTouchable()->GetVolume();
if(currentVolume->GetName() == "target" && particleName == "DMParticleALP"){
  /*
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(6, 0, step->GetPreStepPoint()->GetKineticEnergy());
    analysisManager->AddNtupleRow(6);
    */
    fRunAction->StoreKineticEnergy(6, step->GetPreStepPoint()->GetKineticEnergy());
}
  if(currentVolume->GetName() == "target" && particleName == "gamma"){
    /*
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, step->GetPreStepPoint()->GetKineticEnergy());
    analysisManager->AddNtupleRow();
    */
    fRunAction->StoreKineticEnergy(0, step->GetPreStepPoint()->GetKineticEnergy());
    //G4cout << "I'm produced  " << step->GetPreStepPoint()->GetKineticEnergy() << G4endl;
    /*
    std::string filename = "Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "GammaProductions.txt";
    std::ifstream inputFile(filename);
    std::vector<std::pair<int, int>> energyData;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        int bin, count;
        if (iss >> bin >> count) {
            energyData.push_back({bin, count});
        }
    }
    inputFile.close();
    G4int energyBin = static_cast<G4int>(std::floor(step->GetPreStepPoint()->GetKineticEnergy())); // 0 ~ 299 범위로 변환
        energyData[energyBin].second += 1;
    std::ofstream outputFile(filename);
    for (const auto& entry : energyData) {
        outputFile << entry.first << " " << entry.second << "\n";
    }
    outputFile.close();
    */
  }
/*
  if(track->GetTrackID() < fRunAction->GetNElectrons()+1 && track->GetTrackID() > -1 && currentVolume->GetName() == "target"){
    G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
      if( (static_cast<int>(std::floor((position.x() + 0.002) /0.00004)) > 0) && (static_cast<int>(std::floor((position.x() + 0.002) /0.00004)) < 100) && (static_cast<int>(std::floor((position.y() + 0.002) /0.00004)) > 0) && (static_cast<int>(std::floor((position.y() + 0.002) /0.00004)) < 100) && static_cast<G4int>(std::floor(step->GetPreStepPoint()->GetKineticEnergy())) == 300) fRunAction->StorePos(static_cast<int>(std::floor((position.x() + 0.002) /0.00004)),static_cast<int>(std::floor((position.y() + 0.002) /0.00004)));
  else{
  G4double Energy = step->GetPreStepPoint()->GetKineticEnergy();
  G4ThreeVector direction = step->GetTrack()->GetMomentumDirection();
  std::ofstream outFile("LittleDamsa_"+std::to_string(fClusterId)+"_"+std::to_string(fProcId)+"_beam_"+particleName+".txt", std::ios_base::app);
  outFile << Energy << "     " << position.x() << "     " << position.y() << "     " << position.z() 
      << "     " << direction.x() << "     " << direction.y() << "     " << direction.z() << "     " << step->GetPreStepPoint()->GetGlobalTime() << G4endl;
      outFile.close();
  //G4cout << 1*micrometer << "   " << position.x() << "   " << static_cast<int>(std::floor((position.x() + 0.002) /0.00004)) << "   " << position.y() << "   " << static_cast<int>(std::floor((position.y() + 0.002) /0.00004)) << G4endl;
  }
  }
    /*
    if(track->GetTrackID() % 10 == 0) G4cout << track->GetTrackID() << "   ";
    if(track->GetTrackID() % 100 == 0) G4cout << track->GetTrackID() << "   ";
    if(track->GetTrackID() % 1000 == 0) G4cout << track->GetTrackID() << "   ";
  if( (static_cast<int>(std::floor((position.x() + 0.002) /0.00004)) > 0) && (static_cast<int>(std::floor((position.x() + 0.002) /0.00004)) < 100) && (static_cast<int>(std::floor((position.y() + 0.002) /0.00004)) > 0) && (static_cast<int>(std::floor((position.y() + 0.002) /0.00004)) < 100) && static_cast<G4int>(std::floor(step->GetPreStepPoint()->GetKineticEnergy())) == 300){
     // Define the file path
     int x = static_cast<int>(std::floor((position.x() + 0.002) /0.00004));
     int y = static_cast<int>(std::floor((position.y() + 0.002) /0.00004));
  std::string filename = "Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "Beam_e-.txt";
std::ifstream inputFile(filename);
    std::vector<std::pair<int, int>> XYData;
    std::string line;
    while (std::getline(inputFile, line)) {
        std::istringstream iss(line);
        int bin, count;
        if (iss >> bin >> count) {
            XYData.push_back({bin, count});
        }
    }
    inputFile.close();
        XYData[x+100*y].second += 1;
    std::ofstream outputFile(filename);
    for (const auto& entry : XYData) {
        outputFile << entry.first << " " << entry.second << "\n";
    }
    outputFile.close();
  }
  */

if(currentVolume->GetName() == "ECAL"){
  G4int eventID = G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
    //fRunAction->StoreKineticEnergy(0, step->GetPreStepPoint()->GetKineticEnergy());
    //G4cout << "In ECAL" << G4endl;
  G4double Energy = step->GetPreStepPoint()->GetKineticEnergy();
  G4ThreeVector position = step->GetPreStepPoint()->GetPosition();
  G4ThreeVector direction = track->GetMomentumDirection();
  std::ofstream outFile("LittleDamsa_"+std::to_string(fClusterId)+"_"+std::to_string(fProcId)+"_stage2_"+particleName+".txt", std::ios_base::app);
  outFile << step->GetTrack()->GetCreatorProcess()->GetProcessName() << "   " << Energy << "     " << position.x() << "     " << position.y() << "     " << position.z() 
      << "     " << direction.x() << "     " << direction.y() << "     " << direction.z() << "     " << step->GetPreStepPoint()->GetGlobalTime() << "     " << eventID << "     " << std::endl;
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