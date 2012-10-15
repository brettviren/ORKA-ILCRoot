#ifndef ILCDCHCLUSTERCORRECTION_H
#define ILCDCHCLUSTERCORRECTION_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */


#include "TObject.h"

////////////////////////////////////////////////////////////////////////////
//                                                                        //
//  Used for correcting the cluster positions in some way or the other    //
//                                                                        //
////////////////////////////////////////////////////////////////////////////

class IlcDCHclusterCorrection : public TObject {

 public:  

  IlcDCHclusterCorrection();
  Float_t GetCorrection(Int_t plane, Int_t timebin, Float_t angle) const;
  Float_t GetSigma(Int_t plane, Int_t timebin, Float_t angle) const;
  Float_t GetOffsetAngle() const { return fOffsetAngle; }
  void    SetOffsetAngle(Float_t angle){fOffsetAngle=angle;}
  void    SetCorrection(Int_t plane,Int_t timebin, Float_t angle, Float_t value,Float_t sigma);
  Float_t GetAngle(Int_t i){return (i-10.)/10.+fOffsetAngle;}
  static  IlcDCHclusterCorrection * GetCorrection();

 protected:

  Float_t  fCorrections[6][30][20][2]; // Correction values
  Float_t  fOffsetAngle;               // Offset angle
				 
  ClassDef(IlcDCHclusterCorrection,1)  // ClusterCorrection for the DCH
 
};

#endif
