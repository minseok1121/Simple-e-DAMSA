#ifndef RUN_HH
#define RUN_HH

#include "G4Run.hh"
#include <tuple>

class Run : public G4Run
{
public:
    Run();
    virtual ~Run();

    virtual void RecordEvent(const G4Event* evt) override;
    virtual void Merge(const G4Run* aRun) override;

    void StoreKineticEnergy(int particleIndex, G4double kineticEnergy);
    void StorePos(int x, int y);

     // Getter functions
    const int (*GetEventDataH1() const)[300] { return eventDataH1; }
    const int (*GetEventDataH2() const)[100] { return eventDataH2; }
    
    int tempKineticEnergyData[7][300]; // Updated to array
    int tempPos[100][100]; // Existing array

private:
    int eventDataH1[7][300]; // Updated to array
    int eventDataH2[100][100]; // New histogram array

    G4int nbins = 300;
    
};

#endif // RUN_HH
