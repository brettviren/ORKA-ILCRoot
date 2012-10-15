// $Id: IlcEveVSDCreator.h 37044 2009-11-19 22:14:18Z mtadel $
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

#ifndef IlcEveVSDCreator_H
#define IlcEveVSDCreator_H

#include <TEveVSD.h>

#include <map>

class IlcTPCParam;
class IlcRunLoader;

class IlcEveVSDCreator : public TEveVSD
{
public:
  IlcEveVSDCreator(const Text_t* name="IlcEveVSDCreator", const Text_t* title="");
  virtual ~IlcEveVSDCreator() {}

  void CreateVSD(const Text_t* vsdFile="IlcVSD.root");

  void CreateTrees();

  // --------------------------------------------------------------
  // Conversion functions.

  void ConvertKinematics();

  void ConvertHits();
  void ConvertAnyHits(const TString& detector, const TString& clones_class,
                      Int_t det_id, Float_t minDistSqr=0);
//   void ConvertTPCHits(Int_t det_id, Float_t minDistSqr=0);

  void ConvertClusters();
  void ConvertAnyClusters(const TString& detector, const TString& branch_name,
                          Int_t det_id);
//   void ConvertTPCClusters(Int_t det_id);

  void ConvertRecTracks();

  void ConvertV0();
  void ConvertKinks();

  void ConvertGenInfo();

  // --------------------------------------------------------------

  Int_t GetDebugLevel() const   { return fDebugLevel; }
  void  SetDebugLevel(Int_t dl) { fDebugLevel = dl; }

protected:
  void               MakeItsDigitsInfo();
  TEveMCRecCrossRef* GetGeninfo(Int_t label);

  Float_t       fTPCHitRes;  // Resolution for storing TPC hits.
  Float_t       fTRDHitRes;  // Resolution for storing TRD hits.

  Int_t         fDebugLevel; // Internal debug level.

  IlcRunLoader *fRunLoader;  // Internal run-loader.

  std::map<Int_t, TEveMCRecCrossRef*> fGenInfoMap; // Map label to MC-Rec cross-ref data structure.

private:
  IlcEveVSDCreator(const IlcEveVSDCreator&);            // Not implemented
  IlcEveVSDCreator& operator=(const IlcEveVSDCreator&); // Not implemented

  ClassDef(IlcEveVSDCreator, 0); // Create VSD file from ILC data.
}; // endclass IlcEveVSDCreator

#endif
