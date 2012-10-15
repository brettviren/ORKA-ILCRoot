// $Id: IlcEveLego.h 43215 2010-08-30 12:48:52Z mtadel $
// Author: Stefano Carrazza 2010, CERN, stefano.carrazza@cern.ch

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

#ifndef ILCEVELEGO_H
#define ILCEVELEGO_H

#include "TEveElement.h"

class IlcESDEvent;
class IlcEveEventSelector;
class IlcEveMultiView;
class IlcESDtrack;
class IlcPhysicsSelection;

class TEveCalo3D;
class TEveCaloDataHist;
class TEveCaloLego;
class TEveCaloLegoOverlay;
class TEveScene;
class TEveViewer;
class TEveWindowSlot;
class TGLOverlayButton;
class TGLViewer;
class TH2F;

//______________________________________________________________________________
// 2D & 3D calorimeter like histograms from the ESD data.
//

class IlcEveLego : public TEveElementList
{
public:
  IlcEveLego(const char* name="IlcEveLego");
  virtual ~IlcEveLego();

  // Get Methods
  TEveCalo3D*       GetCalo3D()         {   return fCalo3d;           }
  TEveCalo3D*       GetCalo3DAllEvents(){   return fCalo3dAllEvents;  }
  TEveCaloDataHist* GetData()           {   return fData;             }
  TEveCaloDataHist* GetDataAllEvents()  {   return fDataAllEvents;    }
  IlcESDEvent*      GetESD()            {   return fEsd;              }
  TEveCaloLego*     GetLego()           {   return fLego;             }
  TEveCaloLego*     GetLegoAllEvents()  {   return fLegoAllEvents;    }
  IlcEveMultiView*  GetMultiView()      {   return fAl;               }
  Int_t             GetParticleType(IlcESDtrack *track);
  Float_t           GetPtMax();
  Float_t           GetPtMaxAE();

  // Set Methods
  void SetCollisionCandidatesOnly();
  void SetMaxPt(Double_t val);
  void SetMaxPtAE(Double_t val);
  void SetParticleType(Int_t id, Bool_t status);
  void SetParticleTypeAE(Int_t id, Bool_t status);
  void SetThreshold(Double_t val);
  void SetThresholdAE(Double_t val);
  void SetTracks(Int_t id)   {  fTracksId = id;    Update();         }
  void SetTracksAE(Int_t id) {  fTracksIdAE = id;  FilterAllData();  }

  // Functions
  void              ApplyParticleTypeSelectionAE();
  TEveCaloLego*     CreateHistoLego();
  TEveCaloLego*     CreateHistoLego(TEveWindowSlot* slot);
  TEveCalo3D*       Create3DView();
  TEveCalo3D*       Create3DView(TEveWindowSlot* slot);
  void              CreateProjections(TEveWindowSlot* slot1,
                                      TEveWindowSlot* slot2);
  TEveCaloDataHist* FilterData();
  TEveCaloDataHist* FilterAllData();
  TEveCaloDataHist* LoadData();
  TEveCaloDataHist* LoadAllData();
  TEveCaloDataHist* LoadAllEvents();
  void              SwitchDataType(Bool_t status);
  void              Update();


private:
  Bool_t              fIsMC;                    // Switch to MC mode for IlcPhysicsSelection
  Bool_t              fCollisionCandidatesOnly; // Activate flag when loading all events
  Bool_t              *fParticleTypeId;         // Determine how particles to show
  Bool_t              *fParticleTypeIdAE;       // Determine how particles to show
  Int_t               fTracksId;                // Determine tracks selection
  Float_t             fMaxPt;                   // Set maximum pT
  Int_t               fTracksIdAE;              // Determine tracks selection for all events
  Float_t             fMaxPtAE;                 // Determine maximum pT for all events
  IlcESDEvent         *fEsd;                    // ESD tree
  IlcPhysicsSelection *fPhysicsSelection;       // Physics selection object
  TH2F                *fHistopos;               // Positive charge histogram
  TH2F                *fHistoposAllEvents;      // Positive charge histogram for all events
  TH2F                *fHistoneg;               // Negative charge histogram
  TH2F                *fHistonegAllEvents;      // Negative charge histogram for all events
  TH2F                *fHistoElectrons;         // Electrons histogram
  TH2F                *fHistoElectronsAllEvents;// Electrons histogram all events
  TH2F                *fHistoMuons;             // Muons histogram
  TH2F                *fHistoMuonsAllEvents;    // Muons histogram all events
  TH2F                *fHistoPions;             // Pions histogram
  TH2F                *fHistoPionsAllEvents;    // Pions histogram all events
  TH2F                *fHistoKaons;             // Kaons histogram
  TH2F                *fHistoKaonsAllEvents;    // Kaons histogram all events
  TH2F                *fHistoProtons;           // Protons histogram
  TH2F                *fHistoProtonsAllEvents;  // Protons histogram all events
  TEveCaloDataHist    *fData;                   // Calo data for 2D, 3D histograms
  TEveCaloDataHist    *fDataAllEvents;          // Calo data for all events
  TEveCaloLego        *fLego;                   // Calo lego for histograms
  TEveCaloLego        *fLegoAllEvents;          // Calo lego for all events histograms
  TEveCalo3D          *fCalo3d;                 // 3D histogram for single event
  TEveCalo3D          *fCalo3dAllEvents;        // 3D histogram for all events
  TGLViewer           *fGlv;                    // Viewer object
  TEveViewer          *fHisto2dv;               // Viewer for histograms
  TEveScene           *fHisto2ds;               // Scene for 3d histogram
  TEveScene           *fHisto2ds2;              // Scene for 3d histogram new tab
  TEveViewer          *fHisto2dAllEventsv0;     // Viewer 1 for all events tab
  TEveViewer          *fHisto2dAllEventsv1;     // Viewer 2 for all events tab
  TEveViewer          *fHisto2dAllEventsv2;     // Viewer 3 for all events tab
  TEveViewer          *fHisto2dAllEventsv3;     // Viewer 4 for all events tab
  TEveScene           *fHisto2dAllEventss0;     // Scene for all events tab
  TEveScene           *fHisto2dAllEventss1;     // Scene for all events tab
  TEveScene           *fHisto2dAllEventss2;     // Scene for all events tab
  TEveScene           *fHisto2dAllEventss3;     // Scene for all events tab
  IlcEveMultiView     *fAl;                     // IlcEveMultiView object to create 2D projections
  TEveCaloLegoOverlay *fHisto2dLegoOverlay;     // Overlay for calo lego
  TEveCaloLegoOverlay *fHisto2dAllEventsLegoOverlay; // Overlay for calo lego all events
  TEveWindowSlot      *fHisto2dAllEventsSlot;   // Window slot for 2d all events histogram

  IlcEveLego(const IlcEveLego&);                // Not implemented
  IlcEveLego& operator=(const IlcEveLego&);     // Not implemented


  ClassDef(IlcEveLego, 0); // Short description.
};

#endif
