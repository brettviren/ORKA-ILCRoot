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

/*
$Log: IlcTARGETChannelSPD.cxx,v $
Revision 1.1.1.1  2008/03/11 14:52:50  vitomeg
Initial commit

Revision 1.1.1.1  2006/11/16 16:27:40  garren
IlcRoot framework

Revision 1.2  2006/11/09 20:42:52  danieleb
Module with digitization process(only for barrels), test version

Revision 1.1.1.1  2006/09/04 14:56:24  vitomeg
initial revision

Revision 1.2  2005/11/03 13:09:19  hristov
Removing meaningless const declarations (linuxicc)

Revision 1.1  2005/10/11 12:31:50  masera
Preprocessor classes for SPD (Paul Nilsson)

*/

///////////////////////////////////////////////////////////////////////////
// IlcTARGETChannelSPD implementation by P. Nilsson 2005
// AUTHOR/CONTACT: Paul.Nilsson@cern.ch
//
// Objects of this class are stored in TObjArrays and should be
// interpreted as "bad" channels, i.e. either noisy or dead channels
// depending on where they are stored.
//
// A channel has the structure:
//
//                           Int_t fColumn:    Column in the SPD module
//                           Int_t fRow:       Row in the SPD module
//
// The class is used by the IlcTARGETPreprocessorSPD class to store noisy
// and dead channels in the calibration database for the SPD.
//
// A channel can be compared with other channels (the equilcty operator
// is defined). This is e.g. useful for the clustering algorithm. A noisy
// channel should not be used in the clustering
///////////////////////////////////////////////////////////////////////////

#include "IlcTARGETChannelSPD.h"

ClassImp(IlcTARGETChannelSPD)

//__________________________________________________________________________
IlcTARGETChannelSPD::IlcTARGETChannelSPD(void) :
fColumn(-1),
fRow(-1)
{
  // Default constructor
}


//__________________________________________________________________________
IlcTARGETChannelSPD::IlcTARGETChannelSPD(const IlcTARGETChannelSPD &ch) :
  TObject(ch)
{
  // Copy constructor

  fColumn = ch.fColumn;
  fRow = ch.fRow;
}

//__________________________________________________________________________
IlcTARGETChannelSPD& IlcTARGETChannelSPD::operator=(const IlcTARGETChannelSPD &ch)
{
  // Assignment operator
  
  // Guard against self-assignment
  if (this != &ch)
    {
      // Copy the data members
      fColumn = ch.fColumn;
      fRow = ch.fRow;
    }
  return *this;
}

//__________________________________________________________________________
Bool_t IlcTARGETChannelSPD::operator==(const IlcTARGETChannelSPD &channel) const
{
  // Equilcty operator
  // For comparisons between IlcTARGETChannelSPD objects

  return ( ((fColumn == channel.fColumn) && (fRow == channel.fRow)) );
}

//__________________________________________________________________________
IlcTARGETChannelSPD::IlcTARGETChannelSPD(Int_t column, Int_t row)
{
  // Constructor for already existing channel

  fColumn = column;
  fRow = row;
}
