#ifndef ILCDISPLAYFRAME_H
#define ILCDISPLAYFRAME_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////////
// ILC DISPLAY FRAME CLASS                                           //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>
#include <RQ_OBJECT.h>
#include <TRootEmbeddedCanvas.h>
#include <TGDimension.h>
#include <TPad.h>
#include <TGeometry.h>

#include "IlcDisplayClusters.h"

class TGCompositeFrame;
class TGTab;
class TObjArray;

class IlcDisplayHLT;

class IlcDisplayFrame{
  //This class implements the display of the event

public:

 IlcDisplayFrame(const TGWindow *p, UInt_t w, UInt_t h);
 virtual ~IlcDisplayFrame();
 
 //Getters
 TGCompositeFrame*		GetDisplayFrame() const {return fMainFrame;};
 TCanvas*	       		GetMainCanvas() const {return fMainCanvas;};
 Int_t		       		GetPreviousW() const {return fPreviousW;};
 Int_t		       		GetPreviousH() const {return fPreviousH;};
 TGDimension	       		GetFrameDimension() const
   {return ((TGCanvas*)fMainEmbeddedCanvas)->GetViewPort()->GetDefaultSize();};
 Int_t		       		GetNbActivePoints() const;
 Int_t                          GetNbClusters() const
   {return fClusters->GetNbClusters();};

 //Setters
 void		       		SetPreviousW(Int_t w){fPreviousW=w;};
 void	       			SetPreviousH(Int_t h){fPreviousH=h;};
 void	       			SetEditable(Bool_t b){gPad->SetEditable(b);};
 
 void	       			DoView(Int_t view);
 void	       			Draw(Float_t theta,Float_t phi,Float_t psi, Bool_t check=1);
 void	       			DrawDetector(const char *name);
 void	       			DrawHits();
 void	       			DrawX3d();
 void	       			DrawGL();
 void	       			LoadEnabledModules();
 void                           LoadClusters(Int_t nevent);
 void                           LoadHLTClusters(Int_t nevent);
 void		       		LoadHits();
 void		       		ApplyCuts();
 void		       		EnableDetector(const char *name);
 void		       		DisableDetector(const char *name);
 void		       		ExecuteEvent(Int_t event, Int_t px,Int_t py,TObject *);
 void                           SavePadGIF(const char *file);
 TGeometry*        		GetGeometry(Bool_t check=kTRUE);

private:

 TGCompositeFrame		*fMainFrame; // Main frame
 TGCompositeFrame		*fFrame1; // First frame
 TGCompositeFrame		*fFrame2; // Second frame
 TGTab			       	*fMainTab; // Main tab
 Bool_t			       	fAllViews; // Flag for all views
 TRootEmbeddedCanvas		*fMainEmbeddedCanvas;//embedded Canvas which contains the main view(s)
 TRootEmbeddedCanvas		*fSelectionEmbeddedCanvas; // Selected embedded canvas
 TCanvas	       		*fMainCanvas; // Main canvas
 TCanvas       			*fSelectionCanvas; // Selection canvas
 Float_t     			fClipMin;  // Min. clip
 Float_t     			fClipMax;  // Max. clip
 Int_t				fPreviousW;// Previous width
 Int_t				fPreviousH;// Previous height
 Float_t	      		fRange; // Range
 IlcDisplayClusters             *fClusters; // Clusters
 IlcDisplayHLT                  *fHLT; // HLT display
 TObjArray	      		*fPoints; // Array of points
 TObjArray		       	*fPoints2;// Array of points
 TObjArray		        *fModules;// Array of modules
 Int_t			       	fNbModules; // Number of modules
 Bool_t		       		*fActivePoints; // Flags for active points
 TObjArray	       		*fPolyMarkers;//Array for TPolyMarker3D
 Float_t	       		*fClustersPos;// Cluster position
 Int_t		       		fNbClusters;// Number of clusters
 TGeometry			*fGeometry;
 RQ_OBJECT("IlcDisplayFrame")

 ClassDef(IlcDisplayFrame,0);
};

#endif
