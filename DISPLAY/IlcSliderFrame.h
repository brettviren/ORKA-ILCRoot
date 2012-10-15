#ifndef ILCSLIDERFRAME_H
#define ILCSLIDERFRAME_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////////
// ILC SLIDER FRAME CLASS                                            //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>
#include <RQ_OBJECT.h>
#include <TGDoubleSlider.h>

class TGCompositeFrame;
class TGLayoutHints;
class TGLabel;
class TGNumberEntry;

class IlcSliderFrame{
  //This class implements the cuts manager

public:

 IlcSliderFrame(const TGWindow *p, UInt_t w, UInt_t h);
 virtual ~IlcSliderFrame();

 //Setters
 void		      	SetMomentumRange(Float_t min, Float_t max){fMomentumSlider->SetRange(min,max);};
 void		       	SetRapidityRange(Float_t min, Float_t max){fRapiditySlider->SetRange(min,max);};
 
 Float_t       	       	GetMomentumMax() const {return fMomentumSlider->GetMaxPosition();};
 Float_t       		GetMomentumMin() const {return fMomentumSlider->GetMinPosition();};
 Float_t	      	GetRapidityMax() const {return fRapiditySlider->GetMaxPosition();};
 Float_t       		GetRapidityMin() const {return fRapiditySlider->GetMinPosition();};
 TGCompositeFrame*	GetSliderFrame() const {return fMainFrame;};

 //Slots
 void		     	CloseWindow();
 void		       	DoSlider(Int_t pos=0);
 void		       	DoField(Long_t pos=0);
 void		       	DoReleased(Int_t pos=0) const;
 void                   DoPositionChanged(Int_t pos=0) const;

 //I/O
 void		       	SaveToRC() const;
 void		       	LoadFromRC();

private:
 
 TGCompositeFrame  	*fMainFrame; // Main frame
 TGCompositeFrame	*fMomentumFrame; // Momentum frame
 TGCompositeFrame	*fRapidityFrame; // Rapidity frame
 TGLayoutHints		*fLayout;//Layout of the frame
 TGLayoutHints		*fMomentumLayout; // Layout of momentum frame
 TGLayoutHints		*fRapidityLayout; // Layout of rapidity frame
 TGDoubleHSlider       	*fMomentumSlider; // Momentum slider
 TGDoubleHSlider       	*fRapiditySlider; // Rapidity slider
 TGLabel       		*fMomentumLabel;  // Momentum label
 TGLabel	       	*fRapidityLabel;  // Rapidity label
 TGNumberEntry		*fMomentumMaxValue;// Max. value of momentum
 TGNumberEntry		*fMomentumMinValue;// Min. value of momentum
 TGNumberEntry		*fRapidityMaxValue;// Max. value of rapidity
 TGNumberEntry		*fRapidityMinValue;// Min. value of rapidity

 RQ_OBJECT("IlcSliderFrame")

 ClassDef(IlcSliderFrame,0);
};

#endif
