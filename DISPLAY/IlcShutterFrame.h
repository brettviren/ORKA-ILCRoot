#ifndef ILCSHUTTERFRAME_H
#define ILCSHUTTERFRAME_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////////
// ILC SHUTTER FRAME CLASS                                           //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>
#include <RQ_OBJECT.h>

class TGCompositeFrame;
class TGLayoutHints;
class TGShutter;
class IlcDetectorFrame;

class IlcShutterFrame{
  //This class implements the shutter frame
public:

 IlcShutterFrame(TGCompositeFrame *p, UInt_t w, UInt_t h);
 virtual ~IlcShutterFrame();

 TGCompositeFrame*	GetShutterFrame(){return fMainFrame;};

private:

 TGCompositeFrame	*fMainFrame; // Main frame
 TGLayoutHints		*fLayout; // Layout
 TGShutter	       	*fShutter; // Shutter
 IlcDetectorFrame	*fDetectorFrame; // Detector frame
 TGLayoutHints		*fDetectorFrameLayout; // Detector frame layout

 RQ_OBJECT("IlcShutterFrame")

 ClassDef(IlcShutterFrame,0);
};

#endif
