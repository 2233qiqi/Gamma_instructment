#include "PrimaryGeneratorAction.hh"

#include "G4ParticleGun.hh"
#include "G4IonTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4Geantino.hh"    
#include "G4SystemOfUnits.hh"
#include "G4Event.hh"
#include "G4Exception.hh"

PrimaryGeneratorAction::PrimaryGeneratorAction() : G4VUserPrimaryGeneratorAction()
{
    fGPS = new G4GeneralParticleSource();
}

void PrimaryGeneratorAction::GeneratePrimaries(G4Event *event)
{
    fGPS->GeneratePrimaryVertex(event);
}
PrimaryGeneratorAction::~PrimaryGeneratorAction()
{
    delete fGPS;
}
