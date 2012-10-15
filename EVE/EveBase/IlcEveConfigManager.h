// $Id: IlcEveConfigManager.h 43420 2010-09-08 09:02:32Z pdebski $
// Author: Matevz Tadel 2009

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

#ifndef IlcEveConfigManager_H
#define IlcEveConfigManager_H

#include "TObject.h"

class TGPopupMenu;

//______________________________________________________________________________
// Short description of IlcEveConfigManager
//

class IlcEveConfigManager : public TObject
{
public:
  static IlcEveConfigManager* InitializeMaster();
  static IlcEveConfigManager* GetMaster();

  virtual ~IlcEveConfigManager() {}

  void IlcEvePopupHandler(Int_t id);

protected:
  static IlcEveConfigManager* fgMaster;  // Main instance.

  TGPopupMenu      *fAnalysisPopup; // IlcEve menu with analysis tools.

  TGPopupMenu      *fMenuCameraPopup; // Camera menu.

  TGPopupMenu      *fIlcEvePopup; // IlcEve menu.

  TGPopupMenu      *fIlcEveGeometries; // IlcEve submenu - geometries.

  TGPopupMenu      *fIlcEvePictures; // IlcEve submenu - saving pictures.

  TGPopupMenu      *fIlcEvePicturesHR; // IlcEve submenu - saving pictures in high resolution.

  TGPopupMenu      *fIlcEveDataSelection; // IlcEve submenu - Saving/Opening DataSelection macros.

  TGPopupMenu      *fIlcEveVizDBs; // IlcEve submenu - Saving/Opening VizDB macros.

  Bool_t           fLoadCheck; //for Data Selection Save/Load

private:
  IlcEveConfigManager();

  IlcEveConfigManager(const IlcEveConfigManager&);            // Not implemented
  IlcEveConfigManager& operator=(const IlcEveConfigManager&); // Not implemented

  ClassDef(IlcEveConfigManager, 0); // Short description.
};

#endif
