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

/* $Id: IlcDCHhit.cxx,v 1.3 2009/06/30 10:38:10 cgatto Exp $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Hit object for the DCH                                                   //
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDCHhit.h"

ClassImp(IlcDCHhit)

//_____________________________________________________________________________
IlcDCHhit::IlcDCHhit():IlcHit()
{
  //
  // IlcDCHhit default constructor
  //
  fTime=0.;
}

//_____________________________________________________________________________
IlcDCHhit::IlcDCHhit(Int_t shunt, Int_t track, UShort_t superlayer, UShort_t Celring, UShort_t wire, ULong_t celid, UShort_t trkStat, Float_t *hits, UShort_t ClMult,Int_t status) : IlcHit(shunt, track)
{
  //
  // Create a DCH hit
  //
  // Store detector number
  fSuperlayer = superlayer;
  fCelRing    = Celring;
  fWire       = wire;
  fTrkStat    = trkStat;
  fCelId      = celid;
  fClMult     = ClMult;
  fStatus    =status;

  // Store position 
  fX        = hits[0];
  fY        = hits[1];
  fZ        = hits[2];
  fTime     = hits[3];
  
}

//_____________________________________________________________________________
IlcDCHhit::~IlcDCHhit()
{
  //
  // IlcDCHhit destructor
  //

}
