#ifndef ActionInitialization_hh
#define ActionInitialization_hh

#include "G4VUserActionInitialization.hh"
#include "DetectorConstruction.hh"
#include "DarkMatterPhysics.hh"

class ActionInitialization : public G4VUserActionInitialization
{
public:
    ActionInitialization(DetectorConstruction* detConstruction, DarkMatterPhysics* physics, G4int clusterId, G4int procId);
    virtual ~ActionInitialization();

    virtual void BuildForMaster() const override;
    virtual void Build() const override;

private:
    DetectorConstruction* fDetConstruction;
    DarkMatterPhysics* fPhysics;
    G4int fClusterId; 
    G4int fProcId;
};

#endif
