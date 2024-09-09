#include "Run.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

Run::Run() : G4Run()
{
}

Run::~Run()
{
    // Destructor
}

void Run::RecordEvent(const G4Event* event)
{
    G4Run::RecordEvent(event);

    for (size_t particleIndex = 0; particleIndex < 5; ++particleIndex) {
    for (size_t E = 0; E < 100; ++E) {
            int dNdE = tempKineticEnergyData[particleIndex][E];
            eventDataH1[particleIndex][E] += dNdE;
            tempKineticEnergyData[particleIndex][E] = 0;
    }
}

}

void Run::Merge(const G4Run* run)
{
    const Run* localRun = static_cast<const Run*>(run);

const auto& localEventDataH1 = localRun->eventDataH1;
for (size_t i = 0; i < 5; ++i) {
    for (size_t j = 0; j < 100; ++j) {
        if(!localEventDataH1[i][j]) continue;
        eventDataH1[i][j] += localEventDataH1[i][j];
    }
}

    // 부모 클래스의 Merge 호출
    G4Run::Merge(localRun);
}

void Run::StoreKineticEnergy(int particleIndex, G4double kineticEnergy)
{
    tempKineticEnergyData[particleIndex][static_cast<int>(kineticEnergy)] += 1;
}
