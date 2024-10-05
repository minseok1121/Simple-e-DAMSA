#include "globals.hh"

#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "SteppingActionDMG4.hh"

#include "QGSP_BERT.hh"
#include "FTFP_BERT.hh"

#include "DarkMatterPhysics.hh"
#include "DarkMatter.hh"

#include "G4UImanager.hh"
#include "G4UIterminal.hh"

#include "G4RunManager.hh"
#include "G4PhysListFactory.hh"

#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#include "G4ios.hh"

#include "G4MTRunManager.hh"
#include "G4RunManagerFactory.hh"
#include "ActionInitialization.hh"

#include "G4OpticalPhysics.hh"

#include <atomic>

#include "G4Threading.hh"

#include "G4AnalysisManager.hh"


std::atomic<int> fEventCounter(0);

int main(int argc,char** argv) {

  G4int clusterId = std::stoi(argv[2]);
  G4int procId = std::stoi(argv[3]);

CLHEP::HepRandom::setTheSeed(time(NULL));

  // Run manager
  auto runManager =
    G4RunManagerFactory::CreateRunManager(G4RunManagerType::MT);
  
  G4int numCores = G4Threading::G4GetNumberOfCores();
    runManager->SetNumberOfThreads(10);
    //runManager->SetNumberOfThreads(1);

    std::cout << "Number of workers (threads): " << runManager->GetNumberOfThreads() << std::endl;
//G4MTRunManager* runManager = new G4MTRunManager;
  // UserInitialization classes
  DetectorConstruction* mkexp = new DetectorConstruction;
  runManager->SetUserInitialization(mkexp);

  // ___ Here the "extension" part starts ___
  G4PhysListFactory factory;
  G4VModularPhysicsList * phys = factory.GetReferencePhysList("FTFP_BERT");
  // ^^^ most of the standard physics lists are available by this interface

//  G4PhysicsListHelper * phLHelper = G4PhysicsListHelper::GetPhysicsListHelper();
//  phLHelper->DumpOrdingParameterTable();
double epsilon = 0.01;
if (argc > 1) {
    epsilon = std::stod(argv[1]);
  }

  DarkMatterPhysics* myPhysics = new DarkMatterPhysics(epsilon);
  phys->RegisterPhysics(myPhysics);
  // ^^^ Here the "extension" part ends ^^^

  runManager->SetUserInitialization(phys);  // init phys

  // UserAction classes
  runManager->SetUserInitialization(new ActionInitialization(mkexp, myPhysics, clusterId, procId));
  // Initialize the run manager
  runManager->Initialize();

  // User interactions
  G4UImanager * UI = G4UImanager::GetUIpointer();  
 
  UI->ApplyCommand("/control/execute cardg.g4mac");



    delete runManager;

  return 0;
}
