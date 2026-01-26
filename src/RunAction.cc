#include "RunAction.hh"
#include "G4Run.hh"
#include "G4SystemOfUnits.hh"

RunAction :: RunAction() :G4UserRunAction()
{
    auto analysisManager = G4AnalysisManager ::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetFileName("output.root");

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