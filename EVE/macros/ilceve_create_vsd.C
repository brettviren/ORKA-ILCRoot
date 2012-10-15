// $Id: ilceve_create_vsd.C 37044 2009-11-19 22:14:18Z mtadel $
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

void ilceve_create_vsd(Int_t nEvents=1, Int_t minTracks=0)
{
  // Invoke from a running ilceve.
  // nEvents will be domped starting from current one.
  // If minTracks is set at least that many ESD tracks must exist.

  TEveVSD::DisableTObjectStreamersForVSDStruct();

  IlcEveVSDCreator vc;
  vc.SetDebugLevel(2);

  Int_t nDone = 0;
  while (nDone < nEvents)
  {
    if (minTracks)
    {
      IlcESDEvent* esd = IlcEveEventManager::AssertESD();
      while (esd->GetNumberOfTracks() < minTracks)
      {
	IlcEveEventManager::GetMaster()->NextEvent();
	esd = IlcEveEventManager::AssertESD();
      }
    }
    vc.CreateVSD("IlcVSD.root");
    ++nDone;

    IlcEveEventManager::GetMaster()->NextEvent();
  }
}
