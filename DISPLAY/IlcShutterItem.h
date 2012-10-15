#ifndef ILCSHUTTERITEM_H
#define ILCSHUTTERITEM_H

/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/////////////////////////////////////////////////////////////////////////
// ILC SHUTTER ITEM CLASS                                            //
// Author: Mayeul   ROUSSELET                                          //
// e-mail: Mayeul.Rousselet@cern.ch                                    //
// Last update:26/08/2003                                              //
/////////////////////////////////////////////////////////////////////////

#include <Rtypes.h>
#include <RQ_OBJECT.h>

class TGShutter;
class TGShutterItem;
class TGCompositeFrame;
class TGButton;

class IlcShutterItem{
  //This class implements the shutter item, ie the base element of a shutter and provides functions to add button... in the shutter
public:
	
 IlcShutterItem(TGShutter *s, const char *text,UInt_t id);
 virtual ~IlcShutterItem();

 //Getters
 TGShutterItem*		GetShutterItem()const {return fShutterItem;};
 TGCompositeFrame*	GetShutterItemFrame()const {return fMainFrame;};

 //Fill functions
 void	      		AddTextButton(const char *text, const char *tiptext,  UInt_t idb);
 void	       		AddPictureButton(const char *file, const char *tiptext,UInt_t idb);
 void	       		AddCheckButton(const char *txt,Int_t idb);

 //Slot
 void	       		DoButton(Int_t pos=0) const;

private:

 TGCompositeFrame	*fMainFrame; // Main frame
 TGShutterItem		*fShutterItem; // Shutter item
 TGButton		*fButton; // Button

 RQ_OBJECT("IlcShutterItem")

 ClassDef(IlcShutterItem,0);
};

#endif
