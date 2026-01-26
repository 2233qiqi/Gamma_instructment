#include "EventAction.hh"
#include "G4Event.hh"
#include "G4SystemOfUnits.hh"
#include "G4AnalysisManager.hh"

EventAction::EventAction():G4UserEventAction(),fTotalEdep (0)
{

}

EventAction::~EventAction()
{

}

void EventAction :: BeginOfEventAction (const G4Event *event)
{
    fTotalEdep =0;
}

void EventAction :: EndOfEventAction(const G4Event *event)
{
    if (fTotalEdep > 0) 
        {
           auto analysisManager = G4AnalysisManager::Instance();
           analysisManager->FillH1(0, fTotalEdep);
        }
}

void EventAction::AddEdep(G4double edep)
{
    fTotalEdep += edep;
}