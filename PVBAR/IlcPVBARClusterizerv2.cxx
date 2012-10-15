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

/* $Id: IlcPVBARClusterizerv2.cxx 20956 2007-09-26 14:22:18Z cvetan $ */

//////////////////////////////////////////////////////////////////////////////
//  Clusterization class for ORKA reconstruction.
// Performs clusterization (collects neighbouring active cells)
// It differs from IlcPVBARClusterizerv1 in neighbour definition only
//*-- Author: Boris Polichtchouk, IHEP

// --- ROOT system ---
#include "TBenchmark.h"

// --- Standard library ---

// --- IlcRoot header files ---
#include "IlcLog.h"
#include "IlcPVBARClusterizerv2.h"
#include "IlcPVBAREvalRecPoint.h"
#include "IlcPVBARRecCpvManager.h"
#include "IlcPVBARRecEmcManager.h"
#include "IlcPVBARGeometry.h"
#include "IlcPVBARDigit.h"

ClassImp(IlcPVBARClusterizerv2)

//____________________________________________________________________________
IlcPVBARClusterizerv2::IlcPVBARClusterizerv2() : IlcPVBARClusterizerv1() 
{}

//____________________________________________________________________________
IlcPVBARClusterizerv2::IlcPVBARClusterizerv2(IlcPVBARGeometry *geom):
IlcPVBARClusterizerv1(geom)
{}

//____________________________________________________________________________
void IlcPVBARClusterizerv2::GetNumberOfClustersFound(int* numb) const
{
  // Returns the number of found EMC and CPV rec.points

  numb[0] = fEMCRecPoints->GetEntries();  
  numb[1] = fCPVRecPoints->GetEntries();  
}

//____________________________________________________________________________
void IlcPVBARClusterizerv2::Digits2Clusters(Option_t* option)
{
  // Steering method

  if(strstr(option,"tim"))
    gBenchmark->Start("PVBARClusterizer"); 
  
  if(strstr(option,"print"))
    Print() ; 
}
//   IlcPVBARGetter * gime = IlcPVBARGetter::Instance() ; 

//   TFolder* wPoolF =  gime->PVBARLoader()->GetDetectorDataFolder();
  
//   TObjArray* wPool = new TObjArray(400);
//   wPool->SetName("SmartPoints");
//   wPoolF->Add(wPool);
//   wPoolF->Add(this);

//   Int_t nevents = gime->MaxEvent() ;
//   Int_t ievent ;

//   for(ievent = 0; ievent<nevents; ievent++) {
    
//     gime->Event(ievent,"D") ;
    
//     IlcInfo(Form("MakeClusters invoked..")) ;
//     MakeClusters() ;
//     IlcInfo(Form("MakeClusters done.")) ;


//     //SmartRecPoints will communicate with wPool.

//     IlcPVBAREvalRecPoint* rp=0;

//     // CPV reconstruction

//     IlcPVBARRecCpvManager* recCpv = new IlcPVBARRecCpvManager();
//     wPoolF->Add(recCpv);

//     Int_t iPoint; //loop variable

//     for(iPoint=0; iPoint<gime->CpvRecPoints()->GetEntriesFast(); iPoint++) {
//       rp = new IlcPVBAREvalRecPoint(iPoint, kTRUE);
//       rp->MakeJob();
//     }

//     IlcPVBAREvalRecPoint pt;
//     pt.UpdateWorkingPool();

//     TObjArray * cpvRecPoints = gime->CpvRecPoints() ; 
//     Int_t nOldCpv = cpvRecPoints->GetEntries();
//     cpvRecPoints->Delete();
//     cpvRecPoints->Compress();

//     Int_t i; //loop variable

//     for(i=0; i<wPool->GetEntries(); i++)
//       cpvRecPoints->Add(wPool->At(i));

//     wPool->Clear();
//     wPool->Compress();

//     wPoolF->Remove(recCpv);
//     delete recCpv;

