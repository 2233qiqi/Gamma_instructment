#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4VisAttributes.hh"
#include "G4Colour.hh"

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct()
{
    auto* nist = G4NistManager::Instance();
    auto* mat_Air = nist->FindOrBuildMaterial("G4_AIR");
    auto* mat_Al = nist->FindOrBuildMaterial("G4_Al");
    auto* mat_Ge = nist->FindOrBuildMaterial("G4_Ge");
    auto* mat_Vac = nist->FindOrBuildMaterial("G4_Galactic");

    G4double D2 = 82.0 * mm;
    G4double L2 = 50.0 * mm;
    G4double D1 = 14.0 * mm;
    G4double L1 = 20.0 * mm;
    G4double R_corner = 7.0 * mm;
    G4double R1_hole = 7.0 * mm;
    
    G4double D3 = 102.0 * mm;
    G4double L3 = 61.0 * mm;
    G4double T1 = 1.2 * mm;
    G4double L5 = 0.015 * mm;
    G4double L4 = 5.0 * mm;
    
    G4double P2_DeadLayer = 0.5 * mm;

    G4double worldSize = 30 * cm;
    auto* solidWorld = new G4Box("World", worldSize, worldSize, worldSize);
    auto* logicWorld = new G4LogicalVolume(solidWorld, mat_Air, "World_LV");
    auto* physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World_PV", nullptr, false, 0, true);

    auto* solidCap = new G4Tubs("AlCap", 0, D3/2., L3/2., 0, 360*deg);
    auto* logicCap = new G4LogicalVolume(solidCap, mat_Al, "AlCap_LV");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,0), logicCap, "AlCap_PV", logicWorld, false, 0, true);

    auto* solidVac = new G4Tubs("Vac", 0, (D3/2.) - T1, (L3 - L5)/2., 0, 360*deg);
    auto* logicVac = new G4LogicalVolume(solidVac, mat_Vac, "Vac_LV");
    new G4PVPlacement(nullptr, G4ThreeVector(0,0,-L5/2.), logicVac, "Vac_PV", logicCap, false, 0, true);

    const G4int numZ = 4;
    G4double zPlanes[] = { -L2/2., L2/2. - R_corner, L2/2. - R_corner + 2*mm, L2/2. }; 
    G4double rInner[]  = { 0, 0, 0, 0 };
    G4double rOuter[]  = { D2/2., D2/2., D2/2. - 2*mm, D2/2. - R_corner };
    
    auto* solidGeCyl = new G4Tubs("GeCyl", 0, D2/2., L2/2., 0, 360*deg);
    
    auto* solidHoleBody = new G4Tubs("HoleBody", 0, D1/2., (L1 - R1_hole)/2., 0, 360*deg);
    auto* solidHoleSphere = new G4Sphere("HoleSphere", 0, R1_hole, 0, 360*deg, 0, 180*deg);
    
    G4double holeZ = -L2/2. + (L1 - R1_hole)/2.;
    auto* solidGeWithHole = new G4SubtractionSolid("GeWithHole", solidGeCyl, solidHoleBody, nullptr, G4ThreeVector(0,0, holeZ));
    solidGeWithHole = new G4SubtractionSolid("GeWithFinalHole", solidGeWithHole, solidHoleSphere, nullptr, G4ThreeVector(0,0, -L2/2. + L1 - R1_hole));

    auto* logicGe = new G4LogicalVolume(solidGeWithHole, mat_Ge, "Ge_LV");

    G4double zPlacement = (L3/2. - L5) - L4 - L2/2.;
    new G4PVPlacement(nullptr, G4ThreeVector(0,0, zPlacement), logicGe, "Ge_PV", logicVac, false, 0, true);

    auto* visGe = new G4VisAttributes(G4Colour(1.0, 0.5, 0.5, 0.8));
    visGe->SetForceSolid(true);
    logicGe->SetVisAttributes(visGe);

    auto* visAl = new G4VisAttributes(G4Colour(0.7, 1.0, 0.7, 0.8));
    logicCap->SetVisAttributes(visAl);

    return physWorld;
}
