#ifndef DetectorConstruction_h
#define DetectorConstruction_h 1

#include "globals.hh"
#include "G4VUserDetectorConstruction.hh"
#include "G4PVPlacement.hh"
#include "G4Cache.hh"

class G4Box;
class G4Tubs;
class G4LogicalVolume;
class G4VPhysicalVolume;
class G4Material;
class G4GenericMessenger;

using namespace std;


class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();
    ~DetectorConstruction();

    void InitializeAnalysis();

  public:
  
    G4VPhysicalVolume* Construct();

    void SetAEmission(G4int AE) {AEmission = AE;}
    G4int GetAEmission() {return AEmission;}

  private:

    void DefineMaterials();

  private:

    G4Material* GeneralAir;
    G4Material* GeneralHelium;
    G4Material* GeneralAluminium;
    G4Material* GeneralIron;
    G4Material* GeneralCopper;
    G4Material* GeneralTungsten;
    G4Material* GeneralLead;
    G4Material* GeneralVacuum;

    G4Box*             solidWorld;
    G4LogicalVolume*   logicWorld;
    G4VPhysicalVolume* physiWorld;
    G4VPhysicalVolume* physiTube;

    G4Box*             solidBox;
    G4Tubs*            solidTube;
    G4LogicalVolume*   logicTube;
    G4LogicalVolume*   logicBox;
    G4LogicalVolume*   logicCsIBar;
    G4LogicalVolume*   logicSiPm;

    G4int AEmission;

};

#endif
