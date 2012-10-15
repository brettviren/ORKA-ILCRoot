/* $Id: IlcCFDataGrid.cxx 43839 2010-09-24 14:45:23Z rvernet $ */
/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
//--------------------------------------------------------------------//
//                                                                    //
// IlcCFDataGrid Class                                                //
// Class to handle observed data and correct them                     // 
//                                                                    //
// -- Author : S.Arcelli                                              //
//                                                                    //
// substantially modified by r. vernet                                //
//                                                                    //
//--------------------------------------------------------------------//
//
//
#include "TMath.h"
#include "IlcLog.h"
#include "IlcCFDataGrid.h"

//____________________________________________________________________
ClassImp(IlcCFDataGrid)

//____________________________________________________________________
IlcCFDataGrid::IlcCFDataGrid() : 
  IlcCFGridSparse(),
  fSelData(-1),
  fContainer(0x0)
{
  //
  // default constructor
  //
}

//____________________________________________________________________
IlcCFDataGrid::IlcCFDataGrid(const Char_t* name, const Char_t* title, const Int_t nVarIn, const Int_t * nBinIn) :
  IlcCFGridSparse(name,title,nVarIn,nBinIn),
  fSelData(-1),
  fContainer(0x0)
{
  //
  // main constructor
  //
  SumW2();// errors saved
}

//____________________________________________________________________
IlcCFDataGrid::IlcCFDataGrid(const Char_t* name, const Char_t* title, const IlcCFContainer &c, Int_t step) :  
  IlcCFGridSparse(name,title),
  fSelData(step),
  fContainer(&c)
{
  //
  // main constructor
  // assign directly the selection step
  //

  //simply clones the container's data at specified step
  fData = (THnSparse*) fContainer->GetGrid(fSelData)->GetGrid()->Clone();
  SumW2();
  IlcInfo(Form("retrieving measured data from Container %s at selection step %i.",fContainer->GetName(),fSelData));
}

//____________________________________________________________________
IlcCFDataGrid::IlcCFDataGrid(const IlcCFDataGrid& data) : 
  IlcCFGridSparse(data),
  fSelData(-1),
  fContainer(0x0)
{
  //
  // copy constructor
  //
  ((IlcCFDataGrid &)data).Copy(*this);
}

//____________________________________________________________________
IlcCFDataGrid::~IlcCFDataGrid()
{
  //
  // destructor
  //
}

//____________________________________________________________________
IlcCFDataGrid &IlcCFDataGrid::operator=(const IlcCFDataGrid &c)
{
  //
  // assigment operator
  //
  if (this != &c) c.Copy(*this);
  return *this;
} 

//____________________________________________________________________
void IlcCFDataGrid::ApplyEffCorrection(const IlcCFEffGrid &c)
{

  //
  // Apply the efficiency correction
  //
  if (c.GetNVar()!=GetNVar()) {
    IlcInfo("Different number of variables, cannot apply correction");
    return;
  }
  Divide(&c);
  IlcInfo(Form("correction applied on data grid %s with efficiency %s.",GetName(),c.GetName()));
}
//____________________________________________________________________
void IlcCFDataGrid::ApplyBGCorrection(const IlcCFDataGrid &c)
{

  //
  // Apply correction for background
  //
  if (c.GetNVar()!=GetNVar()) {
    IlcInfo("Different number of variables, cannot apply correction");
    return;
  }
  Add(&c,-1);
  IlcInfo(Form("background %s subtracted from data %s.",c.GetName(),GetName()));
}

//____________________________________________________________________
void IlcCFDataGrid::Copy(TObject& c) const
{
  // copy function
  Copy(c);
  IlcCFDataGrid& target = (IlcCFDataGrid &) c;
  target.fContainer=fContainer;
  target.fSelData=fSelData;
}
