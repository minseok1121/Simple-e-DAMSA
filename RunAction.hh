#ifndef RUNACTION_HH
#define RUNACTION_HH

#include "G4UserRunAction.hh"
#include <vector>

class Run;

class RunAction : public G4UserRunAction
{
public:
    RunAction(G4int clusterId, G4int procId);
    virtual ~RunAction();

    virtual void BeginOfRunAction(const G4Run*) override;
    virtual void EndOfRunAction(const G4Run* run) override;

    G4Run* GenerateRun() override;
    
    void StoreKineticEnergy(int particleIndex, G4double kineticEnergy);
    void StorePos(int x, int y);

    void SetNElectrons(G4int n) { nElectrons = n; }
    G4int GetNElectrons() const { return nElectrons; }

    G4int GetNbins() const { return nbins; }

private:
    G4int fClusterId;
    G4int fProcId;
    G4int nElectrons;

    G4int nbins = 300;

    Run* fRun;
};

#endif // RUNACTION_HH
