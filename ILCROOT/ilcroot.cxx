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

/* $Id: ilcroot.cxx 58023 2012-07-31 16:29:45Z hristov $ */

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// ilcroot                                                              //
//                                                                      //
// Main program used to create ilcroot application.                     //
//                                                                      //
//                                                                      //
// To be able to communicate between the FORTRAN code of GEANT and the  //
// ROOT data structure, a number of interface routines have been        //
// developed.                                                           //
//Begin_Html
/*
<img src="picts/newg.gif">
*/
//End_Html
//////////////////////////////////////////////////////////////////////////

//Standard Root includes
#include <TROOT.h>
#include <TRint.h>
#include <TFile.h>
#include <IlcRun.h>
#include "Riostream.h"
#include "ARVersion.h"

#if defined __linux
//On linux Fortran wants this, so we give to it!
int xargv=0;
int xargc=0;
#endif

#ifdef FORTRAN_G95
extern "C" void g95_runtime_start();
#endif

#if defined WIN32 
  extern "C" int __fastflag=0; 
  extern "C" int _pctype=0; 
  extern "C" int __mb_cur_max=0; 
#endif 

using std::cout;
using std::endl;

//_____________________________________________________________________________
int main(int argc, char **argv)
{
  //
  // gIlc main program.
  // It creates the main Geant3 and IlcRun objects.
  //
  // The Hits are written out after each track in a ROOT Tree structure TreeH,
  // of the file gilc.root. There is one such tree per event. The kinematics
  // of all the particles that produce hits, together with their genealogy up
  // to the primary tracks is stared in the gilc.root file in an other tree
  // TreeK of which exists one per event. Finally the information of the events
  // in the run is stored in the same file in the tree TreeE, containing the
  // run and event number, the number of vertices, tracks and primary tracks
  // in the event.
  
  for ( int i = 1; i < argc; ++i ) 
  {
    TString argument(argv[i]);
    
    if (argument=="--version")
    {      
      cout << "ilcroot " << ILCROOT_SVN_REVISION << " " << ILCROOT_SVN_BRANCH << endl;
      return 0;
    }    
  }
  
  // Create new configuration 
  
  new IlcRun("gIlc","The ILC Off-line Simulation Framework");
    
  // Start interactive geant
  
  TRint *theApp = new TRint("ilcroot", &argc, argv);
#ifdef FORTRAN_G95
  g95_runtime_start();
#endif
  
  // --- Start the event loop ---
  theApp->Run();
  
  return(0);
}

