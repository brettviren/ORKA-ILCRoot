/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcAODCaloCells.h 56373 2012-05-10 14:30:16Z hristov $ */

//-------------------------------------------------------------------------
//     AOD class to store calorimeter cell data
//     Author: Markus Oldenburg, CERN
//-------------------------------------------------------------------------

#ifndef ILCAODCELLS_H
#define ILCAODCELLS_H

#include <IlcVCaloCells.h>
#include <TMath.h>

class IlcAODCaloCells : public IlcVCaloCells
{
 public:
  IlcAODCaloCells();
  IlcAODCaloCells(const char* name, const char* title, VCells_t ttype=kUndef);
  IlcAODCaloCells(const IlcAODCaloCells& cells); 
  IlcAODCaloCells& operator=(const IlcAODCaloCells& cells);
  virtual ~IlcAODCaloCells();

  virtual IlcVCaloCells* CopyCaloCells(Bool_t all) const;
  virtual void    Copy(TObject &obj) const;
  void            Clear(const Option_t*);
  void            CreateContainer(Short_t nCells);
  void            DeleteContainer();
  void            Sort();
  
  inline Bool_t   GetCell(Short_t pos, Short_t &cellNumber, Double_t &amplitude,  Double_t &time, Short_t &mclabel,      Double_t &efrac) const ;
  Bool_t          SetCell(Short_t pos, Short_t  cellNumber, Double_t  amplitude,  Double_t  time, Short_t  mclabel = -1, Double_t  efrac = 0.)  ;
  
  Short_t         GetNumberOfCells() const  { return fNCells ; }
  void            SetNumberOfCells(Int_t n) { fNCells = n    ; }
  
  inline Double_t GetCellAmplitude(Short_t cellNumber);
  inline Short_t  GetCellPosition(Short_t cellNumber);
  inline Double_t GetCellTime(Short_t cellNumber);
  
  inline Double_t GetAmplitude(Short_t pos) const;
  inline Short_t  GetCellNumber(Short_t pos) const;
  inline Double_t GetTime(Short_t pos) const;
  
  Bool_t          IsEMCAL() const { return (fType == kEMCALCell); }
  Bool_t          IsPHOS()  const { return (fType == kPHOSCell) ; }
  
  Char_t          GetType() const { return fType;}
  void            SetType(Char_t ttype) { fType=ttype; }
  
  // MC & embedding
  inline Short_t  GetCellMCLabel(Short_t cellNumber) ;
  inline Short_t  GetMCLabel(Short_t pos) const ;
  
  inline Double_t GetCellEFraction(Short_t cellNumber) ;
  inline Double_t GetEFraction(Short_t pos) const ;  
  
  inline void     SetEFraction    (Short_t pos,         Double32_t efrac) ;
  inline void     SetCellEFraction(Short_t cellNumber,  Double32_t efrac) ;
  
 protected:
  
  Int_t       fNCells;       // Number of cells
  Short_t    *fCellNumber;   //[fNCells] array of cell numbers
  Double32_t *fAmplitude;    //[fNCells][0.,0.,16] array with cell amplitudes (= energy!)
  Double32_t *fTime;         //[fNCells][0.,0.,16] array with cell times
  Double32_t *fEFraction;    //[fNCells][0.,0.,16] array with fraction of MC energy and data - for embedding
  Short_t    *fMCLabel;      //[fNCells] array of MC labels
  Bool_t      fIsSorted;     //! true if cell arrays are sorted by index
  Char_t      fType;         // Cell type
  
  ClassDef(IlcAODCaloCells, 3);
  
};

Bool_t IlcAODCaloCells::GetCell(Short_t pos, Short_t &cellNumber, Double_t &amplitude, 
                                Double_t &time, Short_t & mclabel, Double_t & efrac) const 
{ 
  if (pos>=0 && pos<fNCells) 
  {
    cellNumber = fCellNumber[pos];
    amplitude  = fAmplitude[pos];
    
    if(fTime)      time    = fTime[pos];
    else           time    =-1.;
    if(fMCLabel)   mclabel = fMCLabel[pos];
    else           mclabel =-1 ; 
    if(fEFraction) efrac   = fEFraction[pos];
    else           efrac   = 0 ;
    
    return kTRUE;
    
  } else {
    return kFALSE;
  }
}

