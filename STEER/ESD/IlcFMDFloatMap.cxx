/**************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.          *
 * All rights reserved.                                       *
 *                                                            *
 * Author: The ILC Off-line Project.                        *
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                            *
 * Permission to use, copy, modify and distribute this        *
 * software and its documentation strictly for non-commercial *
 * purposes is hereby granted without fee, provided that the  *
 * above copyright notice appears in all copies and that both *
 * the copyright notice and this permission notice appear in  *
 * the supporting documentation. The authors make no claims   *
 * about the suitability of this software for any purpose. It *
 * is provided "as is" without express or implied warranty.   *
 **************************************************************/
/* $Id: IlcFMDFloatMap.cxx 51728 2011-09-25 13:14:50Z hristov $ */
//__________________________________________________________
// 
// Map of per strip Float_t information
// the floats are indexed by the coordinates 
//     DETECTOR # (1-3)
//     RING ID    ('I' or 'O', any case)
//     SECTOR #   (0-39)
//     STRIP #    (0-511)
//
// 
// Created Mon Nov  8 12:51:51 2004 by Christian Holm Christensen
// 
#include "IlcFMDFloatMap.h"	//ILCFMDFLOATMAP_H
//__________________________________________________________
ClassImp(IlcFMDFloatMap)
#if 0
  ; // This is here to keep Emacs for indenting the next line
#endif

//__________________________________________________________
IlcFMDFloatMap::IlcFMDFloatMap(const IlcFMDMap& other)
  : IlcFMDMap(other),
    fTotal(fMaxDetectors * fMaxRings * fMaxSectors * fMaxStrips),
    fData(0)
{
  if (fTotal == 0) fTotal = 51200;
  fData = new Float_t[fTotal];
  // Copy constructor
  if (!other.IsFloat()) return;
  for (Int_t i = 0; i < fTotal; i++) fData[i] = other.AtAsFloat(i);
}

//__________________________________________________________
IlcFMDFloatMap::IlcFMDFloatMap(const IlcFMDFloatMap& other)
  : IlcFMDMap(other.fMaxDetectors,
              other.fMaxRings,
              other.fMaxSectors,
              other.fMaxStrips),
    fTotal(fMaxDetectors * fMaxRings * fMaxSectors * fMaxStrips),
    fData(0)
{
  if (fTotal == 0) fTotal = 51200;
  fData = new Float_t[fTotal];
  // Copy constructor
  for (Int_t i = 0; i < fTotal; i++)
    fData[i] = other.fData[i];
}

//__________________________________________________________
IlcFMDFloatMap::IlcFMDFloatMap()
  : IlcFMDMap(),
    fTotal(0),
    fData(0)
{
  // Constructor.
  // Parameters:
  //	None
}

//__________________________________________________________
IlcFMDFloatMap::IlcFMDFloatMap(Int_t maxDet,
			       Int_t maxRing,
			       Int_t maxSec,
			       Int_t maxStr)
  : IlcFMDMap(maxDet, maxRing, maxSec, maxStr),
    fTotal(fMaxDetectors * fMaxRings * fMaxSectors * fMaxStrips),
    fData(0)
{
  // Constructor.
  // Parameters:
  //	maxDet	Maximum number of detectors
  //	maxRing	Maximum number of rings per detector
  //	maxSec	Maximum number of sectors per ring
  //	maxStr	Maximum number of strips per sector
  if (fTotal == 0) fTotal = 51200;
  fData = new Float_t[fTotal];
  Reset(0);
}

//__________________________________________________________
IlcFMDFloatMap&
IlcFMDFloatMap::operator=(const IlcFMDFloatMap& other)
{
  // Assignment operator 
  if(&other != this){
    if(fMaxDetectors!= other.fMaxDetectors||
       fMaxRings    != other.fMaxRings||
       fMaxSectors  != other.fMaxSectors||
       fMaxStrips   != other.fMaxStrips){
      // allocate new memory only if the array size is different....
      fMaxDetectors = other.fMaxDetectors;
      fMaxRings     = other.fMaxRings;
      fMaxSectors   = other.fMaxSectors;
      fMaxStrips    = other.fMaxStrips;
      fTotal        = fMaxDetectors * fMaxRings * fMaxSectors * fMaxStrips;
      if (fTotal == 0) fTotal = 51200;
      if (fData) delete [] fData;
      fData = new Float_t[fTotal];
    }
    for (Int_t i = 0; i < fTotal; i++) fData[i] = other.fData[i];
  }
  return *this;
}


//__________________________________________________________
void
IlcFMDFloatMap::Reset(const Float_t& val)
{
  // Reset map to val
  for (Int_t i = 0; i < fTotal; i++) fData[i] = val;
}

//__________________________________________________________
Float_t&
IlcFMDFloatMap::operator()(UShort_t det, 
			   Char_t   ring, 
			   UShort_t sec, 
			   UShort_t str)
{
  // Get data
  // Parameters:
  //	det	Detector #
  //	ring	Ring ID
  //	sec	Sector #
  //	str	Strip #
  // Returns appropriate data
  return fData[CalcIndex(det, ring, sec, str)];
}

//__________________________________________________________
const Float_t&
IlcFMDFloatMap::operator()(UShort_t det, 
			   Char_t   ring, 
			   UShort_t sec, 
			   UShort_t str) const
{
  // Get data
  // Parameters:
  //	det	Detector #
  //	ring	Ring ID
  //	sec	Sector #
  //	str	Strip #
  // Returns appropriate data
  return fData[CalcIndex(det, ring, sec, str)];
}

//__________________________________________________________
// 
// EOF
// 

