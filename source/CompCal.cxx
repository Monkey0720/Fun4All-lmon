
//_____________________________________________________________________________
//
// composite calorimeter,
// consists of a matrix of individual cells
//_____________________________________________________________________________

//local headers
#include "CompCal.h"
#include "Cell.h"
#include "OpDet.h"
#include "RootOut.h"

//ROOT
#include <TTree.h>

//Geant headers
#include <Geant4/G4LogicalVolume.hh>

//C++ headers
#include <vector>


//_____________________________________________________________________________
CompCal::CompCal(const G4String& nam, G4double zpos, G4double ypos, G4LogicalVolume *top, RootOut *rout): 
  fNam(nam) {

//  G4cout << "CompCal::CompCal: " << fNam << G4endl;

  //number of cells in x and y
  G4int ncells = 7;

  //create the cells
  fCells = new std::vector<Cell*>;
  for(G4int ix=0; ix<ncells; ix++) {
    for(G4int iy=0; iy<ncells; iy++) {

      //name for the cell from ix and iy
      std::stringstream ss;
      ss << "_";
      ss.fill('0');
      ss.width(2);
      ss << ix << "x";
      ss.fill('0');
      ss.width(2);
      ss << iy;

      G4String cell_nam = fNam + ss.str();
      Cell *cl = new Cell(cell_nam, ix, iy, ncells, zpos, ypos, top, *this, rout);
      fCells->push_back(cl);
      cl->CreateOutput(rout->GetTree());
    }
  }

  ClearEvent();

}//CompCal

//_____________________________________________________________________________
void CompCal::CreateOutput(TTree *tree) {

  AddBranch("_en", &fEdep, tree);
  AddBranch("_x", &fX, tree);
  AddBranch("_y", &fY, tree);
  AddBranch("_z", &fZ, tree);
  AddBranch("_xyzE", &fXyzE, tree);

  AddBranch("_nphot", &fNphot, tree);
  AddBranch("_nscin", &fNscin, tree);
  AddBranch("_ncerenkov", &fNcerenkov, tree);

  AddBranch("_nphotDet", &fNphotDet, tree);
  AddBranch("_nscinDet", &fNscinDet, tree);
  AddBranch("_ncerenkovDet", &fNcerenkovDet, tree);

}//CreateOutput

//_____________________________________________________________________________
void CompCal::FinishEvent() {

  //cell loop
  std::vector<Cell*>::iterator i;
  for(i = fCells->begin(); i != fCells->end(); ++i) {
    const Cell& c = **i;

    //deposited energy from cells
    fEdep += c.fE;

    //optical photons in cells
    fNphot += c.fNphot;
    fNscin += c.fNscin;
    fNcerenkov += c.fNcerenkov;

    //optical photons from cell photon detectors
    fNphotDet += c.fOpDet->fNphot;
    fNscinDet += c.fOpDet->fNscin;
    fNcerenkovDet += c.fOpDet->fNcerenkov;

  }//cell loop

}//WriteEvent

//_____________________________________________________________________________
void CompCal::ClearEvent() {

  fEdep = 0.;
  fX = 9999.;
  fY = 9999.;
  fZ = 9999.;
  fXyzE = -9999.;

  fNphot = 0;
  fNscin = 0;
  fNcerenkov = 0;

  fNphotDet = 0;
  fNscinDet = 0;
  fNcerenkovDet = 0;

}//ClearEvent

/*
//_____________________________________________________________________________
void CompCal::Add(std::vector<Detector*> *vec) {

  //add this detector and its parts to sensitive detectors
  vec->push_back(this);

  //cell loop
  std::vector<Cell*>::iterator i = fCells->begin();
  while(i != fCells->end()) (*i++)->Add(vec);

}//Add
*/

//_____________________________________________________________________________
void CompCal::AddBranch(const std::string& nam, Double_t *val, TTree *tree) {

  //add branch for one Double_t variable

  std::string name = fNam + nam; // branch name from detector name and variable name
  std::string leaf = name + "/D"; // leaflist for Double_t
  tree->Branch(name.c_str(), val, leaf.c_str()); // create the branch

}//AddBranch

//_____________________________________________________________________________
void CompCal::AddBranch(const std::string& nam, ULong64_t *val, TTree *tree) {

  //add branch for one ULong64_t variable

  std::string name = fNam + nam; // branch name from detector name and variable name
  std::string leaf = name + "/l"; // leaflist for Double_t
  tree->Branch(name.c_str(), val, leaf.c_str()); // create the branch

}//AddBranch
















