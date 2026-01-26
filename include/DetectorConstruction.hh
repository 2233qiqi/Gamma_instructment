#ifndef MyDetectorConstruction_h
#define MyDetectorConstruction_h 1

#include "G4VUserDetectorConstruction.hh"
#include "globals.hh"

class DetectorConstruction;
class G4VPhysicalVolume;
class G4LogicalVolume;
class G4Material;

class DetectorConstruction : public G4VUserDetectorConstruction
{
  public:

    DetectorConstruction();

    virtual ~DetectorConstruction();
    virtual G4VPhysicalVolume* Construct();


  private:
    void DefineMaterials();
    G4LogicalVolume* fLogicGeCrystal; 
};


#endif