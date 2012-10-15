// $Id: tpc_gui.C 42007 2010-07-01 13:18:48Z mtadel $
// Main authors: Matevz Tadel & Alja Mrak-Tadel: 2006, 2007

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

// Function to spawn a gui for reading rootified raw-data from TPC sector test.
//
// To use:
// a) select IlcEveTPCLoader entry in the list-tree view;
//    you'll get a dialog to steer the data-loading process in an adjacent window
// b) to select a ROOT file containing the raw-data double-click on 'File:'
//    text entry to spawn a file-dialog or type in the name
// c) click open to actually open the file and load an event

#ifdef __CINT__

class IlcEveTPCData;
class IlcEveTPCSector2D;
class IlcEveTPCSector3D;

#else

#include <TEveManager.h>
#include <EveDet/IlcEveTPCData.h>
#include <EveDet/IlcEveTPCSector2D.h>
#include <EveDet/IlcEveTPCSector3D.h>
#include <EveDet/IlcEveTPCLoader.h>

#include <TSystem.h>
#include <TStyle.h>

#endif


IlcEveTPCLoader* tpc_loader = 0;

void tpc_gui(const char *file=0, Int_t ievent=0)
{
  gStyle->SetPalette(1, 0);

  IlcEveTPCLoader* l = tpc_loader = new IlcEveTPCLoader;
  IlcEveTPCData*   d = new IlcEveTPCData;
  // d->SetLoadPedestal(5);
  d->SetLoadThreshold(5);
  d->SetAutoPedestal(kTRUE);
  l->SetData(d);
  // l->SetDoubleSR(kTRUE);
  l->SetInitParams(40, 980, 10); // min-time, max-time, threshold
  // l->SetTPCEquipementMap("EquipmentIdMap.data");

  gEve->AddElement(l);

  if(file != 0) {
    l->SetFile(file);
    l->OpenFile();
    l->GotoEvent(ievent);
  }
}
