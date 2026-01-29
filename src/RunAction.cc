#include "RunAction.hh"
#include "EventAction.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

RunAction::RunAction() : G4UserRunAction()
{
    auto analysisManager = G4AnalysisManager::Instance();
    

    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName("EventData"); 
    analysisManager->CreateH1("E_dep", "Energy Deposition", 3000, 0.,1500.0*keV);
}

RunAction::~RunAction()
{
}

void RunAction::BeginOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile();
}

void RunAction::EndOfRunAction(const G4Run*)
{
    auto analysisManager = G4AnalysisManager::Instance();
    
 
    analysisManager->Write();
    analysisManager->CloseFile();

    
}