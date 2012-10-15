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

/* $Id: IlcDIPO.cxx 15830 2006-11-14 17:11:17Z hristov $ */

///////////////////////////////////////////////////////////////////////////////
//                                                                           //
//  Magnetic Dipole                                                          //
//  This class contains the description of the base class for the            //
//  magnetic dipole                                                          //
//                                                                           //
//Begin_Html
/*
<img src="picts/IlcDIPOClass.gif">
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
//                                                                           //
///////////////////////////////////////////////////////////////////////////////

#include "IlcDIPO.h"
#include "IlcRun.h"
#include "IlcLog.h"
 
ClassImp(IlcDIPO)
 
//_____________________________________________________________________________
IlcDIPO::IlcDIPO()
{
  //
  // Default constructor for the Dipole Magnet
  //
}
 
//_____________________________________________________________________________
IlcDIPO::IlcDIPO(const char *name, const char *title)
  : IlcModule(name,title)
{
  //
  // Standard constructor for the Magnetic Dipole
  //
  //PH  SetMarkerColor(7);
  //PH  SetMarkerStyle(2);
  //PH  SetMarkerSize(0.4);
}
 
//_____________________________________________________________________________
void IlcDIPO::Init()
{
  //
  // Initialise Magnetic Dipole after it has been built
  //
  Int_t i;
  //
  if(IlcLog::GetGlobalDebugLevel()>0) {
    printf("\n%s: ",ClassName());
    for(i=0;i<35;i++) printf("*");
    printf(" DIPO_INIT ");
    for(i=0;i<35;i++) printf("*");
    printf("\n%s: ",ClassName());
    //
    // Here the ABSO initialisation code (if any!)
    for(i=0;i<80;i++) printf("*");
    printf("\n");
  }
}
 
