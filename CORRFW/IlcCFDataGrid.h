#ifndef ILCCFDATAGRID_H
#define ILCCFDATAGRID_H

/* $Id: IlcCFDataGrid.h 43839 2010-09-24 14:45:23Z rvernet $ */

//--------------------------------------------------------------------//
//                                                                    //
// IlcCFDataGrid Class                                             //
// Class to handle observed data and correct them                     // 
//                                                                    //
//--------------------------------------------------------------------//

#include "IlcCFGridSparse.h"
#include "IlcCFEffGrid.h"
#include "IlcCFContainer.h"

class IlcCFDataGrid : public IlcCFGridSparse
{
 public:
  IlcCFDataGrid();
  IlcCFDataGrid(const Char_t* name,const Char_t* title, const IlcCFContainer &c, Int_t step);      //create data grid from container
  IlcCFDataGrid(const Char_t* name,const Char_t* title, const Int_t nVarIn, const Int_t* nBinIn);  //create empty data grid to fill it yourself
  IlcCFDataGrid(const IlcCFDataGrid& c);
  IlcCFDataGrid& operator=(const IlcCFDataGrid& c);
  virtual ~IlcCFDataGrid();

  virtual Int_t GetSelDataStep() const {return fSelData;};

  // Methods for handling/correcting data 

  virtual const IlcCFGridSparse*  GetData() {return fContainer->GetGrid(fSelData);};
  virtual void  ApplyEffCorrection(const IlcCFEffGrid &eff);
  virtual void  ApplyBGCorrection(const IlcCFDataGrid &c);
  //basic operations
  virtual void   Copy(TObject& data) const;
 
  
 private:
  Int_t fSelData; //sel step of the observed data 
  const IlcCFContainer *fContainer; //pointer to the input data Container
  ClassDef(IlcCFDataGrid,2);
};
    
#endif
