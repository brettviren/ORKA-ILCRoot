#ifndef ILCINFOFRAME_H
#define ILCINFOFRAME_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////////
// ILC INFO FRAME CLASS                                              //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>
#include <RQ_OBJECT.h>

class TGCompositeFrame;
class TGLabel;

class IlcInfoFrame{
  //This class implements the info frame where the number of particles... are displayed

public:
	
 IlcInfoFrame(TGCompositeFrame *p, UInt_t w, UInt_t h);
 virtual ~IlcInfoFrame(void);
 
 void			AddLabel(const char *text, UInt_t options);
 TGCompositeFrame	*GetInfoFrame() const {return fMainFrame;};
 void 			Update();
 
private:

 TGCompositeFrame	*fMainFrame; // Main frame
 TGCompositeFrame	*fTitleFrame; // Title frame
 TGCompositeFrame	*fFiguresFrame; // Fugures frame
 TGLabel       		*fNbParticuleLabel; // Label for particle number
 TGLabel       	      	*fNbEventLabel; // Label for event number
 TGLabel       	       	*fNbHitsLabel; // Label for hits number
 TGLabel                *fNbClustersLabel; // Label for clusters number

 RQ_OBJECT("IlcInfoFrame")

 ClassDef(IlcInfoFrame,0);
};

#endif
