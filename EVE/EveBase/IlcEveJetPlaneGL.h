// $Id: IlcEveJetPlaneGL.h 31253 2009-03-02 10:29:56Z mtadel $
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

#ifndef IlcEveJetPlaneGL_H
#define IlcEveJetPlaneGL_H

#include <TGLObject.h>

class TGLViewer;
class TGLScene;

class IlcEveJetPlane;

//==============================================================================
//
// IlcEveJetPlaneGL
//
// GL rendering code for IlcEveJetPlane class.

class IlcEveJetPlaneGL : public TGLObject
{
public:
  IlcEveJetPlaneGL();
  virtual ~IlcEveJetPlaneGL() {}

  virtual Bool_t SetModel(TObject* obj, const Option_t* opt=0);
  virtual void   SetBBox();

  // To support two-level selection
  // virtual Bool_t SupportsSecondarySelect() const { return kTRUE; }
  // virtual void ProcessSelection(TGLRnrCtx & rnrCtx, TGLSelectRecord & rec);

protected:
  IlcEveJetPlane* fM; // Model object.

  virtual void DirectDraw(TGLRnrCtx & rnrCtx) const;

private:
  IlcEveJetPlaneGL(const IlcEveJetPlaneGL&);            // Not implemented
  IlcEveJetPlaneGL& operator=(const IlcEveJetPlaneGL&); // Not implemented

  ClassDef(IlcEveJetPlaneGL, 0); // GL renderer for IlcEveJetPlane.
}; // endclass IlcEveJetPlaneGL

#endif
