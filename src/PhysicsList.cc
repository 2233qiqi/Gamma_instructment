#include "PhysicsList.hh"

#include "G4ProcessManager.hh"
#include "G4Transportation.hh"

#include "G4Gamma.hh"
#include "G4Electron.hh"
#include "G4Positron.hh"

#include "G4PhotoElectricEffect.hh"
#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList()
{
    defaultCutValue = 0.1 * mm;
}

PhysicsList::~PhysicsList() {}



void PhysicsList::ConstructParticle()
{
    G4Gamma::GammaDefinition();
    G4Electron::ElectronDefinition();
    G4Positron::PositronDefinition();
}



void PhysicsList::ConstructProcess()
{
    AddTransportation();

    G4ProcessManager* pManager = G4Gamma::Gamma()->GetProcessManager();

    pManager->AddDiscreteProcess(new G4PhotoElectricEffect);
    pManager->AddDiscreteProcess(new G4ComptonScattering);
    pManager->AddDiscreteProcess(new G4GammaConversion);

    pManager = G4Electron::Electron()->GetProcessManager();

    pManager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
    pManager->AddProcess(new G4eIonisation,        -1, 2, 2);
    pManager->AddProcess(new G4eBremsstrahlung,    -1, 3, 3);

    pManager = G4Positron::Positron()->GetProcessManager();

    pManager->AddProcess(new G4eMultipleScattering, -1, 1, 1);
    pManager->AddProcess(new G4eIonisation,        -1, 2, 2);
    pManager->AddProcess(new G4eBremsstrahlung,    -1, 3, 3);
    pManager->AddProcess(new G4eplusAnnihilation,   0, -1, 4);
}



void PhysicsList::SetCuts()
{
    SetCutValue(defaultCutValue, "gamma");
    SetCutValue(defaultCutValue, "e-");
    SetCutValue(defaultCutValue, "e+");


    DumpCutValuesTable();
}
