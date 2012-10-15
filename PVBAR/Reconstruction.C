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
/* $Id: Reconstruction.C 15767 2006-11-10 02:23:22Z schutz $ */
//_________________________________________________________________________
// Macros performing the full reconstruction chain starting from Digits
// Use Case : 
//          root> .L Reconstruction.C++
//          root> rec("RE", "PVBAR EMCAL") --> does the reconstruction for 
//                                            PVBAR and EMCAL and stores the 
//                                            reconstructed particles in 
//                                            IlcESDs.root
// author  : Yves Schutz (CERN/SUBATECH)
// February 2004
//_________________________________________________________________________
 
#include "IlcReconstruction.h"
#include "TString.h"
#include "Riostream.h"
#include "TStopwatch.h"
#include "IlcTPCReconstructor.h"

void reco(TString opt="TVRE", TString name="all", TString cdb="") 
{
  IlcReconstruction rec ; 
  rec.SetUniformFieldTracking(kFALSE);
  rec.SetWriteESDfriend();
  rec.SetWriteAlignmentData();
  if ( name.Contains("TPC") ) {
   IlcTPCReconstructor::SetCtgRange(2.); // for pp events
   IlcTPCReconstructor::SetStreamLevel(1);
  }
  if ( !cdb.IsNull() ) 
    rec.SetDefaultStorage(cdb.Data() ) ; 

  if ( opt.Contains("W") )
    rec.SetInput("raw.root");

  if ( !opt.Contains("T") ) 
    rec.SetRunTracking("") ;
  else
    rec.SetRunTracking(name.Data()) ;

  if ( !opt.Contains("V") ) 
    rec.SetRunVertexFinder(kFALSE) ; 

  if ( opt.Contains("R") ) 
    rec.SetRunLocalReconstruction(name.Data()) ; 
  else 
    rec.SetRunLocalReconstruction("") ;

  if ( !opt.Contains("E") )
    rec.SetFillESD("") ; 
  else 
    rec.SetFillESD(name.Data()) ; 

  TStopwatch timer;
  timer.Start();
  rec.Run() ;
  timer.Stop();
  timer.Print();
}   
