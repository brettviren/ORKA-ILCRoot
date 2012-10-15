// Author: Stefano Carrazza 2010

/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 * See http://ilcinfo.cern.ch/Offline/IlcRoot/License.html for          *
 * full copyright notice.                                                 *
 **************************************************************************/
 
#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TEveElement.h>

#include <IlcEveEventManager.h>
#include <IlcEveLego.h>
#endif

IlcEveLego* lego(){

   IlcEveEventManager *mng = IlcEveEventManager::GetMaster();
   IlcEveLego *lego = dynamic_cast<IlcEveLego*>(mng->FindGlobal("LegoHisto2D"));

   if ( lego == 0) {
      lego = new IlcEveLego();
      mng->InsertGlobal("LegoHisto2D",lego);
   } else {
      lego->Update();
   } 

   return lego;
}
