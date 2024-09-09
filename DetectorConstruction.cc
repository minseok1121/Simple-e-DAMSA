#include "DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4MaterialTable.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4GeometryManager.hh"
#include "G4TransportationManager.hh"
#include "G4UserLimits.hh"
#include "G4SDManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4GenericMessenger.hh"

#include "G4ios.hh"

#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4SubtractionSolid.hh"
#include "G4MultiFunctionalDetector.hh"
#include "G4SDManager.hh"

#include "G4Cons.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"
#include "G4OpticalSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4AnalysisManager.hh"


DetectorConstruction::DetectorConstruction()
{
}


DetectorConstruction::~DetectorConstruction()
{
}


G4VPhysicalVolume* DetectorConstruction::Construct()
{
  G4Material* DefinePolyurethane();
  G4NistManager* nist = G4NistManager::Instance();
  G4Material* tar_mat = nist->FindOrBuildMaterial("G4_W");
  G4Material* modulator_mat = DefinePolyurethane();
  G4Material* chamber_mat = nist->FindOrBuildMaterial("G4_Galactic");

  G4double detectorSizeX = 5.0*cm;
  G4double detectorSizeY = 5.0*cm;
  G4double targetLength = 10.0*cm;
  G4double                          modulatorWidth = 20.0*cm;
  G4double chamberRadius  = 10.0*cm, chamberSize = 30.0*cm;
  G4double                          ECALSize = 44.0*cm;
  
  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = 200*cm;
  G4double world_sizeZ  = 200*cm;
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_Galactic");

  auto solidWorld = new G4Box("World",                           // its name
    0.5 * world_sizeXY, 0.5 * world_sizeXY, 0.5 * world_sizeZ);  // its size

  auto logicWorld = new G4LogicalVolume(solidWorld,  // its solid
    chamber_mat,                                       // its material
    "World");                                        // its name

  auto physWorld = new G4PVPlacement(nullptr,  // no rotation
    G4ThreeVector(),                           // at (0,0,0)
    logicWorld,                                // its logical volume
    "World",                                   // its name
    nullptr,                                   // its mother  volume
    false,                                     // no boolean operation
    0,                                         // copy number
    checkOverlaps);                            // overlaps checking

 //G4ThreeVector pos0 = G4ThreeVector(0,0,-(targetLength/2+modulatorWidth+chamberSize/2+0.6*cm));
  G4ThreeVector pos0 = G4ThreeVector(0,0,-(targetLength/2+chamberSize/2+0.6*cm));
  auto targetS = new G4Box("target", detectorSizeX / 2, detectorSizeY / 2, targetLength / 2);
  auto logictarget = new G4LogicalVolume(targetS,  // its solid
    tar_mat,                                     // its material
    //G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe")
    "target");                                 // its name
  new G4PVPlacement(nullptr,  // no rotation
    pos0,          // at (0,0,0)
    logictarget,                 // its logical volume
    "target",               // its name
    logicWorld,               // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking

    G4ThreeVector pos2 = G4ThreeVector(0,0,0);
  auto steelchamS = new G4Tubs("steelchamber", 0, chamberRadius+0.6*cm, chamberSize/2+0.6*cm, 0. * deg, 360. * deg);
  auto chamberS = new G4Tubs("chamber", 0, chamberRadius, chamberSize/2, 0. * deg, 360. * deg);
  auto logicchamber = new G4LogicalVolume(chamberS, chamber_mat, "chamber", nullptr, nullptr, nullptr);
  G4SubtractionSolid* solidDecayChamber = new G4SubtractionSolid("DecayChamber", steelchamS, chamberS);
  G4LogicalVolume* logicDecayChamber = new G4LogicalVolume(solidDecayChamber, G4NistManager::Instance()->FindOrBuildMaterial("G4_Fe"), "DecayChamber");
  new G4PVPlacement(nullptr,  // no rotation
    pos2,          // at (x,y,z)
    logicchamber,                // its logical volume
    "chamber",                // its name
    logicWorld,                  // its mother  volume
    false,                    // no boolean operations
    0,                        // copy number
    checkOverlaps);          // checking overlaps
  new G4PVPlacement(nullptr,  // no rotation
    pos2,          // at (x,y,z)
    logicDecayChamber,                // its logical volume
    "steel",                // its name
    logicWorld,                  // its mother  volume
    false,                    // no boolean operations
    0,                        // copy number
    checkOverlaps);          // checking overlaps

  G4ThreeVector pos3 = G4ThreeVector(0,0,chamberSize/2+0.6*cm+ECALSize/2);
  auto ECALS = new G4Tubs("ECAL", 0., chamberRadius, ECALSize/2, 0. * deg, 360. * deg);
  auto logicECAL = new G4LogicalVolume(ECALS,  // its solid
    chamber_mat,                                     // its material
    "ECAL");                                 // its name
  new G4PVPlacement(nullptr,  // no rotation
    pos3,          // at (0,0,0)
    logicECAL,                 // its logical volume
    "ECAL",               // its name
    logicWorld,               // its mother  volume
    false,                    // no boolean operation
    0,                        // copy number
    checkOverlaps);           // overlaps checking

  return physWorld;
}

G4Material* DefinePolyurethane() {
    G4NistManager* nistManager = G4NistManager::Instance();
    G4double density = 1.2 * g/cm3;
    G4Element* elC = nistManager->FindOrBuildElement("C");
    G4Element* elH = nistManager->FindOrBuildElement("H");
    G4Element* elO = nistManager->FindOrBuildElement("O");
    G4Material* polyurethane = new G4Material("Polyurethane", density, 3);
    polyurethane->AddElement(elC, 15);
    polyurethane->AddElement(elH, 22);
    polyurethane->AddElement(elO, 1);

    return polyurethane;
}