//     IlcInfo(Form("       %d", gime->CpvRecPoints()->GetEntries() )) ;
//     IlcInfo(Form("       %d cpvRecPoints", cpvRecPoints->GetEntries() )) ;


//     // Now Emc reconstruction

//     IlcPVBARRecEmcManager* recEmc = new IlcPVBARRecEmcManager();
//     wPoolF->Add(recEmc);

//     for(iPoint=0; iPoint<gime->EmcRecPoints()->GetEntriesFast(); iPoint++) {
//       rp = new IlcPVBAREvalRecPoint(iPoint, kFALSE);
//       rp->MakeJob();
//     }

//     pt.UpdateWorkingPool();

//     TObjArray * emcRecPoints = gime->EmcRecPoints() ; 
//     Int_t nOldEmc = emcRecPoints->GetEntries();
//     emcRecPoints->Delete();
//     emcRecPoints->Compress();

//     for(i=0; i<wPool->GetEntries(); i++)
//       emcRecPoints->Add(wPool->At(i));

//     wPool->Clear();
//     wPool->Compress();

//     wPoolF->Remove(recEmc);
//     delete recEmc;

//     TString message ; 
//     message  = "       %d  OLD cpvRecPoints\n" ; 
//     message += "       %d\n" ; 
//     message += "       %d cpvRecPoints\n" ; 

//     message += "       %d OLD emcRecPoints " ; 
//     message += "       %d\n" ;
//     message += "       %d emcRecPoints\n" ;

//     IlcInfo(Form("%s", message.Data(), 
// 	 nOldCpv, 
// 	 gime->CpvRecPoints()->GetEntries(),cpvRecPoints->GetEntries(), 
// 	 nOldEmc, 
// 	 gime->EmcRecPoints()->GetEntries(), emcRecPoints->GetEntries() )); 
    

//     WriteRecPoints();


//   } // loop over events

//   if(strstr(option,"tim")) {
//     gBenchmark->Stop("PVBARClusterizer");
//     IlcInfo(Form("took %f seconds for Clusterizing", gBenchmark->GetCpuTime("PVBARClusterizer") )) ;
//   }
// }

//____________________________________________________________________________
Int_t IlcPVBARClusterizerv2::AreNeighbours(IlcPVBARDigit* d1, IlcPVBARDigit* d2) const
{
  // Points are neighbours if they have common edge.
  // Points with common vertex are NOT neighbours.
  // This treatment of neighbourship is the part of 
  // ORKA algorithm of clusterization.

  // Gives the neighbourness of two digits = 0 are not neighbour but continue searching 
  //                                       = 1 are neighbour
  //                                       = 2 are not neighbour but do not continue searching
  // The order of d1 and d2 is important: first (d1) should be a digit already in a cluster 
  // which is compared to a digit (d2)  not yet in a cluster  

  Int_t rv = 0 ; 

  Int_t relid1[4] ; 
  fGeom->AbsToRelNumbering(d1->GetId(), relid1) ; 

  Int_t relid2[4] ; 
  fGeom->AbsToRelNumbering(d2->GetId(), relid2) ; 
 
  if ( (relid1[0] == relid2[0]) && (relid1[1]==relid2[1]) ) { // inside the same PVBAR module and the same PPSD Module 
    Int_t rowdiff = TMath::Abs( relid1[2] - relid2[2] ) ;  
    Int_t coldiff = TMath::Abs( relid1[3] - relid2[3] ) ;  
    
    if ( ( (coldiff < 1) && (rowdiff <= 1) ) || ( ( coldiff <= 1 )  && ( rowdiff < 1 ) ) ){
      rv = 1 ; 
    }
    else {
      if((relid2[2] > relid1[2]) && (relid2[3] > relid1[3]+1)) 
	rv = 2; //  Difference in row numbers is too large to look further 
    }

  } 
  else {
    
    if( (relid1[0] < relid2[0]) || (relid1[1] < relid2[1]) )  
      rv=2 ;

  }

  return rv ; 

}
