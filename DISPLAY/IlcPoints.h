#ifndef ILCPOINTS_H
#define ILCPOINTS_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcPoints.h,v 1.1.1.1 2008/03/11 14:53:08 vitomeg Exp $ */

// This class contains the points
// that are used for the ILC
// event display

#include "TPolyMarker3D.h"
// class IlcDetector;
class TParticle;

class IlcPoints : public TPolyMarker3D {
public:
  IlcPoints();
  IlcPoints(const IlcPoints& pts);
  IlcPoints(Int_t nhits);
  virtual ~IlcPoints();
  virtual Int_t         DistancetoPrimitive(Int_t px, Int_t py);
  virtual void          ExecuteEvent(Int_t event, Int_t px, Int_t py);
  IlcDetector          *GetDetector() const {return fDetector;}
  Int_t                 GetIndex() const {return fIndex;}
  TParticle            *GetParticle() const;
  virtual const Text_t *GetName() const;
  virtual void          InspectParticle() const; // *MENU*
  virtual void          DumpParticle() const; // *MENU*
  virtual Text_t       *GetObjectInfo(Int_t px, Int_t py) const;
  IlcPoints &           operator=(const IlcPoints &pts)
    {pts.Copy(*this); return (*this);}
  virtual void          Propagate(); // *MENU*
  virtual void          SetDetector(IlcDetector *det) {fDetector = det;}
  virtual void          SetParticle(Int_t index) {fIndex = index;}
  
protected:
  void Copy(TObject &pts) const;

  IlcDetector     *fDetector;    //Pointer to IlcDetector object
  Int_t            fIndex;       //Particle number in IlcRun::fParticles

  ClassDef(IlcPoints,1) //Class to draw detector hits (is PolyMarker3D)
};
#endif
