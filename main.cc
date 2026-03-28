#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"

#include "DetectorConstruction.hh"
#include "PhysicsList.hh"
#include "ActionInitialization.hh"

int main(int argc, char** argv)
{
    auto* runManager = new G4RunManager;

    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new PhysicsList());
    runManager->SetUserInitialization(new ActionInitialization());

    auto* visManager = new G4VisExecutive;
    visManager->Initialize();

    auto* ui = new G4UIExecutive(argc, argv);

    auto* UImanager = G4UImanager::GetUIpointer();

    UImanager->ApplyCommand("/control/execute run.mac");

    ui->SessionStart();

    delete ui;
    delete visManager;
    delete runManager;
    return 0;
}
