#ifndef IlcVVertex_H
#define IlcVVertex_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//-------------------------------------------------------------------------
//     base class for ESD and AOD vertices
//     Author: A. Dainese
//-------------------------------------------------------------------------

#include <TNamed.h>

class IlcVVertex: public TNamed {

public:
  IlcVVertex() { }
  virtual ~IlcVVertex() { }
  IlcVVertex(const IlcVVertex& vVert); 
  IlcVVertex& operator=(const IlcVVertex& vVert);

  // vertex properties
  virtual void     GetXYZ(Double_t position[3]) const = 0;
  virtual Double_t GetX() const = 0;
  virtual Double_t GetY() const = 0;
  virtual Double_t GetZ() const = 0;
  virtual void     GetCovarianceMatrix(Double_t covmatrix[6]) const = 0;
  

  virtual Double_t GetChi2perNDF() const = 0;
  virtual Double_t GetChi2() const = 0;
  virtual Int_t    GetNDF() const = 0;

  virtual Int_t    GetNContributors() const = 0;
  virtual void     PrintIndices() const = 0;
  virtual void     Print(Option_t* option = "") const = 0;

  virtual void Clear(Option_t* option) {TNamed::Clear(option);}


  ClassDef(IlcVVertex,1)  // base class for vertices
};

#endif
