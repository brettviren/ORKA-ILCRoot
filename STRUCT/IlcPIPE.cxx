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

/* $Id: IlcPIPE.cxx 15830 2006-11-14 17:11:17Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Beam pipe class                                                          //
//                                                                           //
//Begin_Html
/*
<img src="picts/IlcPIPEClass.gif">
*/
//End_Html
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcPIPE.h"
#include "IlcRun.h"
 
ClassImp(IlcPIPE)
 
//_____________________________________________________________________________
IlcPIPE::IlcPIPE()
{
  //
  // Default constructor for beam pipe
  //
}
 
//_____________________________________________________________________________
IlcPIPE::IlcPIPE(const char *name, const char *title)
       : IlcModule(name,title)
{
  //
  // Standard constructor for beam pipe
  //
  //PH  SetMarkerColor(7);
  //PH  SetMarkerStyle(2);
  //PH  SetMarkerSize(0.4);
}
 
