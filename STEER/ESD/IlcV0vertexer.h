#ifndef ILCV0VERTEXER_H
#define ILCV0VERTEXER_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

//------------------------------------------------------------------
//                    V0 Vertexer Class
//            reads tracks writes out V0 vertices
//   Origin: Iouri Belikov, IReS, Strasbourg, Jouri.Belikov@cern.ch 
//------------------------------------------------------------------

#include "TObject.h"

class TTree;
class IlcESDEvent;

//_____________________________________________________________________________
class IlcV0vertexer : public TObject {
public:
  IlcV0vertexer();
  void SetCuts(const Double_t cuts[7]);
  static void SetDefaultCuts(const Double_t cuts[7]);

  Int_t Tracks2V0vertices(IlcESDEvent *event);

  void GetCuts(Double_t cuts[7]) const;
  static void GetDefaultCuts(Double_t cuts[7]);

private:
#ifndef __MAKECINT__
	#ifdef WIN32
	  __declspec(dllexport) static	  Double_t fgChi2max;      // maximal allowed chi2 
	  __declspec(dllexport) static	  Double_t fgDNmin;        // min allowed impact parameter for the 1st daughter
	  __declspec(dllexport) static	  Double_t fgDPmin;        // min allowed impact parameter for the 2nd daughter
	  __declspec(dllexport) static	  Double_t fgDCAmax;       // maximal allowed DCA between the daughter tracks 
	  __declspec(dllexport) static	  Double_t fgCPAmin;       // minimal allowed cosine of V0's pointing angle
	  __declspec(dllexport) static	  Double_t fgRmin, fgRmax; // max & min radii of the fiducial volume
	#else
	  static	  Double_t fgChi2max;      // maximal allowed chi2 
	  static	  Double_t fgDNmin;        // min allowed impact parameter for the 1st daughter
	  static	  Double_t fgDPmin;        // min allowed impact parameter for the 2nd daughter
	  static	  Double_t fgDCAmax;       // maximal allowed DCA between the daughter tracks 
	  static	  Double_t fgCPAmin;       // minimal allowed cosine of V0's pointing angle
	  static	  Double_t fgRmin, fgRmax; // max & min radii of the fiducial volume
	#endif
#else
	static
	  Double_t fgChi2max;      // maximal allowed chi2 
	  static
	  Double_t fgDNmin;        // min allowed impact parameter for the 1st daughter
	  static
	  Double_t fgDPmin;        // min allowed impact parameter for the 2nd daughter
	  static
	  Double_t fgDCAmax;       // maximal allowed DCA between the daughter tracks 
	  static
	  Double_t fgCPAmin;       // minimal allowed cosine of V0's pointing angle
	  static
	  Double_t fgRmin, fgRmax; // max & min radii of the fiducial volume
 #endif  /*__MAKECINT__*/
	  
	  Double_t fChi2max;      // maximal allowed chi2 
	  Double_t fDNmin;        // min allowed impact parameter for the 1st daughter
	  Double_t fDPmin;        // min allowed impact parameter for the 2nd daughter
	  Double_t fDCAmax;       // maximal allowed DCA between the daughter tracks 
	  Double_t fCPAmin;       // minimal allowed cosine of V0's pointing angle
	  Double_t fRmin, fRmax;  // max & min radii of the fiducial volume

  ClassDef(IlcV0vertexer,3)  // V0 verterxer 
};

#ifdef WIN32
//IlcV0vertexer();
void SetCuts(const Double_t cuts[7]);
#else
inline IlcV0vertexer::IlcV0vertexer() :
  TObject(),
  fChi2max(fgChi2max), 
  fDNmin(fgDNmin),
  fDPmin(fgDPmin),
  fDCAmax(fgDCAmax),
  fCPAmin(fgCPAmin), 
  fRmin(fgRmin),
  fRmax(fgRmax) 
{
}

inline void IlcV0vertexer::SetCuts(const Double_t cuts[7]) {
  fChi2max=cuts[0]; 
  fDNmin=cuts[1];   fDPmin=cuts[2];
  fDCAmax=cuts[3];  fCPAmin=cuts[4];
  fRmin=cuts[5];    fRmax=cuts[6]; 
}
#endif

inline void IlcV0vertexer::SetDefaultCuts(const Double_t cuts[7]) {
  fgChi2max=cuts[0]; 
  fgDNmin=cuts[1];   fgDPmin=cuts[2];
  fgDCAmax=cuts[3];  fgCPAmin=cuts[4];
  fgRmin=cuts[5];    fgRmax=cuts[6]; 
}

inline void IlcV0vertexer::GetCuts(Double_t cuts[7]) const {
  cuts[0]=fChi2max; 
  cuts[1]=fDNmin;   cuts[2]=fDPmin; 
  cuts[3]=fDCAmax;  cuts[4]=fCPAmin;
  cuts[5]=fRmin;    cuts[6]=fRmax; 
}

inline void IlcV0vertexer::GetDefaultCuts(Double_t cuts[7]) {
  cuts[0]=fgChi2max; 
  cuts[1]=fgDNmin;   cuts[2]=fgDPmin; 
  cuts[3]=fgDCAmax;  cuts[4]=fgCPAmin;
  cuts[5]=fgRmin;    cuts[6]=fgRmax; 
}

#endif