Double_t IlcAODCaloCells::GetCellAmplitude(Short_t cellNumber)
{ 
  if (!fIsSorted) {
    Sort();
    fIsSorted=kTRUE;
  }

  Short_t pos = TMath::BinarySearch(fNCells, fCellNumber, cellNumber);
  if (pos>=0 && fCellNumber[pos] == cellNumber) {
    return fAmplitude[pos];
  } else {
    return 0.;
  }
}

Double_t IlcAODCaloCells::GetCellTime(Short_t cellNumber)
{ 
  if(!fTime) return -1;
  
  if (!fIsSorted) {
    Sort();
    fIsSorted=kTRUE;
  }
  
  Short_t pos = TMath::BinarySearch(fNCells, fCellNumber, cellNumber);
  if (pos>=0 && pos < fNCells && fCellNumber[pos] == cellNumber) {
    return fTime[pos];
  } else {
    return -1.;
  }
}

Double_t IlcAODCaloCells::GetAmplitude(Short_t pos) const 
{ 
  if (pos>=0 && pos<fNCells) {
    return fAmplitude[pos];
  } else {
    return 0.;
  }
}

Double_t IlcAODCaloCells::GetTime(Short_t pos) const 
{ 
  if (pos>=0 && pos<fNCells && fTime) {
    return fTime[pos];
  } else {
    return -1.;
  }
}

Short_t IlcAODCaloCells::GetCellNumber(Short_t pos) const 
{ 
  if (pos>=0 && pos<fNCells) {
    return fCellNumber[pos];
  } else {
    return fNCells;
  }
}

Short_t IlcAODCaloCells::GetCellPosition(Short_t cellNumber)
{ 
  if (!fIsSorted) {
    Sort();
    fIsSorted=kTRUE;
  }

   Int_t nabove, nbelow, middle;
   Short_t pos = -1;

   nabove = fNCells + 1;
   nbelow = 0;
   while (nabove - nbelow > 1) {
      middle = (nabove + nbelow) / 2;
      if (cellNumber == fCellNumber[middle-1]) {
	  pos =   middle - 1;
	  break;
      }
      if (cellNumber  < fCellNumber[middle-1]) nabove = middle;
      else                                     nbelow = middle;
   }

  return pos;
}

Short_t IlcAODCaloCells::GetMCLabel(Short_t pos) const 
{ 
  if (pos>=0 && pos<fNCells && fMCLabel) {
    return fMCLabel[pos];
  } else {
    return 0.;
  }
}

Double_t IlcAODCaloCells::GetEFraction(Short_t pos) const 
{ 
  if (pos>=0 && pos<fNCells && fEFraction) {
    return fEFraction[pos];
  } else {
    return 0.;
  }
}

Short_t IlcAODCaloCells::GetCellMCLabel(Short_t cellNumber)
{ 
  if(!fMCLabel) return -1;
  
  if (!fIsSorted) {
    Sort();
    fIsSorted=kTRUE;
  }
  
  Short_t pos = TMath::BinarySearch(fNCells, fCellNumber, cellNumber);
  if (pos>=0 && fCellNumber[pos] == cellNumber) {
    return fMCLabel[pos];
  } else {
    return 0.;
  }
}

Double_t IlcAODCaloCells::GetCellEFraction(Short_t cellNumber)
{ 
  if(!fEFraction) return 0;

  if (!fIsSorted) {
    Sort();
    fIsSorted=kTRUE;
  }
  
  Short_t pos = TMath::BinarySearch(fNCells, fCellNumber, cellNumber);
  if (pos>=0 && pos < fNCells && fCellNumber[pos] == cellNumber) {
    return fEFraction[pos];
  } else {
    return -1.;
  }
}

void IlcAODCaloCells::SetEFraction(Short_t pos,  Double32_t efrac)
{
  // Sets the fraction of energy from MC with respect to data at the given position
  
  
  if (pos>=0 && pos < fNCells) 
  {
    if(!fEFraction) fEFraction = new Double32_t[fNCells];
    fEFraction[pos]  = efrac;
  } 
}

void IlcAODCaloCells::SetCellEFraction(Short_t cellNumber, Double32_t efrac)
{ 
  if (!fIsSorted) {
    Sort();
    fIsSorted=kTRUE;
  }
  
  Short_t pos = TMath::BinarySearch(fNCells, fCellNumber, cellNumber);
  if (pos>=0 && pos < fNCells && fCellNumber[pos] == cellNumber) 
  {
    if(!fEFraction) fEFraction = new Double32_t[fNCells];
    fEFraction[pos] = efrac;
  } 
}


#endif
