#include "Run.hh"
#include "G4Event.hh"
#include "G4HCofThisEvent.hh"
#include "G4SDManager.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

Run::Run() : G4Run()
{
    // Initialize temp arrays
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < nbins; ++j) {
            tempKineticEnergyData[i][j] = 0;
            eventDataH1[i][j] = 0;
        }
    }
    /*
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            tempPos[i][j] = 0;
            eventDataH2[i][j] = 0;
        }
    }
    */
}

Run::~Run()
{
    // Destructor
}

void Run::RecordEvent(const G4Event* event)
{
    G4Run::RecordEvent(event);
    for (size_t particleIndex = 0; particleIndex < 7; ++particleIndex) {
    for (size_t E = 0; E < nbins; ++E) {
            int dNdE = tempKineticEnergyData[particleIndex][E];
            //if(dNdE > 0) G4cout << E << "  " << dNdE << "  ";
            eventDataH1[particleIndex][E] += dNdE;
            tempKineticEnergyData[particleIndex][E] = 0;
    }
}
/*
for (size_t i = 0; i < 100; ++i) {
        for (size_t j = 0; j < 100; ++j) {
            int dN = tempPos[i][j];
            eventDataH2[i][j] += dN;
            tempPos[i][j] = 0;
        }
}
*/

}

void Run::Merge(const G4Run* run)
{
    const Run* localRun = static_cast<const Run*>(run);
for (size_t i = 0; i < 7; ++i) {
    for (size_t j = 0; j < nbins; ++j) {
        eventDataH1[i][j] += localRun->eventDataH1[i][j];
    }
}
/*
for (size_t i = 0; i < 100; ++i) {
        for (size_t j = 0; j < 100; ++j) {
            eventDataH2[i][j] += localRun->eventDataH2[i][j];
        }
}
*/
    // 부모 클래스의 Merge 호출
    G4Run::Merge(localRun);
}

void Run::StoreKineticEnergy(int particleIndex, G4double kineticEnergy)
{
    tempKineticEnergyData[particleIndex][static_cast<int>(kineticEnergy/300*nbins)] += 1;
    //G4cout << static_cast<int>(kineticEnergy/300*nbins) << "   ";
}

void Run::StorePos(int x, int y)
{
    //G4cout << x << "   " << y << G4endl;
    tempPos[x][y] += 1;
}
