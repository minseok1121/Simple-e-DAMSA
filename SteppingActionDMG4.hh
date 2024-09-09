#pragma once

#include "G4UserSteppingAction.hh"
#include "globals.hh"

#include "G4ThreeVector.hh"
#include "G4OpBoundaryProcess.hh"

#include <mutex>
#include "G4Threading.hh"
#include <fstream>
#include "G4Accumulable.hh"
#include "G4AccumulableManager.hh"

class DetectorConstruction;
class EventAction;
class RunAction;


class SteppingActionDMG4 : public G4UserSteppingAction
{
  public:
    SteppingActionDMG4(DetectorConstruction* myDC, EventAction* myEA, G4int clusterId, G4int procId, RunAction* runAction);
    virtual ~SteppingActionDMG4(){};

    virtual void UserSteppingAction(const G4Step*);
    
    virtual void Reset();

    virtual void Finalize();

  private:

    EventAction* eventAction;
    G4ThreeVector fInitialPosition;
    G4int fClusterId;
    G4int fProcId;
    G4int ms = 11;

    RunAction* fRunAction;
};
