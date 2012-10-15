#ifndef ILCVCALOCELLS_H
#define ILCVCALOCELLS_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//
//     Virtual class to access calorimeter cell data
//     Author: Gustavo Conesa Balbastre INFN-LNF
//
//-------------------------------------------------------------------------


#include <TNamed.h>
#include <TMath.h>

class IlcVCaloCells : public TNamed
{
 public:

  enum VCells_t {kUndef = -1, 
                 kPHOSCell, 
                 kEMCALCell};

  IlcVCaloCells() : TNamed()  {;}
  IlcVCaloCells(const char* name, const char* title) : TNamed(name, title) {;}
  IlcVCaloCells(const IlcVCaloCells& cells) : TNamed(cells.GetName(), cells.GetTitle()) {;}
  IlcVCaloCells & operator=(const IlcVCaloCells& cells ) ;
  virtual ~IlcVCaloCells()    {;}
  void Clear(const Option_t*) {;}
  
  virtual Bool_t   IsEMCAL() const         = 0;
  virtual Bool_t   IsPHOS()  const         = 0;
  virtual Char_t   GetType() const         = 0;
  virtual void     SetType(Char_t ttype)   = 0;

  virtual void     CreateContainer(Short_t nCells) = 0;
  virtual void     DeleteContainer()               = 0;
  virtual void     Sort()                          = 0;
  
  virtual Bool_t   GetCell(Short_t pos, Short_t &cellNumber, Double_t &amplitude, Double_t &time, Short_t &mclabel,    Double_t  &efrac) const = 0;
  virtual Bool_t   SetCell(Short_t pos, Short_t  cellNumber, Double_t  amplitude, Double_t  time, Short_t  mclabel=-1, Double_t   efrac=0)     = 0;
  
  virtual Short_t  GetNumberOfCells() const              = 0;
  virtual void     SetNumberOfCells(Int_t n)             = 0;
  
  virtual Double_t GetCellAmplitude(Short_t cellNumber)  = 0;
  virtual Double_t GetCellTime(Short_t cellNumber)       = 0;
  virtual Short_t  GetCellPosition(Short_t cellNumber)   = 0;
  
  virtual Double_t GetAmplitude(Short_t pos)  const      = 0;
  virtual Double_t GetTime(Short_t pos)       const      = 0;
  virtual Short_t  GetCellNumber(Short_t pos) const      = 0;

  virtual void           Copy(TObject &obj)        const = 0;
  virtual IlcVCaloCells* CopyCaloCells(Bool_t all) const = 0;

  // MC & embedding
  virtual Short_t  GetCellMCLabel(Short_t cellNumber)    = 0;
  virtual Short_t  GetMCLabel(Short_t pos) const         = 0;

  virtual Double_t GetCellEFraction(Short_t cellNumber)  = 0;
  virtual Double_t GetEFraction(Short_t pos) const       = 0;

  virtual void     SetCellEFraction(Short_t cellNumber, Double_t efrac) = 0;
  virtual void     SetEFraction    (Short_t pos,        Double_t efrac) = 0;
  
  ClassDef(IlcVCaloCells, 0);
};

#endif
