#ifndef ILCDETECTORFRAME_H
#define ILCDETECTORFRAME_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////////
// ILC DETECTOR FRAME CLASS                                          //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>
#include <RQ_OBJECT.h>

class TGWindow;
class TGCompositeFrame;
class TGCheckButton;
class TGTextButton;

class IlcDetectorFrame{
  //This classe implements the frame which contains the list of enabled detectors
  //and allows the user to change the current status of the detector (enabled/disabled)

public:

 IlcDetectorFrame(const TGWindow *p, Int_t w, Int_t h,UInt_t bgc);
 virtual ~IlcDetectorFrame();
 
 TGCompositeFrame*	GetDetectorFrame(){return fMainFrame;};
 
 //Slots
 void		      	DoButton(Int_t pos=0);
 void		      	DoCheckButton(Int_t pos=0);
 void		       	DoSpecific() const;

private:

 TGCompositeFrame	*fMainFrame; // Main frame
 TGCompositeFrame	*fButtonFrame; // Button frame
 TGCheckButton		**fCheckButton; // Array of checked buttons
 TGTextButton		*fButtonAll; // Button for "All"
 TGTextButton		*fButtonInvert; // Button for "Invert"
 Bool_t		       	*fCheckedButton; //fEnabledButton[i]=kTRUE if button checked
 Int_t		       	*fCheckButtonId; // Array of IDs for checked buttons
 Bool_t		       	fCheckedMode; // Flag for checked mode
 static int	       	fgBaseId; // Base ID???

 RQ_OBJECT("IlcDetectorFrame")

 ClassDef(IlcDetectorFrame,0);
};
#endif
