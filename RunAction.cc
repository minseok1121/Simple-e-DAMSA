#include "RunAction.hh"
#include "Run.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"
#include "G4RunManager.hh"

// Constructor
RunAction::RunAction(G4int clusterId, G4int procId)
    : G4UserRunAction(), fClusterId(clusterId), fProcId(procId)
{
    // Constructor body can remain empty
}

// Destructor
RunAction::~RunAction()
{
    // No need for specific cleanup of vectors
}

// Begin of Run Action
void RunAction::BeginOfRunAction(const G4Run*)
{
if (!G4Threading::IsMasterThread()) return;
G4cout << "AM I IN?" << G4endl;
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName("Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId));  // Set file name for histograms
    analysisManager->SetVerboseLevel(0);

    // Create histograms
    G4int nbins = 100;
    G4double vmin = 0.;
    G4double vmax = 100.;

        analysisManager->CreateH1("GammaProduction", "dNdE", nbins, vmin, vmax);
        analysisManager->CreateH1("Stage1_e-", "dNdE", nbins, vmin, vmax);
        analysisManager->CreateH1("Stage1_e+", "dNdE", nbins, vmin, vmax);
        analysisManager->CreateH1("Stage1_gamma", "dNdE", nbins, vmin, vmax);
        analysisManager->CreateH1("Stage1_neutron", "dNdE", nbins, vmin, vmax);

    analysisManager->OpenFile();
}

// End of Run Action
void RunAction::EndOfRunAction(const G4Run* run)
{
    if (!G4Threading::IsMasterThread()) return;
    G4cout << "In master thread. Running EndOfRunAction." << G4endl;
    
    const Run* myRun = static_cast<const Run*>(run);
    
    // Analysis 작업
    auto analysisManager = G4AnalysisManager::Instance();

    for (size_t i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 100; ++j) {
            analysisManager->FillH1(i, j, myRun->GetEventDataH1()[i][j]);
        }
    }

    analysisManager->Write();
    analysisManager->CloseFile();
}


// Generate Run
G4Run* RunAction::GenerateRun() {
    return new Run();
}

void RunAction::StoreKineticEnergy(int particleIndex, G4double kineticEnergy) {
    Run* fRun = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    fRun->StoreKineticEnergy(particleIndex, kineticEnergy);
}