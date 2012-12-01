#ifndef ILCCASCADEVERTEXER_H
#define ILCCASCADEVERTEXER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//------------------------------------------------------------------
//                    Cascade Vertexer Class
//          Reads V0s and tracks, writes out cascade vertices
//    Origin: Christian Kuhn, IReS, Strasbourg, christian.kuhn@ires.in2p3.fr
//------------------------------------------------------------------

#include "TObject.h"

class IlcESDEvent;
class IlcESDv0;
class IlcExternalTrackParam;

//_____________________________________________________________________________
class IlcCascadeVertexer : public TObject {
public:
  IlcCascadeVertexer();
  void SetCuts(const Double_t cuts[8]);
  static void SetDefaultCuts(const Double_t cuts[8]);

  Int_t V0sTracks2CascadeVertices(IlcESDEvent *event);
  Double_t Det(Double_t a00, Double_t a01, Double_t a10, Double_t a11) const;
  Double_t Det(Double_t a00,Double_t a01,Double_t a02,
	       Double_t a10,Double_t a11,Double_t a12,
	       Double_t a20,Double_t a21,Double_t a22) const;

  Double_t PropagateToDCA(IlcESDv0 *vtx,IlcExternalTrackParam *trk,Double_t b);

  void GetCuts(Double_t cuts[8]) const;
  static void GetDefaultCuts(Double_t cuts[8]);

private:
#ifndef __MAKECINT__
	#ifdef WIN32
      __declspec(dllexport) static	  Double_t fgChi2max;   // maximal allowed chi2 
	  __declspec(dllexport) static	  Double_t fgDV0min;    // min. allowed V0 impact parameter
	  __declspec(dllexport) static	  Double_t fgMassWin;   // window around the Lambda mass
	  __declspec(dllexport) static	  Double_t fgDBachMin;  // min. allowed bachelor impact parameter
	  __declspec(dllexport) static	  Double_t fgDCAmax;    // maximal allowed DCA between the V0 and the track 
	  __declspec(dllexport) static	  Double_t fgCPAmin;    // minimal allowed cosine of the cascade pointing angle
	  __declspec(dllexport) static	  Double_t fgRmin, fgRmax;// max & min radii of the fiducial volume
	#else
      static	  Double_t fgChi2max;   // maximal allowed chi2 
	  static	  Double_t fgDV0min;    // min. allowed V0 impact parameter
	  static	  Double_t fgMassWin;   // window around the Lambda mass
	  static	  Double_t fgDBachMin;  // min. allowed bachelor impact parameter
	  static	  Double_t fgDCAmax;    // maximal allowed DCA between the V0 and the track 
	  static	  Double_t fgCPAmin;    // minimal allowed cosine of the cascade pointing angle
	  static	  Double_t fgRmin, fgRmax;// max & min radii of the fiducial volume
	#endif
#else
  static
  Double_t fgChi2max;   // maximal allowed chi2 
  static
  Double_t fgDV0min;    // min. allowed V0 impact parameter
  static
  Double_t fgMassWin;   // window around the Lambda mass
  static
  Double_t fgDBachMin;  // min. allowed bachelor impact parameter
  static
  Double_t fgDCAmax;    // maximal allowed DCA between the V0 and the track 
  static
  Double_t fgCPAmin;    // minimal allowed cosine of the cascade pointing angle
  static
  Double_t fgRmin, fgRmax;// max & min radii of the fiducial volume
#endif  /*__MAKECINT__*/

  Double_t fChi2max;    // maximal allowed chi2 
  Double_t fDV0min;     // min. allowed V0 impact parameter
  Double_t fMassWin;    // window around the Lambda mass
  Double_t fDBachMin;   // min. allowed bachelor impact parameter
  Double_t fDCAmax;     // maximal allowed DCA between the V0 and the track 
  Double_t fCPAmin;     // minimal allowed cosine of the cascade pointing angle
  Double_t fRmin, fRmax;// max & min radii of the fiducial volume
  
  ClassDef(IlcCascadeVertexer,3)  // cascade verterxer 
};

#ifdef WIN32
#else
inline IlcCascadeVertexer::IlcCascadeVertexer() :
  TObject(),
  fChi2max(fgChi2max), 
  fDV0min(fgDV0min),
  fMassWin(fgMassWin),
  fDBachMin(fgDBachMin),
  fDCAmax(fgDCAmax),
  fCPAmin(fgCPAmin), 
  fRmin(fgRmin),
  fRmax(fgRmax)
{
}
#endif

inline void IlcCascadeVertexer::SetCuts(const Double_t cuts[8]) {
  fChi2max=cuts[0]; 
  fDV0min=cuts[1];   fMassWin=cuts[2]; fDBachMin=cuts[3];
  fDCAmax=cuts[4];   fCPAmin=cuts[5];
  fRmin=cuts[6];     fRmax=cuts[7]; 
}

inline void IlcCascadeVertexer::SetDefaultCuts(const Double_t cuts[8]) {
  fgChi2max=cuts[0]; 
  fgDV0min=cuts[1];   fgMassWin=cuts[2]; fgDBachMin=cuts[3];
  fgDCAmax=cuts[4];   fgCPAmin=cuts[5];
  fgRmin=cuts[6];     fgRmax=cuts[7]; 
}

inline void IlcCascadeVertexer::GetCuts(Double_t cuts[8]) const {
  cuts[0]=fChi2max; 
  cuts[1]=fDV0min;   cuts[2]=fMassWin;  cuts[3]=fDBachMin;
  cuts[4]=fDCAmax;   cuts[5]=fCPAmin;
  cuts[6]=fRmin;     cuts[7]=fRmax; 
}

inline void IlcCascadeVertexer::GetDefaultCuts(Double_t cuts[8]) {
  cuts[0]=fgChi2max; 
  cuts[1]=fgDV0min;   cuts[2]=fgMassWin;  cuts[3]=fgDBachMin;
  cuts[4]=fgDCAmax;   cuts[5]=fgCPAmin;
  cuts[6]=fgRmin;     cuts[7]=fgRmax; 
}

#endif

