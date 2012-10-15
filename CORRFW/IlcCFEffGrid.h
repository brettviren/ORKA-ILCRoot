#ifndef ILCCFEFFGRID_H
#define ILCCFEFFGRID_H

/* $Id: IlcCFEffGrid.h 45914 2010-12-09 11:18:45Z rvernet $ */

//-----------------------------------------------------------------//
//                                                                 //
// IlcCFEffGrid Class                                              //
// Class to handle efficiency grids                                // 
//                                                                 //
//-----------------------------------------------------------------//

#include "IlcCFGridSparse.h"
#include "IlcCFContainer.h"
class TH1D;
class TH2D;
class TH3D;

class IlcCFEffGrid : public IlcCFGridSparse
{
 public:
  IlcCFEffGrid();
  IlcCFEffGrid(const Char_t* name,const Char_t* title, const Int_t nVarIn, const Int_t* nBinIn);
  IlcCFEffGrid(const Char_t* name,const Char_t* title, const IlcCFContainer &c);
  virtual ~IlcCFEffGrid();
  IlcCFEffGrid(const IlcCFEffGrid& eff);
  IlcCFEffGrid& operator=(const IlcCFEffGrid& eff);
  
  virtual Double_t GetAverage() const ;
  virtual Int_t GetSelNumStep() const {return fSelNum;};
  virtual Int_t GetSelDenStep() const {return fSelDen;};
  virtual TH1*  Project(Int_t ivar1, Int_t ivar2=-1,Int_t ivar3=-1) const;
  virtual IlcCFGridSparse*  Project(Int_t, const Int_t*, const Double_t*, const Double_t*, Bool_t) const {IlcWarning("should not be used"); return 0x0;}
  virtual IlcCFEffGrid* MakeSlice(Int_t nVars, const Int_t* vars, const Double_t* varMin, const Double_t* varMax, Bool_t useBins=0) const;

  //Efficiency calculation
  virtual void  CalculateEfficiency(Int_t istep1, Int_t istep2, Option_t *option ="B" /*binomial*/);
  virtual IlcCFGridSparse*  GetNum() const {return fContainer->GetGrid(fSelNum);};
  virtual IlcCFGridSparse*  GetDen() const {return fContainer->GetGrid(fSelDen);};
  virtual void  SetContainer(const IlcCFContainer &c) {fContainer=&c;};

 private:
  const IlcCFContainer *fContainer; //pointer to the input IlcContainer
  Int_t fSelNum;                    //numerator selection step
  Int_t fSelDen;                    //denominator selection step
  
  ClassDef(IlcCFEffGrid,1);
};
    
#endif
