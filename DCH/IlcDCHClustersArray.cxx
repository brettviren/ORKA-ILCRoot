/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

/* $Id: IlcDCHClustersArray.cxx,v 1.1 2008/04/22 09:43:37 tassiell Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Time Projection Chamber clusters objects                                //
//
//  Origin: Marian Ivanov , GSI Darmstadt
//                                                                           //
//                                                                           //
//                                                                          //
///////////////////////////////////////////////////////////////////////////////
#include "IlcTPCParam.h" 
#include "IlcSegmentArray.h" 
#include "IlcComplexCluster.h"
#include "IlcClusters.h"
#include "IlcClustersArray.h" 
#include "IlcTPCClustersRow.h" 

#include "IlcTPCClustersArray.h"
#include "TClonesArray.h"
#include "TDirectory.h"
#include <TClass.h>



//_____________________________________________________________________________

ClassImp(IlcTPCClustersArray) 

IlcTPCClustersArray::IlcTPCClustersArray()
{
  fParam = 0;
  SetClass("IlcTPCClustersRow");
}

IlcTPCClustersArray::~IlcTPCClustersArray()
{
  //
}



IlcTPCClustersRow * IlcTPCClustersArray::GetRow(Int_t sector,Int_t row) 
{
  //
  //return clusters ((IlcTPCClustersRow *) per given sector and padrow
  //
  if (fParam==0) return 0;
  Int_t index = ((IlcTPCParam*)fParam)->GetIndex(sector,row);  
  return (IlcTPCClustersRow *)(*this)[index];
}

IlcTPCClustersRow *  IlcTPCClustersArray::CreateRow(Int_t sector, Int_t row)
{
  //
  //create digits row  
  //
  //if row just exist - delete it
  IlcTPCParam * param = (IlcTPCParam*)fParam;
  Int_t index = param->GetIndex(sector,row);  
  IlcTPCClustersRow * clusters = (IlcTPCClustersRow *)(*this)[index];
  if (clusters !=0) delete clusters;

  clusters = (IlcTPCClustersRow *) AddSegment(index);
  if (clusters == 0) return 0;
  return clusters;
}

IlcTPCClustersRow * IlcTPCClustersArray::LoadRow(Int_t sector,Int_t row)
{
  //
  //return clusters ((IlcTPCClustersRow *) per given sector and padrow
  //
  if (fParam==0) return 0;
  Int_t index = ((IlcTPCParam*)fParam)->GetIndex(sector,row);  
  return (  IlcTPCClustersRow *) LoadSegment(index);
}

Bool_t  IlcTPCClustersArray::StoreRow(Int_t sector,Int_t row)
{
  //
  //return clusters ((IlcTPCClustersRow *) per given sector and padrow
  //
  if (fParam==0) return 0;
  Int_t index = ((IlcTPCParam*)fParam)->GetIndex(sector,row);  
  StoreSegment(index);
  return kTRUE;
}

Bool_t  IlcTPCClustersArray::ClearRow(Int_t sector,Int_t row)
{
  //
  //return clusters ((IlcTPCDigitsRow *) per given sector and padrow
  //
  if (fParam==0) return 0;
  Int_t index = ((IlcTPCParam*)fParam)->GetIndex(sector,row);  
  ClearSegment(index);
  return kTRUE;
}



Bool_t IlcTPCClustersArray::Setup(const IlcDetectorParam *param)
{
  //
  //setup  function to adjust array parameters
  //
  if (param==0) return kFALSE;
  fParam = (IlcDetectorParam *)param;
  return MakeArray(((IlcTPCParam*)fParam)->GetNRowsTotal());

}
Bool_t IlcTPCClustersArray::Update()
{
  //
  //setup  function to adjust array parameters
  //
  if (fParam ==0 ) return kFALSE;
  if (fTree!=0) return MakeDictionary( ((IlcTPCParam*)fParam)->GetNRowsTotal()) ;
  ((IlcTPCParam*)fParam)->Update();
  return MakeArray(((IlcTPCParam*)fParam)->GetNRowsTotal());
}


IlcSegmentID * IlcTPCClustersArray::NewSegment()
{
  //
  //create object according class information 
  if (fClusterType==0) {
    Error("IlcTPCCLustersArray", "cluster type isn't adjusted");
    return 0;
  }
  IlcSegmentID *segment=IlcSegmentArray::NewSegment();
  ((IlcTPCClustersRow*)segment)->SetClass(fClusterType->GetName()); 
  ((IlcTPCClustersRow*)segment)->SetArray(100);
  return segment;
}
