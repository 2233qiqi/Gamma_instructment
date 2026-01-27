#include "DetectorConstruction.hh"
#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Torus.hh"
#include "G4SubtractionSolid.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4UnionSolid.hh"
#include "G4VisAttributes.hh"




DetectorConstruction::DetectorConstruction() 
{

}
DetectorConstruction::~DetectorConstruction(){

}

G4VPhysicalVolume *DetectorConstruction::Construct()
{
    G4NistManager* nist = G4NistManager::Instance();
    G4Material* mat_Air = nist->FindOrBuildMaterial("G4_AIR");
    G4Material* mat_Al  = nist->FindOrBuildMaterial("G4_Al");
    G4Material* mat_Ge  = nist->FindOrBuildMaterial("G4_Ge");
    G4Material* mat_Vac = nist->FindOrBuildMaterial("G4_Galactic");

 
    G4double D2_GeOD       = 82.0 * mm;  
    G4double L2_GeLen      = 50.0 * mm;   
    G4double R_Corner      = 7.0 * mm;   
    G4double R1_Corner     = 7.0 * mm;    
    
    G4double D1_HoleDia    = 14.0 * mm;   
    G4double Hole_Depth    = 40.0 * mm;  
                                         
    G4double D3_CapOD      = 102.0 * mm;  
    G4double T1_SideThick  = 1.2 * mm;    
    G4double L5_WinThick   = 0.015 * mm; 
    G4double L4_Dist       = 5.0 * mm;    
    
    G4double Cup_Thick     = 1.0 * mm;    
    G4double Cup_Height    = 20.0 * mm;   

    G4double R_Ge          = D2_GeOD / 2.0;
    G4double R_Hole        = D1_HoleDia / 2.0;
    G4double R_Cap         = D3_CapOD / 2.0;
    
   //世界
    G4double worldSize = 1.0 * m;
    G4Box* solidWorld = new G4Box("LogicWorld", worldSize/2, worldSize/2, worldSize/2);
    G4LogicalVolume* logicWorld = new G4LogicalVolume(solidWorld, mat_Air, "LogicWorld");
    G4VPhysicalVolume* physWorld = new G4PVPlacement(0, G4ThreeVector(), logicWorld, "World", 0, false, 0, true);

   //铝壳
    G4double capTotalHeight = 150.0 * mm; 

    G4Tubs* solidCapTube = new G4Tubs("CapTube", R_Cap - T1_SideThick, R_Cap, capTotalHeight/2.0, 0, 360*deg);
    G4Tubs* solidCapWin  = new G4Tubs("CapWin", 0, R_Cap, L5_WinThick/2.0, 0, 360*deg);
    
    G4LogicalVolume* logicCapTube = new G4LogicalVolume(solidCapTube, mat_Al, "CapTube_LV");
    G4LogicalVolume* logicCapWin  = new G4LogicalVolume(solidCapWin, mat_Al, "CapWin_LV");

    G4ThreeVector posCap(0,0,0);
    new G4PVPlacement(0, posCap, logicCapTube, "CapTube_PV", logicWorld, false, 0, true);
    
    G4double zWin = capTotalHeight/2.0 + L5_WinThick/2.0; 
    new G4PVPlacement(0, G4ThreeVector(0,0,zWin), logicCapWin, "CapWin_PV", logicWorld, false, 0, true);

   //内部环境
    G4Tubs* solidVac = new G4Tubs("Vacuum", 0, R_Cap - T1_SideThick, capTotalHeight/2.0, 0, 360*deg);
    G4LogicalVolume* logicVac = new G4LogicalVolume(solidVac, mat_Vac, "Vacuum_LV");
    new G4PVPlacement(0, G4ThreeVector(0,0,0), logicVac, "Vacuum_PV", logicWorld, false, 0, true);

    //Ge晶体
    G4Tubs* solidGeCore = new G4Tubs("GeCore", 0, R_Ge - R_Corner, L2_GeLen/2.0, 0, 360*deg);
    G4Tubs* solidGePlate = new G4Tubs("GePlate", 0, R_Ge, (L2_GeLen - 2*R_Corner)/2.0, 0, 360*deg);
    G4Torus* solidGeTorus = new G4Torus("GeTorus", 0, R_Corner, R_Ge - R_Corner, 0, 360*deg);

    
    G4UnionSolid* uniTemp1 = new G4UnionSolid("Ge_Core_Plate", solidGeCore, solidGePlate);
    G4double zTorusPos = L2_GeLen/2.0 - R_Corner;
    G4UnionSolid* uniTemp2 = new G4UnionSolid("Ge_Add_Top", uniTemp1, solidGeTorus, 0, G4ThreeVector(0,0, zTorusPos));
    G4UnionSolid* solidGeBullet = new G4UnionSolid("Ge_Bulletized", uniTemp2, solidGeTorus, 0, G4ThreeVector(0,0, -zTorusPos));
    G4Tubs* solidHole = new G4Tubs("Hole", 0, R_Hole, Hole_Depth/2.0, 0, 360*deg);

    G4double zHolePos = -L2_GeLen/2.0 + Hole_Depth/2.0 - 0.01*mm; 

    G4SubtractionSolid* solidGeFinal = new G4SubtractionSolid("Ge_Final", solidGeBullet, solidHole,  0, G4ThreeVector(0,0,zHolePos));

    fLogicGeCrystal = new G4LogicalVolume(solidGeFinal, mat_Ge, "GeCrystal_LV");


    //晶体
    G4double zGePos = (capTotalHeight/2.0) - L4_Dist - (L2_GeLen/2.0);

    new G4PVPlacement(0, G4ThreeVector(0,0,zGePos), fLogicGeCrystal, "GeCrystal_PV", logicVac, false, 0, true);


    //底座
    G4Tubs* solidCup = new G4Tubs("HolderCup", R_Ge, R_Ge + Cup_Thick, Cup_Height/2.0, 0, 360*deg);
    G4LogicalVolume* logicCup = new G4LogicalVolume(solidCup, mat_Al, "HolderCup_LV");

    G4double zCupPos = (zGePos - L2_GeLen/2.0) + Cup_Height/2.0;
    
    new G4PVPlacement(0, G4ThreeVector(0,0,zCupPos), logicCup, "HolderCup_PV", logicVac, false, 0, true);


    //可视化
    G4VisAttributes* visWorld = new G4VisAttributes(G4Color(1.0, 1.0, 1.0)); 
    visWorld->SetForceWireframe(true); 
    logicWorld->SetVisAttributes(visWorld);

    logicVac->SetVisAttributes(G4VisAttributes::GetInvisible());

    G4VisAttributes* visGe = new G4VisAttributes(G4Color(1.0, 0.0, 0.0, 1.0)); 
    visGe->SetForceSolid(true); 
    fLogicGeCrystal->SetVisAttributes(visGe);

    G4VisAttributes* visAlCap = new G4VisAttributes(G4Color(0.8, 0.8, 0.8, 0.3)); 
    visAlCap->SetForceSolid(true); 
    logicCapTube->SetVisAttributes(visAlCap);
    logicCapWin->SetVisAttributes(visAlCap);

    G4VisAttributes* visCup = new G4VisAttributes(G4Color(1.0, 0.8, 0.0, 1.0));
    visCup->SetForceSolid(true);
    logicCup->SetVisAttributes(visCup);

    return physWorld;


}   

