#include "ActionInitialization.hh"
#include "PrimaryGeneratorAction.hh"
#include "EventAction.hh"
#include "SteppingActionDMG4.hh"
#include "RunAction.hh"

ActionInitialization::ActionInitialization(DetectorConstruction* detConstruction, DarkMatterPhysics* physics, G4int clusterId, G4int procId)
    : G4VUserActionInitialization(),
      fDetConstruction(detConstruction),
      fPhysics(physics),
      fClusterId(clusterId),
      fProcId(procId)
{}

ActionInitialization::~ActionInitialization()
{}

void ActionInitialization::BuildForMaster() const
{
    // Implement master thread actions here if needed
    SetUserAction(new RunAction(fClusterId, fProcId));
}

void ActionInitialization::Build() const
{
    SetUserAction(new PrimaryGeneratorAction(fDetConstruction));
    RunAction* runAction = new RunAction(fClusterId, fProcId);
    SetUserAction(runAction);
    EventAction* eventAction = new EventAction(fDetConstruction, fPhysics->GetDarkMatterPointer(), runAction);
    SetUserAction(eventAction);
    SetUserAction(new SteppingActionDMG4(fDetConstruction, eventAction, fClusterId, fProcId, runAction));
}
