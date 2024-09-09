#include "globals.hh"

#include "EventAction.hh"
#include "DetectorConstruction.hh"
#include "SteppingActionDMG4.hh"

#include "DarkMatter.hh"

#include "G4Event.hh"
#include "G4EventManager.hh"
#include "G4TrajectoryContainer.hh"
#include "G4Trajectory.hh"
#include "G4VVisManager.hh"
#include "G4UImanager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

#include "G4AutoLock.hh"

#include "G4SDManager.hh"
#include "G4AnalysisManager.hh"

#include "RunAction.hh"

EventAction::EventAction(DetectorConstruction* myDC, DarkMatter* DMPointer, RunAction* runAction)
    : G4UserEventAction(), myDetector(myDC), myDarkMatter(DMPointer), fEventCounter(0), NEmissions(0), fRunAction(runAction)
{
}

EventAction::~EventAction()
{
}

void EventAction::BeginOfEventAction(const G4Event* event)
{
    theSteppingAction->Reset();
    myDetector->SetAEmission(0);
}

void EventAction::EndOfEventAction(const G4Event* event)
{
    G4AutoLock lock(&eventMutex);
    fEventCounter++;
    if(fEventCounter % 100000 == 0){
    G4cout << "Processed " << fEventCounter << " events so far." << G4endl;
    }
}

