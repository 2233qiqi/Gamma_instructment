#include "SteppingAction.hh"
#include "DetectorConstruction.hh"
#include "EventAction.hh"
#include "RunAction.hh"

#include "G4Step.hh"
#include "G4Event.hh"
#include "RunAction.hh"
#include "G4RunManager.hh"
#include "G4LogicalVolume.hh"

SteppingAction::SteppingAction(EventAction* eventAction):G4UserSteppingAction(), fEventAction(eventAction), fScoringVolume(NULL)
{

}

SteppingAction :: ~SteppingAction()
{

}

void SteppingAction::UserSteppingAction(const G4Step* step)
{
    
  G4LogicalVolume* volume = step->GetPreStepPoint()->GetTouchableHandle()
                              ->GetVolume()->GetLogicalVolume();

    if (volume->GetName() == "Ge_LV") {
        G4double edep = step->GetTotalEnergyDeposit();

        if (edep > 0.) {
            fEventAction->AddEdep(edep);
        }
    }
  }

