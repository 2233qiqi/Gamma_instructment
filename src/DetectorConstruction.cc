#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4Polycone.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"

DetectorConstruction::DetectorConstruction() {}
DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    auto* nist = G4NistManager::Instance();
    auto* mat_Air = nist->FindOrBuildMaterial("G4_AIR");
    auto* mat_Al  = nist->FindOrBuildMaterial("G4_Al");
    auto* mat_Ge  = nist->FindOrBuildMaterial("G4_Ge");
    auto* mat_Vac = nist->FindOrBuildMaterial("G4_Galactic");

    G4double D1 = 14.0 * mm;
    G4double D2 = 82.0 * mm;
    G4double D3 = 102.0 * mm;
    G4double L1 = 20.0 * mm;
    G4double L2 = 50.0 * mm;
    G4double L3 = 61.0 * mm;
    G4double L4 = 5.0 * mm;
    G4double L5 = 0.015 * mm;
    G4double T1 = 1.2 * mm;
    G4double R  = 7.0 * mm;
    G4double R1 = 7.0 * mm;
    G4double P2 = 0.5 * mm;

    auto* solidWorld = new G4Box("World", 20*cm, 20*cm, 20*cm);
    auto* logicWorld = new G4LogicalVolume(solidWorld, mat_Air, "World_LV");
    auto* physWorld  = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World_PV", nullptr, false, 0);

    auto* solidCap = new G4Tubs("AlCap", 0, D3/2., L3/2., 0, 360*deg);
    auto* logicCap = new G4LogicalVolume(solidCap, mat_Al, "AlCap_LV");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), logicCap, "AlCap_PV", logicWorld, false, 0);

    G4double vacRadius = D3/2. - T1;
    G4double vacHeight = L3 - L5;
    auto* solidVac = new G4Tubs("Vac", 0, vacRadius, vacHeight/2., 0, 360*deg);
    auto* logicVac = new G4LogicalVolume(solidVac, mat_Vac, "Vac_LV");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,-L5/2.), logicVac, "Vac_PV", logicCap, false, 0);

    const G4int nZ = 3;
    G4double zPlanes[] = { -L2/2., L2/2. - R, L2/2. };
    G4double rInner[]  = { 0, 0, 0 };
    G4double rOuter[]  = { D2/2., D2/2., D2/2. - R }; 
    auto* solidGeFull = new G4Polycone("GeFull", 0, 360*deg, nZ, zPlanes, rInner, rOuter);

    G4double zPlanesAct[] = { -L2/2., L2/2. - R, L2/2. - P2 };
    G4double rOuterAct[]  = { D2/2. - P2, D2/2. - P2, D2/2. - R };
    auto* solidGeActiveBase = new G4Polycone("GeActiveBase", 0, 360*deg, nZ, zPlanesAct, rInner, rOuterAct);

    auto* holeCyl = new G4Tubs("HoleCyl", 0, D1/2., (L1-R1)/2., 0, 360*deg);
    auto* holeSph = new G4Sphere("HoleSph", 0, R1, 0, 360*deg, 0, 90*deg);
    
    G4double holeZ_cyl = -L2/2. + (L1-R1)/2.;
    G4double holeZ_sph = -L2/2. + (L1-R1);

    auto* solidGeActiveWithHole = new G4SubtractionSolid("GeAct1", solidGeActiveBase, holeCyl, nullptr, G4ThreeVector(0,0,holeZ_cyl));
    auto* solidGeActiveFinal = new G4SubtractionSolid("GeActiveFinal", solidGeActiveWithHole, holeSph, nullptr, G4ThreeVector(0,0,holeZ_sph));

    auto* logicGeDead = new G4LogicalVolume(solidGeFull, mat_Ge, "GeDead_LV");
    auto* logicGeActive = new G4LogicalVolume(solidGeActiveFinal, mat_Ge, "GeActive_LV");

    G4double crystalZ = (vacHeight/2.) - L4 - L2/2.;
    
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,crystalZ), logicGeDead, "GeDead_PV", logicVac, false, 0);
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), logicGeActive, "GeActive_PV", logicGeDead, false, 0);

    auto* visActive = new G4VisAttributes(G4Colour(1.0, 0.0, 0.0, 0.5));
    visActive->SetForceSolid(true);
    logicGeActive->SetVisAttributes(visActive);

    auto* visDead = new G4VisAttributes(G4Colour(0.5, 0.5, 0.5, 0.3));
    logicGeDead->SetVisAttributes(visDead);

    return physWorld;
}