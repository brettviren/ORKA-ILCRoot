// $Id: IlcEveGedEditor.h 37031 2009-11-19 14:57:36Z mtadel $
// Author: Matevz Tadel 2009

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

#ifndef IlcEveGedEditor_H
#define IlcEveGedEditor_H

#include <TGedFrame.h>
#include <TEveGedEditor.h>


//==============================================================================
// IlcEveGedNameFrame
//==============================================================================

//______________________________________________________________________________
// Short description of IlcEveGedNameFrame
//

class IlcEveGedNameFrame  : public TGedFrame
{
public:
  IlcEveGedNameFrame(const TGWindow *p=0);
  virtual ~IlcEveGedNameFrame() {}

  virtual void SetModel(TObject* obj);

protected:
  TGTextButton *fB; // Info button.

private:
  IlcEveGedNameFrame(const IlcEveGedNameFrame&);            // Not implemented
  IlcEveGedNameFrame& operator=(const IlcEveGedNameFrame&); // Not implemented

  ClassDef(IlcEveGedNameFrame, 0); // Specialization of GED top name-frame for IlcEve.
};


//==============================================================================
// IlcEveGedEditor
//==============================================================================

//______________________________________________________________________________
// Short description of IlcEveGedEditor
//

class IlcEveGedEditor : public TEveGedEditor
{
public:
  IlcEveGedEditor();
  virtual ~IlcEveGedEditor() {}

protected:
  virtual TGedFrame* CreateNameFrame(const TGWindow* parent, const char* tab_name);

private:
  IlcEveGedEditor(const IlcEveGedEditor&);            // Not implemented
  IlcEveGedEditor& operator=(const IlcEveGedEditor&); // Not implemented

  ClassDef(IlcEveGedEditor, 0); // // Specialization of GED editor for IlcEve.
};

#endif
