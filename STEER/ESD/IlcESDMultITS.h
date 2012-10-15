#ifndef IlcESDMultITS_H
#define IlcESDMultITS_H

#include <TObject.h>
#include <TMath.h>

class IlcESDMultITS : public TObject 
{
public:
  IlcESDMultITS():
    TObject(),
    fPhi(999),
    fEta(999){
    // Default constructor
  }
  IlcESDMultITS(Float_t phi, Float_t eta):
    TObject(),
    fPhi(phi),
    fEta(eta) {
    // Constructor
  }
    
  Float_t GetPhi() const {return fPhi;}
  Float_t GetEta() const {return fEta;}
  Float_t GetTheta() const {return 2*TMath::ATan(TMath::Exp(-fEta));}
  
private:

  Float_t  fPhi; // Phi angle
  Float_t  fEta; // Pseudo-rapidity

  ClassDef(IlcESDMultITS,1)
};

#endif
