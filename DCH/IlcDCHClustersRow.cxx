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
 
/* $Id: IlcDCHClustersRow.cxx,v 1.1 2008/04/22 09:43:37 tassiell Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Time Projection Chamber IlcTPCClusterRow  objects
//  -   clusters for given segment of TPC                                //
//
//  Origin: Marian Ivanov , GSI Darmstadt
//                                                                           //
//                                                                          //
///////////////////////////////////////////////////////////////////////////////
#include "IlcTPCcluster.h"
#include <TClass.h>
#include "IlcClusters.h"
#include "IlcTPCClustersRow.h"
#include <TDirectory.h>
#include <TClonesArray.h>


const Int_t kDefSize = 1;  //defalut size


ClassImp(IlcTPCClustersRow) 


//*****************************************************************************
//
//_____________________________________________________________________________
IlcTPCClustersRow::IlcTPCClustersRow() 
{  
  //
  //default constructor
  fNclusters=0;
}

//_____________________________________________________________________________
TObject *IlcTPCClustersRow::InsertCluster(const TObject *c) 
{    
  //
  // Add a simulated cluster copy to the list
  //
  if (fClass==0) {
    Error("IlcClusters", "class type not specified");
    return 0;
  }
  if(!fClusters) fClusters=new TClonesArray(fClass->GetName(),1000);
  TClonesArray &lclusters = *fClusters;
  return new(lclusters[fNclusters++]) IlcTPCcluster(*((IlcTPCcluster*)c));
}
//__________________________________________________________________________


TObject *IlcTPCClustersRow::Append(){
 //create new object return pointer to this object
 return fClusters->operator[](fClusters->GetEntriesFast());
}

