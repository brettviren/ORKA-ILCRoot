#ifndef ILCTARGETSEGMENTATIONSSD_H
#define ILCTARGETSEGMENTATIONSSD_H

#include "IlcTARGETsegmentation2.h"
#include "IlcTARGETgeom.h"

// segmentation for SSD

class IlcTARGETsegmentationSSD : public IlcTARGETsegmentation2 {
 public:

    IlcTARGETsegmentationSSD();
    IlcTARGETsegmentationSSD(IlcTARGETgeom *gm);
    IlcTARGETsegmentationSSD(const IlcTARGETsegmentationSSD &source);
    virtual ~IlcTARGETsegmentationSSD(){}
    IlcTARGETsegmentationSSD& operator=(const IlcTARGETsegmentationSSD &source);


    // Strip size  
    virtual void    SetPadSize(Int_t lay,Float_t pitch,Float_t /* d */)
	{fPitch[lay]=pitch;}

    // Maximum number of strips along the two coordinates  
    virtual void    SetNPads(Int_t lay,Int_t p1,Int_t /* d */){fNstrips[lay]=p1;}
    // Returns the maximum number of cells (digits) posible
    virtual Int_t   GetNPads(Int_t lay) const {return 2*fNstrips[lay];}


    // Set stereo angles Pside-Nside 
    virtual void    SetAngles(Int_t lay,Float_t pa=0.0175, Float_t na=0.0175) 
                         {fStereoPl[lay]=pa; fStereoNl[lay]=na;
			 }


    // Set stereo angles Pside-Nside 
    // Transform from real coordinates to strips
    virtual void    GetPadIxz(Int_t lay,Float_t x ,Float_t z ,Int_t   &iP,Int_t  &iN) const;
    // Transform from strips to real coordinates
    virtual void    GetPadCxz(Int_t lay,Int_t iP, Int_t iN, Float_t &x , Float_t &z) const;
    virtual void    GetPadTxz(Int_t lay,Float_t &x , Float_t &z) const;
    // Transformation from Geant cm detector center local coordinates
    // to detector P and N side strip numbers..
    virtual Bool_t  LocalToDet(Int_t lay,Float_t x,Float_t z,Int_t &iP,Int_t &iN) const;
    // Transformation from detector segmentation/cell coordiantes starting
    // from 0. iPN=0 for P side and 1 for N side strip. Returned is z=0.0
    // and the corresponding x value..
    virtual void    DetToLocal(Int_t lay,Int_t ix,Int_t iPN,Float_t &x,Float_t &z) const;
    // Given one P side strip and one N side strip, Returns kTRUE if they
    // cross each other and the location of the two crossing strips and
    // their correxlation matrix c[2][2].
    virtual Bool_t  GetCrossing(Int_t lay,Int_t iP,Int_t iN,Float_t &x,Float_t &z,
				Float_t c[2][2]);

    virtual void Init();

    // Strip size in x
    virtual Float_t Dpx(Int_t lay,Int_t) const {return fPitch[lay];}
    // Strip size in z 
    virtual Float_t Dpz(Int_t lay,Int_t) const {return fDz[lay];}
    // Maximum number of Strips in x
    virtual Int_t    Npx(Int_t lay) const {return fNstrips[lay];}
    // Maximum number of Strips in z
    virtual Int_t    Npz(Int_t)const {return 1;}

    // Angles : Pside stereo angle-Nside stereo angle
     virtual void Angles(Int_t lay,Float_t &aP,Float_t &aN) const;
     virtual void SetLayer(Int_t l);
     virtual Int_t GetLayer() const {return fLayer;}
    // Print Default parameters
     virtual void PrintDefaultParameters() const;

  protected:

  virtual void Copy(TObject &obj) const; 

  Int_t*      fNstrips;       // Number of strips in x 
  Float_t*    fStereoP;       // Stereo angle for Pside (rad)
  Float_t*    fStereoN;       // Stereo angle for Nside (rad)
  Float_t*    fPitch;         // Pitch of the strips
  
  Float_t*    fStereoPl;       // Stereo angle for Pside
  Float_t*    fStereoNl;       // Stereo angle for Nside
  Int_t      fLayer;           //! layer number (5 or 6)
  static const Float_t fgkDxDefault;  // Default value for fDx
  static const Float_t fgkDzDefault;  // Default value for fDz
  static const Float_t fgkDyDefault;  // Default value for fDy
  static const Float_t fgkPitchDefault; //Default value for fPitch
  static const Int_t fgkNstripsDefault; //Default value for fNstrips

  ClassDef(IlcTARGETsegmentationSSD,2) //Segmentation class for SSD 
};

#endif
