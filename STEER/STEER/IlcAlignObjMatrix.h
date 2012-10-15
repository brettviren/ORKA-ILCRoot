#ifndef ILCILCGNOBJMATRIX_H
#define ILCILCGNOBJMATRIX_H

/**************************************************************************
 * IlcAlignObjMatrix: derived alignment class storing alignment           *
 *   information for a single volume in form of TGeoHMatrix, which        *
 *   includes the information for a translation, a rotation and a scale   *
 *************************************************************************/
#include "TObject.h"
#include "TString.h"
#include "TGeoMatrix.h"

#include "IlcAlignObj.h"

class IlcAlignObjMatrix : public IlcAlignObj {
 public:
  IlcAlignObjMatrix();
  IlcAlignObjMatrix(const char* symname, UShort_t volUId, Double_t x, Double_t y, Double_t z, Double_t psi, Double_t theta, Double_t phi, Bool_t global) throw (const Char_t *);
  IlcAlignObjMatrix(const char* symname, UShort_t volUId, TGeoMatrix& m, Bool_t global) throw (const Char_t *);
  IlcAlignObjMatrix(const IlcAlignObj& theAlignObj);
  IlcAlignObjMatrix& operator= (const IlcAlignObj& theAlignObj);
  virtual ~IlcAlignObjMatrix();
  
  //Setters
  virtual void SetTranslation(Double_t x, Double_t y, Double_t z);
  virtual void SetTranslation(const TGeoMatrix& m);
  virtual void SetRotation(Double_t psi, Double_t theta, Double_t phi);
  virtual Bool_t SetRotation(const TGeoMatrix& m);

  //Getters
  virtual void GetTranslation(Double_t* tr)  const;
  virtual Bool_t GetAngles(Double_t* angles)  const;
  virtual void GetMatrix(TGeoHMatrix& m) const;

  virtual IlcAlignObj& Inverse() const;
  
 protected:
  TGeoHMatrix fMatrix; // Transformation matrix
  
  ClassDef(IlcAlignObjMatrix, 1)
};

#endif
