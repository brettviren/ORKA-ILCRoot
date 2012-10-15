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

/* $Id: IlcFRAME.cxx 27058 2008-07-02 19:55:01Z morsch $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Space Frame                                                              //
//  This class contains the description of the Space Frame geometry          //
//                                                                           //
//Begin_Html
/*
<img src="picts/IlcFRAMEClass.gif">
</pre>
<br clear=left>
<font size=+2 color=red>
<p>The responsible person for this module is
<a href="mailto:andreas.morsch@cern.ch">Andreas Morsch</a>.
</font>
<pre>
*/
//End_Html
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcFRAME.h"
#include "IlcRun.h"
 
ClassImp(IlcFRAME)

 
//_____________________________________________________________________________
IlcFRAME::IlcFRAME():
  fRefVolumeId1(0),
  fRefVolumeId2(0)
{
  //
  // Default constructor
  //
}
 
//_____________________________________________________________________________
IlcFRAME::IlcFRAME(const char *name, const char *title)
    : IlcModule(name,title),
      fRefVolumeId1(0),
      fRefVolumeId2(0)
{
  //
  // Standard constructor
  //
    
  //PH  SetMarkerColor(7);
  //PH  SetMarkerStyle(2);
  //PH  SetMarkerSize(0.4);
}
 
