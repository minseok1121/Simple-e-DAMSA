#include "PrimaryGeneratorAction.hh"
#include "DetectorConstruction.hh"
#include "G4Event.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4ParticleGun.hh"
#include "G4RunManager.hh"
#include "RunAction.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction(DetectorConstruction* myDC)
: myDetector(myDC)
{
    // Particle gun 생성
    particleGun = new G4ParticleGun(1);
    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();

    // 전자 설정
    G4ParticleDefinition* electron = particleTable->FindParticle("e-");
    particleGun->SetParticleDefinition(electron);
    particleGun->SetParticleMomentumDirection(G4ThreeVector(0.,0.,1.));  // z축 방향으로 발사
    particleGun->SetParticleEnergy(300*MeV);
}

PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete particleGun;
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    const G4double radius = 1*micrometer;  // 반지름 1마이크론
    const G4double length = 1*cm;          // 길이 1cm
    const G4int nElectrons = 1;    // 10^8 개의 전자
 RunAction* runAction = const_cast<RunAction*>(static_cast<const RunAction*>(G4RunManager::GetRunManager()->GetUserRunAction()));
 runAction->SetNElectrons(nElectrons);
    for (G4int i = 0; i < nElectrons; ++i) {
        // 원통 좌표계에서 랜덤한 r, theta, z 값을 생성
        G4double r = radius * std::sqrt(G4UniformRand());  // 균일한 분포에서 r을 선택
        G4double theta = 2 * CLHEP::pi * G4UniformRand();  // 0에서 2pi 사이의 각도
        G4double z = length * (G4UniformRand() - 0.5);     // -0.5cm ~ 0.5cm 사이의 z 좌표

        // 원통 좌표계를 Cartesian 좌표계로 변환
        G4double x = r * std::cos(theta);
        G4double y = r * std::sin(theta);

        // 입자의 위치 설정
        particleGun->SetParticlePosition(G4ThreeVector(x, y, z-27*cm));

        // 입자를 이벤트에 추가
        particleGun->GeneratePrimaryVertex(anEvent);
    }
}
