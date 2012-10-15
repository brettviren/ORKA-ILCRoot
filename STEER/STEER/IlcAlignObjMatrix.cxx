/**************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/

//-----------------------------------------------------------------
//   Implementation of the alignment object class through
//   the derived concrete representation of alignment object class:
//   IlcAlignObjMatrix derived from the base class IlcAlignObj
//-----------------------------------------------------------------

#include "IlcAlignObj.h"
#include "IlcAlignObjMatrix.h"

ClassImp(IlcAlignObjMatrix)

//_____________________________________________________________________________
IlcAlignObjMatrix::IlcAlignObjMatrix() : 
  IlcAlignObj(),
  fMatrix()
{
  // Default constructor
  //
}

//_____________________________________________________________________________
IlcAlignObjMatrix::IlcAlignObjMatrix(const char* symname, UShort_t volUId, Double_t x, Double_t y, Double_t z, Double_t psi, Double_t theta, Double_t phi, Bool_t global) throw (const Char_t *) :
  IlcAlignObj(symname,volUId),
  fMatrix()
{
  // standard constructor with 3 translation + 3 rotation parameters
  // If the user explicitly sets the global variable to kFALSE then the
  // parameters are interpreted as giving the local transformation.
  // This requires to have a gGeoMenager active instance, otherwise the
  // constructor will fail (no object created)
  // 
  if(global){
    SetPars(x, y, z, psi, theta, phi);
  }else{
    if(!SetLocalPars(x,y,z,psi,theta,phi)) throw "Alignment object creation failed (TGeo instance needed)!\n";
  }
}


//_____________________________________________________________________________
IlcAlignObjMatrix::IlcAlignObjMatrix(const char* symname, UShort_t volUId, TGeoMatrix& m, Bool_t global) throw (const Char_t *) :
  IlcAlignObj(symname,volUId),
  fMatrix()
{
  // standard constructor with TGeoMatrix
  // If the user explicitly sets the global variable to kFALSE then the
  // parameters are interpreted as giving the local transformation.
  // This requires to have a gGeoMenager active instance, otherwise the
  // constructor will fail (no object created)
  //

  if (global) {
    SetMatrix(m);
  }
  else {
    if (!SetLocalMatrix(m)) throw "Alignment object creation failed (TGeo instance needed)!\n";
  }
}

//_____________________________________________________________________________
IlcAlignObjMatrix::IlcAlignObjMatrix(const IlcAlignObj& theAlignObj) :
  IlcAlignObj(theAlignObj),
  fMatrix()
{
  //copy constructor
  //
  Double_t tr[3];
  theAlignObj.GetTranslation(tr);
  SetTranslation(tr[0],tr[1],tr[2]);
  Double_t rot[3];
  if (theAlignObj.GetAngles(rot))
    SetRotation(rot[0],rot[1],rot[2]);
}

//_____________________________________________________________________________
IlcAlignObjMatrix &IlcAlignObjMatrix::operator =(const IlcAlignObj& theAlignObj)
{  
  // assignment operator
  //
  if(this==&theAlignObj) return *this;
  ((IlcAlignObj *)this)->operator=(theAlignObj);
  Double_t tr[3];
  theAlignObj.GetTranslation(tr);
  SetTranslation(tr[0],tr[1],tr[2]);
  Double_t rot[3];
  if (theAlignObj.GetAngles(rot))
    SetRotation(rot[0],rot[1],rot[2]);

  return *this;
}

//_____________________________________________________________________________
IlcAlignObjMatrix::~IlcAlignObjMatrix()
{
  // Destructor
  //
}

//_____________________________________________________________________________
void IlcAlignObjMatrix::SetTranslation(Double_t x, Double_t y, Double_t z)
{
  // set the translation coefficients of the data member matrix
  // from the parameters passed as arguments
  // 
  Double_t tr[3];
  tr[0]=x; tr[1]=y; tr[2]=z;
  fMatrix.SetTranslation(tr);
}

//_____________________________________________________________________________
void IlcAlignObjMatrix::SetTranslation(const TGeoMatrix& m)
{
  // set the translation coefficients of the data member matrix
  // from the matrix passed as argument
  // 
  const Double_t *tr = m.GetTranslation();
  fMatrix.SetTranslation(tr);
}

//_____________________________________________________________________________
void IlcAlignObjMatrix::SetRotation(Double_t psi, Double_t theta, Double_t phi)
{
  // set the rotation parameters from the parameters passed as arguments
  // 
  Double_t angles[3] = {psi, theta, phi};
  Double_t rot[9];
  AnglesToMatrix(angles,rot);
  fMatrix.SetRotation(rot);
}

//_____________________________________________________________________________
Bool_t IlcAlignObjMatrix::SetRotation(const TGeoMatrix& m)
{
  // set the rotation coefficients of the data member matrix
  // from the matrix passed as argument
  // 
  const Double_t* rot = m.GetRotationMatrix();
  fMatrix.SetRotation(rot);
  return kTRUE;
}

//_____________________________________________________________________________
void IlcAlignObjMatrix::GetTranslation(Double_t *tr) const
{
  // Get Translation from TGeoMatrix
  const Double_t* translation = fMatrix.GetTranslation();
  tr[0] = translation[0];
  tr[1] = translation[1];
  tr[2] = translation[2];
}

//_____________________________________________________________________________
Bool_t IlcAlignObjMatrix::GetAngles(Double_t *angles) const
{
  // Get rotation angles from the TGeoHMatrix
  const Double_t* rot = fMatrix.GetRotationMatrix();
  return MatrixToAngles(rot,angles);
}

//_____________________________________________________________________________
void IlcAlignObjMatrix::GetMatrix(TGeoHMatrix& m) const
{
  // Get TGeoHMatrix
  //
  const Double_t *tr = fMatrix.GetTranslation();
  m.SetTranslation(tr);
  const Double_t *rot = fMatrix.GetRotationMatrix();
  m.SetRotation(rot);
}

//_____________________________________________________________________________
IlcAlignObj& IlcAlignObjMatrix::Inverse() const
{
  // Return a temporary "inverse" of the alignment object, that is return
  // an object with inverted transformation matrix.
  //
   static IlcAlignObjMatrix a;
   a = *this;

   TGeoHMatrix m;
   GetMatrix(m);
   a.SetMatrix(m.Inverse());

   return a;
}
