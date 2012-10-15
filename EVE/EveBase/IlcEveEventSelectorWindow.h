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

////////////////////////////////////////////////////////////////////////////
//
//  IlcEveEventSelectorWindow class
//  GUI for setting event and trigger selections
//
//  origin: Mikolaj Krzewicki, Nikhef, Mikolaj.Krzewicki@cern.ch
//
////////////////////////////////////////////////////////////////////////////


#ifndef IlcEveEventSelectorWindow_H
#define IlcEveEventSelectorWindow_H

#include "TGedFrame.h"
#include "TGComboBox.h"
#include <TObjString.h>

class IlcEveEventManager;
class IlcEveEventSelector;
class TGTextEntry;
class TGNumberEntry;
class TGCheckButton;
class TGComboBox;
class TRootEmbeddedCanvas;

//==============================================================================
// IlcEveEventSelectorWindow
//==============================================================================

//______________________________________________________________________________
// Short description of IlcEveEventSelectorWindow
//

class IlcEveEventSelectorWindow : public TGMainFrame
{
public:
  IlcEveEventSelectorWindow(const TGWindow *p, UInt_t w, UInt_t h, IlcEveEventSelector* sel);
  virtual ~IlcEveEventSelectorWindow();
  void SetEventSelector(IlcEveEventSelector* sel) {fPSelector = sel;}
  void DoSetSelectionString();
  void DoSetTriggerSelectionString();
  void DoHandleTriggerFromComboBox(const char* str);
  void DoSetMultiplicityRange();
  void DoDrawHistogram();
  void SetupTriggerSelect();

protected:

private:
  IlcEveEventSelector* fPSelector; //event selector
  TRootEmbeddedCanvas* fPCanvas;   //the canvas for histograms

  TGTextEntry*   fPDrawFormula;           //test draw input field
  TGTextEntry*   fPEntryFormula;          //selectin formula field
  TGNumberEntry* fPEntryLowerBound;       //lower boung for the formula
  TGNumberEntry* fPEntryHigherBound;      //higher bound for the formula
  TGTextButton*  fPButtonTextDone;        //done button for selection formula
  
  TGComboBox*    fPComboBoxTrigger;       //trigger selection box
  TGTextEntry*   fPEntryTriggerSelection; //trigger selection formula entry field
  TGCheckButton* fPCheckTriggerSimple;    //use simple trigger select
  TGCheckButton* fPCheckTriggerString;    //use trigger select formula
  TGNumberEntry* fPEntryMultHigh;         //lowest allowed multiplicity field
  TGNumberEntry* fPEntryMultLow;          //higest allowed multiplicity field

  IlcEveEventSelectorWindow(const IlcEveEventSelectorWindow&);
  IlcEveEventSelectorWindow& operator=(const IlcEveEventSelectorWindow&);
  
  ClassDef(IlcEveEventSelectorWindow, 1); // GUI window for IlcEveEventSelector
};

#endif
