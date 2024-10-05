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
void RunAction::BeginOfRunAction(const G4Run* aRun)
{
if (!G4Threading::IsMasterThread()) return;
/*
 auto analysisManager = G4AnalysisManager::Instance();
//analysisManager->SetNtupleMerging(true);
analysisManager->SetDefaultFileType("root");
analysisManager->SetFileName("Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId));  // Set file name for histograms
analysisManager->SetVerboseLevel(0);
analysisManager->OpenFile();
// 각 에너지 데이터를 개별 트리로 생성
analysisManager->CreateNtuple("GammaData", "Gamma Production Energy");
analysisManager->CreateNtupleDColumn("GammaProductionEnergy");
analysisManager->FinishNtuple();

analysisManager->CreateNtuple("ElectronData", "Stage1 Electron Energy");
analysisManager->CreateNtupleDColumn("Stage1_e-_Energy");
analysisManager->FinishNtuple();

analysisManager->CreateNtuple("PositronData", "Stage1 Positron Energy");
analysisManager->CreateNtupleDColumn("Stage1_e+_Energy");
analysisManager->FinishNtuple();

analysisManager->CreateNtuple("GammaEnergyData", "Stage1 Gamma Energy");
analysisManager->CreateNtupleDColumn("Stage1_gammaEnergy");
analysisManager->FinishNtuple();

analysisManager->CreateNtuple("NeutronData", "Stage1 Neutron Energy");
analysisManager->CreateNtupleDColumn("Stage1_neutronEnergy");
analysisManager->FinishNtuple();

analysisManager->CreateNtuple("ALPData", "Stage1 ALP Energy");
analysisManager->CreateNtupleDColumn("Stage1_ALPEnergy");
analysisManager->FinishNtuple();

analysisManager->CreateNtuple("ALPProductionData", "ALP Production Energy");
analysisManager->CreateNtupleDColumn("ALPproductionEnergy");
analysisManager->FinishNtuple();
*/
G4cout << nbins << G4endl;
   
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName("Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId));  // Set file name for histograms
    analysisManager->SetVerboseLevel(0);

    // Create histograms
    G4double vmin = 0.;
    G4double vmax = nbins;
    //G4double vmax = 20.;

        analysisManager->CreateH1("GammaProduction", "dNdE", nbins, vmin, vmax);
        analysisManager->CreateH1("Stage1_e-", "dNdE", nbins, vmin, vmax);
        analysisManager->CreateH1("Stage1_e+", "dNdE", nbins, vmin, vmax);
        analysisManager->CreateH1("Stage1_gamma", "dNdE", nbins, vmin, vmax);
        analysisManager->CreateH1("Stage1_neutron", "dNdE", nbins, vmin, vmax);
        analysisManager->CreateH1("Stage1_ALP", "dNdE", nbins, vmin, vmax);
        analysisManager->CreateH1("ALPproduction", "dNdE", nbins, vmin, vmax);

        //analysisManager->CreateH2("Beam_e-", "Beam Dist", 100, -0.002, 0.002, 100, -0.002, 0.002);
//////////////////////////////////////////////////////////////////////////////////////////////////////
/*
        std::string filenames[] = {
        "Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "Stage1_e-.txt",
        "Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "Stage1_e+.txt",
        "Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "Stage1_gamma.txt",
        "Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "Stage1_neutron.txt"
    };
    for (const auto& filename : filenames) {
        std::ofstream file(filename);
        for (int i = 0; i < 300; ++i) {
            file << i << " 0\n";
        }
        file.close();
    }
        
        std::ofstream file2("Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "GammaProductions.txt");
        for (int i = 0; i < 300; ++i) {
            file2 << i << " " << 0 << "\n";  // 각 줄에 에너지 구간과 0 카운트로 초기화
        }
        file2.close();

    const std::string filePath = "Little" + std::to_string(fClusterId) + "_" + std::to_string(fProcId) + "_" + "Beam_e-.txt";
    std::ofstream file3(filePath);

    const int gridSize = 100;
    for (int y = 0; y < gridSize; ++y) {
        for (int x = 0; x < gridSize; ++x) {
            file3 << x+100*y << " 0\n";
        }
    }
    file3.close();
  analysisManager->OpenFile();
*/
analysisManager->OpenFile();
}

// End of Run Action
void RunAction::EndOfRunAction(const G4Run* run)
{
    if (!G4Threading::IsMasterThread()) return;
    G4cout << nbins << G4endl;

    const Run* myRun = static_cast<const Run*>(run);
    
    // Analysis 작업
    auto analysisManager = G4AnalysisManager::Instance();

    for (size_t i = 0; i < 7; ++i) {
        //analysisManager->GetH1(i)->Sumw2(); // 에러바 계산 활성화 -> 수동으로 해야함
        for (size_t j = 0; j < nbins; ++j) {
            analysisManager->FillH1(i, j, myRun->GetEventDataH1()[i][j]);
            //analysisManager->FillH1(i, j, 1);
        }
    }
    /*
    // Fill H2 histogram
    for (size_t x = 0; x < 100; ++x) {
        for (size_t y = 0; y < 100; ++y) {
            analysisManager->FillH2(0, x*0.00004-0.002, y*0.00004-0.002, myRun->GetEventDataH2()[x][y]);
        }
    }
    */
    
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

void RunAction::StorePos(int x, int y) {
    Run* fRun = static_cast<Run*>(G4RunManager::GetRunManager()->GetNonConstCurrentRun());
    fRun->StorePos(x, y);
}

