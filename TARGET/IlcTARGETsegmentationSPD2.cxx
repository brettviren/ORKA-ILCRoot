/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

 
#include "IlcLog.h"
#include "IlcTARGETsegmentationSPD2.h"
#include "IlcTARGETgeom.h"
#include "IlcTARGETgeomSPD.h"
#include "IlcRun.h"
#include <TArrayF.h>
//#include "IlcTARGETgeom.h"
//////////////////////////////////////////////////////
// Segmentation class for                           //
// pixels                                           //
//                                                  //
//////////////////////////////////////////////////////
ClassImp(IlcTARGETsegmentationSPD2)

//_____________________________________________________________________________
IlcTARGETsegmentationSPD2::IlcTARGETsegmentationSPD2(): IlcTARGETsegmentation2(){
  // Default constructor
  fNpx=0;
  fNpz=0;
  fCellSizeX=0;
  fCellSizeZ=0;
  //for(Int_t k=0; k<; k++) fCellSizeX[k] = 0.;
  //for(Int_t k=0; k<6250; k++) fCellSizeZ[k] = 0.;
//Init();
}
IlcTARGETsegmentationSPD2::~IlcTARGETsegmentationSPD2() {
  if (fCellSizeZ){
    for(int i=0;i<fCellSizeZ->GetEntries();i++){
      TArrayF *ar=(TArrayF*)fCellSizeZ->UncheckedAt(i);
      delete ar;
    };
    fCellSizeZ->Clear();
    delete fCellSizeZ;
  }
  if (fCellSizeX){
    for(int i=0;i<fCellSizeX->GetEntries();i++){
      TArrayF *ar=(TArrayF*)fCellSizeX->UncheckedAt(i);
      delete ar;
    };
    fCellSizeX->Clear();
    delete fCellSizeX;
  }
  if (fNpx) delete[] fNpx;
  if (fNpz) delete[] fNpz;
}
//_____________________________________________________________________________
Float_t IlcTARGETsegmentationSPD2::ColFromZ300(Int_t lay,Float_t z) const {
// Get column number for each z-coordinate taking into account the 
// extra pixels in z direction assuming 300 micron sized pixels.


    
     Float_t col = 0.0;
     Float_t pitchz,pitchx,pitchy;
     fGeom2->GetPixelDimension(lay,pitchx,pitchy,pitchz);
     col = Float_t (z/pitchz);
     return col;
}
//_____________________________________________________________________________
Float_t IlcTARGETsegmentationSPD2::ZFromCol300(Int_t lay,Int_t col) const {
// same comments as above
// Get z-coordinate for each colunm number



  Float_t pitchz,pitchx,pitchy;
  fGeom2->GetPixelDimension(lay,pitchx,pitchy,pitchz);
  Float_t z = 0.0;
  z = (col+0.5)*pitchz;
  return z;
}
//_____________________________________________________________________________
Float_t IlcTARGETsegmentationSPD2::ZpitchFromCol300(Int_t lay,Int_t col) const {
  // returns Z pixel pitch for 300 micron pixels.
  

    Float_t pitchz,pitchx,pitchy;
    col = 0; // done to remove unused variable warning.
    fGeom2->GetPixelDimension(lay,pitchx,pitchy,pitchz);
    return pitchz;
}
//_____________________________________________________________________________
Float_t IlcTARGETsegmentationSPD2::ColFromZ(Int_t lay,Float_t z) const {
    // hard-wired - keep it like this till we can parametrise 
    // and get rid of IlcTARGETgeomSPD425
    // Get column number for each z-coordinate taking into account the 
    // extra pixels in z direction 
    
//cout<<"IlcTARGETsegmentationSPD2::ColFromZ"<<endl;
    
    Int_t i;
    Float_t s,col;

    if(z<0||z>fDz[lay]){
	Error("ColFromZ","z=%f outside of range 0.0<=z<fDz=%f",z,fDz[lay]);
	return 0.0; // error
    } // end if outsize of detector
    s = 0.0;
    i = -1;
    while(z>s){
	i++;
        s += ((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(i);
    } // end while
    s -= ((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(i);
    col = (Float_t) i + (z-s)/((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(i);
    return col;

/*
  Float_t col = 0;
  Float_t pitchz = 425;
  if( z < 13175) {
    col = Float_t(z/pitchz);
  } else if( z < 14425) {  
    pitchz = 625;
    col = 31 + (z - 13175)/pitchz;
  } else if( z < 27175) {  
    col = 33 + (z - 14425)/pitchz;
  } else if( z < 28425) {  
    pitchz = 625;
    col = 63 + (z - 27175)/pitchz;
  } else if( z < 41175) {  
    col = 65 + (z - 28425)/pitchz;
  } else if( z < 42425) {  
    pitchz = 625;
    col = 95 + (z - 41175)/pitchz;
  } else if( z < 55175) {  
    col = 97 + (z - 42425)/pitchz;
  } else if( z < 56425) {  
    pitchz = 625;
    col = 127 + (z - 55175)/pitchz;
  } else if( z < 69175) {  
    col = 129 + (z - 56425)/pitchz;
  } else if( z < 70425) {  
    pitchz = 625;
    col = 159 + (z - 69175)/pitchz;
  } else if( z < 83600) {  
    col = 161 + (z - 70425)/pitchz;
  }   
  return TMath::Abs(col);*/
}

//_____________________________________________________________________________
Float_t IlcTARGETsegmentationSPD2::ZFromCol(Int_t lay,Int_t col) const {
    // same comments as above
    // Get z-coordinate for each colunm number
    
//cout<<"IlcTARGETsegmentationSPD2::ZFromCol"<<endl;
    
    Int_t i;
    Float_t z;

    if(col<0||col>=fNpz[lay]){
	Error("ZFromCol","col=%d outside of range 0<=col<fNpZ=%d",col,fNpz[lay]);
	return 0.0; // error
    } // end if outsize of detector
    z = 0.0;
    for(i=0;i<col;i++) z += ((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(i);
    z += 0.5*((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(col);
    return z;

/* 
  Float_t pitchz = 425;
  Float_t z = 0;
  if( col >=0 && col <= 30 ) {  
    z = (col + 0.5)*pitchz;    
  } else if( col >= 31 && col <= 32) {  
    pitchz = 625;
    z = 13175 + (col -31 + 0.5)*pitchz;    
  } else if( col >= 33 && col <= 62) {  
    z = 14425 + (col -33 + 0.5)*pitchz;    
  } else if( col >= 63 && col <= 64) {  
    pitchz = 625;
    z = 27175 + (col -63 + 0.5)*pitchz;    
  } else if( col >= 65 && col <= 94) {  
    z = 28425 + (col -65 + 0.5)*pitchz;    
  } else if( col >= 95 && col <= 96) {  
    pitchz = 625;
    z = 41175 + (col -95 + 0.5)*pitchz;    
  } else if( col >= 97 && col <= 126) {  
    z = 42425 + (col -97 + 0.5)*pitchz;    
  } else if( col >= 127 && col <= 128) {  
    pitchz = 625;
    z = 55175 + (col -127 + 0.5)*pitchz;    
  } else if( col >= 129 && col <= 158) {  
    z = 56425 + (col -129 + 0.5)*pitchz;    
  } else if( col >= 159 && col <= 160) {  
    pitchz = 625;
    z = 69175 + (col -159 + 0.5)*pitchz;    
  } else if( col >= 161 && col <= 191) {  
    z = 70425 + (col -161 + 0.5)*pitchz;    
  }

  return z;*/
}
//______________________________________________________________________
Float_t IlcTARGETsegmentationSPD2::ZpitchFromCol(Int_t lay,Int_t col) const {
    // Get pitch size in z direction for each colunm
    
//cout<<"IlcTARGETsegmentationSPD2::ZpitchFromCol"<<endl;

    Float_t pitchz,pitchx,pitchy;
    fGeom2->GetPixelDimension(lay,pitchx,pitchy,pitchz);
    if(col < 0){
	pitchz = 0.0;
    }
    return pitchz;
}
//______________________________________________________________________
IlcTARGETsegmentationSPD2::IlcTARGETsegmentationSPD2(IlcTARGETgeom *gm){
  // Constructor
   fCorr=0;
   fNpx=0;
   fNpz=0;
   fGeom2 = gm;
   Init();
}
//______________________________________________________________________
void IlcTARGETsegmentationSPD2::Copy(TObject &obj) const {
  // protected method. copy this to obj
  IlcTARGETsegmentation2::Copy(obj);
  ((IlcTARGETsegmentationSPD2& ) obj).fNpx  = fNpx;
  ((IlcTARGETsegmentationSPD2& ) obj).fNpz  = fNpz;
  //Int_t i;
  //for(i=0;i<455;i++) 
         ((IlcTARGETsegmentationSPD2& ) obj).fCellSizeX = fCellSizeX;
  //for(i=0;i<6250;i++) 
         ((IlcTARGETsegmentationSPD2& ) obj).fCellSizeZ = fCellSizeZ;
}

//______________________________________________________________________
IlcTARGETsegmentationSPD2& IlcTARGETsegmentationSPD2::operator=(const IlcTARGETsegmentationSPD2 &source){
   // = operator
   if(this==&source) return *this;
   source.Copy(*this);
   return *this;
}
//____________________________________________________________________________
IlcTARGETsegmentationSPD2::IlcTARGETsegmentationSPD2(const IlcTARGETsegmentationSPD2 &source) :
    IlcTARGETsegmentation2(source){
  // copy constructor
  source.Copy(*this);
}
//----------------------------------------------------------------------
void IlcTARGETsegmentationSPD2::SetBinSize(Int_t lay,TArrayF x,TArrayF z){
    // Fills the array of pixel sizes in x, microns
    // The input array x must have 256 elements.
    Int_t i;
    //cout<<"IlcTARGETsegmentationSPD2::SetBinSize"<<endl;
    
    TArrayF *CellX=new TArrayF(fNpx[lay]);
    TArrayF *CellZ=new TArrayF(fNpz[lay]);
    if (fCellSizeZ){
      for(i=0;i<fCellSizeZ->GetEntries();i++){
        TArrayF *ar=(TArrayF*)fCellSizeZ->UncheckedAt(i);
	delete ar;
      }
    }
    if (fCellSizeX){
      for(i=0;i<fCellSizeX->GetEntries();i++){
        TArrayF *ar=(TArrayF*)fCellSizeX->UncheckedAt(i);
	delete ar;
      }
    }
//cout<<"Sono in IlcTARGETsegmentationSPD2 1"<<endl;

    fCellSizeX->Clear();
    fCellSizeZ->Clear();
    for(i=0;i<fNpx[lay];i++) CellX->AddAt(x[i],i);
    for(i=0;i<fNpz[lay];i++) CellZ->AddAt(z[i],i);
    fCellSizeX->AddAt((TObject *)CellX,lay);
    fCellSizeZ->AddAt((TObject *)CellZ,lay);
    //delete CellX;
    //delete CellZ;
    return;
}
//----------------------------------------------------------------------
void IlcTARGETsegmentationSPD2::Init300(){
// Initialize infromation for 6 read out chip 300X50 micron pixel SPD 
// detectors. This chip is 150 microns thick by 1.28 cm in x by 8.37 cm
// long. It has 256  50 micron pixels in x and 279 300 micron size
// pixels in z.
//cout<<"Sono in IlcTARGETsegmentationSPD2 2"<<endl;
    //const Float_t kconv=10000.;
//    Int_t i;
//    fNpx = 455; // The number of X pixel Cell same as in fCellSizeX array size
//    fNpz = 6250; // The number of Z pixel Cell same as in fCellSizeZ array /size
//    for(i=0;i<fNpx;i++) fCellSizeX[i] = 20.0; // microns all the same
//     for(i=0;i<280;i++) fCellSizeZ[i] = ZpitchFromCol300(i); // microns
//    for(i=0;i<6250;i++) fCellSizeZ[i] = 20.0; // microns
//    for(i=fNpz;i<280;i++) fCellSizeZ[i] = 0.0; // zero out rest of array
//    fDx = 0;
//    for(i=0;i<fNpx;i++) fDx += fCellSizeX[i];
//    fDz = 0;
//    for(i=0;i<fNpz;i++) fDz += fCellSizeZ[i];
//    fDy = 20.0; //microns  SPD sensitive layer thickness
}

//------------------------------
void IlcTARGETsegmentationSPD2::Init(){
// Initialize infromation for 6 read out chip 425X50 micron pixel SPD 
// detectors. This chip is 150 microns thick by 1.28 cm in x by 8.375 cm
// long. It has 256  50 micron pixels in x and 197 mostly 425 micron size
// pixels in z. The two pixels between each readout chip are 625 microns long.

//cout<<"Sono in IlcTARGETsegmentationSPD2 3"<<endl;
    const Float_t kconv=10000.;

    Int_t i,npixelx,npixelz,nlayers;
    Float_t pixelx,pixely,pixelz;

    TArrayF *CellX;
    TArrayF *CellZ;


    //if (Geometry()->GetDetVersion() !=3 ) cout <<"in IlcTARGETsegmentationSPD2: Minor Version: "<<Geometry()->GetDetVersion()<<endl;

    nlayers=fParam->GetNlayers()+fParam->GetNendcaps()+fParam->GetNlayersStrip()+fParam->GetNendcapsStrip();
    //nlayers=fParam->GetTotLayers();

    fCellSizeX=new TObjArray(nlayers);
    fCellSizeZ=new TObjArray(nlayers);
    
    fNpx=new Int_t[nlayers];
    fNpz=new Int_t[nlayers];
    
    fDx.Set(nlayers);
    fDz.Set(nlayers);
    
    for (Int_t lay=0;lay<nlayers;lay++) {
    //if (lay>=5) break;
    fDx[lay] = 0.;
    fDz[lay] = 0.;
    fGeom2->GetNPixelX(lay,npixelx);
    fGeom2->GetNPixelZ(lay,npixelz);
    CellX=new TArrayF(npixelx);
    CellZ=new TArrayF(npixelz);
    fNpx[lay] = npixelx; // The number of X pixel Cell same as in fCellSizeX array size
    fNpz[lay] = npixelz; // The number of Z pixel Cell same as in fCellSizeZ array size

    //printf("lay:%d fnpx:%d fnpz:%d\n",lay,fNpx[lay],fNpz[lay]);

    fGeom2->GetPixelDimension(lay,pixelx,pixely,pixelz);
    
    IlcDebug(5,Form("lay:%d fnpx:%d fnpz:%d\n",lay,fNpx[lay],fNpz[lay]));

    
    for(i=0;i<fNpx[lay];i++) CellX->AddAt(pixelx,i); // microns all the same
    for(i=0;i<fNpz[lay];i++) CellZ->AddAt(pixelz,i);
    fCellSizeX->AddAt((TObject *)CellX,lay);// microns
    fCellSizeZ->AddAt((TObject *)CellZ,lay);// microns

//    for(i=fNpz;i<280;i++) fCellSizeZ[i] = 0.0; // zero out rest of array
    for(i=0;i<fNpx[lay];i++) fDx[lay] += ((TArrayF *)fCellSizeX->UncheckedAt(lay))->At(i);
    for(i=0;i<fNpz[lay];i++) fDz[lay] += ((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(i);
    //fDx[lay] = fNpx[lay]*(((TArrayD *)fCellSizeX->UncheckedAt(lay))->At(0));
    //fDz[lay] = fNpz[lay]*(((TArrayD *)fCellSizeZ->UncheckedAt(lay))->At(0));
    IlcTARGETgeomSPD * geomspd=dynamic_cast<IlcTARGETgeomSPD*> (fGeom2->GetShape2(lay));
    if(!geomspd){
      fDy[lay] = 0;
    }else{
      fDy[lay] = geomspd->GetDy()*2.*kconv;//microns  SPD sensitive layer thickness
    }

//    printf(" IlcTARGETsegmentationSPD2 - Init: lay fNpx fNpz fDx fDz %d %d %d %f %f\n",lay,fNpx[lay], fNpz[lay], fDx[lay], fDz[lay]);
    //if (CellX) delete CellX;
    //if (CellZ) delete CellZ;
  }

}
//------------------------------
void IlcTARGETsegmentationSPD2::SetNPads(Int_t lay,Int_t p1, Int_t p2){
  // for SPD this function should be used ONLY when a beam test setup 
  // configuration is studied

    fNpx[lay]=p1;
    fNpz[lay]=p2;

}

//------------------------------
Float_t IlcTARGETsegmentationSPD2::Dpx(Int_t lay,Int_t i) const {
    //returs x pixel pitch for a give pixel
    //cout<<"IlcTARGETsegmentationSPD2::Dpx"<<endl;
    if(i<0||i>=fNpx[lay]) return 0.0;
    //cout<<"Layer in Dpx "<< lay<<endl;
    //cout<<"fNpx[lay] "<<fNpx[lay]<<endl;
    //cout<<"Index i in Dpx: "<<i<<endl;
    //restituisce la dimensione del pixel? in base all'indice "i". La dimensione del pixel e' salvata in fCellSizeX
    //cout<<"((TArrayF *)fCellSizeX->UncheckedAt(lay))->At(i) "<<((TArrayF *)fCellSizeX->UncheckedAt(lay))->At(i)<<endl; 
    return ((TArrayF *)fCellSizeX->UncheckedAt(lay))->At(i);
}
//------------------------------
Float_t IlcTARGETsegmentationSPD2::Dpz(Int_t lay,Int_t i) const {
    // returns z pixel pitch for a give pixel
    //cout<<"IlcTARGETsegmentationSPD2::Dpz"<<endl;
    if(i<0||i>=fNpz[lay]) return 0.0;    
    //cout<<"Layer in Dpz "<< lay<<endl;
    //cout<<"fNpz[lay] "<<fNpz[lay]<<endl;
    //cout<<"Index i in Dpz: "<<i<<endl;
    //cout<<"((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(i) "<<((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(i)<<endl; 
    return ((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(i);
}
//------------------------------
void IlcTARGETsegmentationSPD2::GetPadIxz(Int_t lay,Float_t x,Float_t z,Int_t &ix,Int_t &iz) const {
//  Returns pixel coordinates (ix,iz) for given real local coordinates (x,z)
//
//cout<<"Sono in IlcTARGETsegmentationSPD2 4"<<endl;
    // expects x, z in microns
//cout<<" Layer "<< lay<<endl;
//cout<<" X "<< x<<endl;
//cout<<" Z "<< z<<endl;
    // same segmentation on x
    Float_t dpx=Dpx(lay,0);
    ix = (Int_t)(x/dpx + 1);
    
    // different segmentation on z
    iz = (Int_t)(ColFromZ(lay,z) + 1);
//cout<<" iX "<< ix<<endl;
//cout<<" iZ "<< iz<<endl;


    if (iz >  fNpz[lay]) iz= fNpz[lay];
    if (ix >  fNpx[lay]) ix= fNpx[lay];
    /*
    if (iz < -fNpz) iz= -fNpz;
    if (ix < -fNpx) ix=-fNpx;
    */
}

//------------------------------
void IlcTARGETsegmentationSPD2::GetPadTxz(Int_t lay,Float_t &x,Float_t &z) const{
//  local transformation of real local coordinates (x,z)
//
//cout<<"IlcTARGETsegmentationSPD2::GetPadTxz"<<endl;
    // expects x, z in microns

    // same segmentation on x
    Float_t dpx=Dpx(lay,0);

    x /= dpx;
    z = ColFromZ(lay,z);
//printf("GetPadTXZ layer: %d X: %f, Z:%f",lay,x,z);
}
//------------------------------
void IlcTARGETsegmentationSPD2::GetPadCxz(Int_t lay,Int_t ix,Int_t iz,Float_t &x,Float_t&z) const {
    // Transform from pixel to real local coordinates

    // returns x, z in microns
//cout<<"IlcTARGETsegmentationSPD2::GetPadCxz"<<endl;
    Float_t dpx=Dpx(lay,0);

    x = (ix>0) ? Float_t(ix*dpx)-dpx/2. : Float_t(ix*dpx)+dpx/2.;
    z = ZFromCol(lay,iz);

//printf("GetPadCxz layer: %d X: %f, Z:%f",lay,x,z);
}
//------------------------------
void IlcTARGETsegmentationSPD2::
Neighbours(Int_t iX, Int_t iZ, Int_t* Nlist, Int_t Xlist[8], Int_t Zlist[8]) const {
  // returns the neighbouring pixels for use in Cluster Finders and the like.
  /*
    *Nlist=4;Xlist[0]=Xlist[1]=iX;Xlist[2]=iX-1;Xlist[3]=iX+1;
    Zlist[0]=iZ-1;Zlist[1]=iZ+1;Zlist[2]=Zlist[3]=iZ;
  */
//cout<<"Sono in IlcTARGETsegmentationSPD2 5"<<endl;

    *Nlist=8;
    Xlist[0]=Xlist[1]=iX;
    Xlist[2]=iX-1;
    Xlist[3]=iX+1;
    Zlist[0]=iZ-1;
    Zlist[1]=iZ+1;
    Zlist[2]=Zlist[3]=iZ;

   // Diagonal elements
    Xlist[4]=iX+1;
    Zlist[4]=iZ+1;

    Xlist[5]=iX-1;
    Zlist[5]=iZ-1;

    Xlist[6]=iX-1;
    Zlist[6]=iZ+1;

    Xlist[7]=iX+1;
    Zlist[7]=iZ-1;
}
//______________________________________________________________________
Bool_t IlcTARGETsegmentationSPD2::LocalToDet(Int_t lay,Float_t x,Float_t z,
                                         Int_t &ix,Int_t &iz) const {
    // Transformation from Geant detector centered local coordinates (cm) to
    // Pixel cell numbers ix and iz.
    // Input:
    //    Float_t   x        detector local coordinate x in cm with respect to
    //                       the center of the sensitive volume.
    //    Float_t   z        detector local coordinate z in cm with respect to
    //                       the center of the sensitive volulme.
    // Output:
    //    Int_t    ix        detector x cell coordinate. Has the range 
    //                       0<=ix<fNpx.
    //    Int_t    iz        detector z cell coordinate. Has the range 
    //                       0<=iz<fNpz.
    // Return:
    //   kTRUE if point x,z is inside sensitive volume, kFALSE otherwise.
    //   A value of -1 for ix or iz indecates that this point is outside of the
    //   detector segmentation as defined.

//cout<<"Sono in IlcTARGETsegmentationSPD2 6"<<endl;

    Int_t i,j;

    Double_t dx,dz;
    const Float_t kconv = 1.0E-04; // converts microns to cm.

    dx = -0.5*kconv*Dx(lay);
    dz = -0.5*kconv*Dz(lay);
    ix = -1;
    iz = -1;
//     IlcInfo(Form("lay: %d x: %f dx %f %f z: %f dz %f %f", lay, x, dx, Dx(lay), z, dz, Dz(lay) ));
    if(x<dx||z<dz) return kFALSE; // outside x||z range.

//     for(i=0;i<Npx(lay);i++){
  
//     //ix= (Int_t)((x-dx)/(((TArrayD *)fCellSizeX->At(lay))->At(0)))+1;

//       dx += kconv*((TArrayF *)fCellSizeX->UncheckedAt(lay))->At(i);

//       //  cout << "sto cercando pixel: i=" << i << " dx=" << dx << " x=" << x << endl;
//     //ix=(Int_t)(x/(kconv*((TArrayF *)fCellSizeX->UncheckedAt(lay))->At(0)));
// //    printf("4) dx=%f dz=%f\n",dx,dz); 
// 	if(x<dx) break;
// //    printf("5) dx=%f dz=%f\n",dx,dz); 
//     } // end for i
    i= (Int_t)((x-dx)/((((TArrayF *)fCellSizeX->UncheckedAt(lay))->At(0))*kconv));
// // // //     IlcInfo(Form("lay: %d i: %d %d %d x: %f dx %f xsize: %f", lay, i , Npx(lay), fNpx[lay], x, dx, (((TArrayF *)fCellSizeX->UncheckedAt(lay))->At(0))*kconv ));
    if(i>=Npx(lay)) return kFALSE; // outside x range.
//    printf("6) dx=%f dz=%f\n",dx,dz); 
//     for(j=0;j<Npz(lay);j++){

// //    printf("7) dx=%f dz=%f\n",dx,dz); 
//     //iz=(Int_t)(z/(kconv*((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(0)));
//     //iz = (Int_t)((z-dz)/(((TArrayD *)fCellSizeZ->At(lay))->At(0)))+1;

//       dz += kconv*((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(i);

//         //cout << "sto cercando pixel: i=" << i << " dz=" << dz << " z=" << z << endl;

// 	if(z<dz) break;
// //    printf("8) dx=%f dz=%f\n",dx,dz); 
//     } // end for j
    j= (Int_t)((z-dz)/((((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(0))*kconv));
// // // //     IlcInfo(Form("lay: %d j: %d %d %d z: %f dz %f %f", lay, j, Npz(lay), fNpz[lay], z, dz, (((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(0))*kconv ));

    if(j>=Npz(lay)) return kFALSE; // outside z range.
    ix = i;
    iz = j;

//if(lay==0){ printf("%d-Layer LocalToDet 0 coordinate Geant x=%f, z=%f, Output ix=%d, iz=%d\n",lay,x,z,ix,iz);}
 /*
 if (lay==0 && x==0 && z==0)
 {cout<<"Ho trovato il punto 0.0 nel layer 0 di coordinate Geant x= "<<x <<" z= "<<z<<" ix= "<<ix <<"iz= "<<iz<<endl;}*/
    return kTRUE; // Found ix and iz, return.
}
//______________________________________________________________________
void IlcTARGETsegmentationSPD2::DetToLocal(Int_t lay,Int_t ix,Int_t iz,Float_t &x,Float_t &z) const
{
// Transformation from Detector cell coordiantes to Geant detector centerd 
// local coordinates (cm).
// Input:
// Int_t    ix        detector x cell coordinate. Has the range 0<=ix<fNpx.
// Int_t    iz        detector z cell coordinate. Has the range 0<=iz<fNpz.
// Output:
// Float_t   x        detector local coordinate x in cm with respect to the
//                    center of the sensitive volume.
// Float_t   z        detector local coordinate z in cm with respect to the
//                    center of the sensitive volulme.
// If ix and or iz is outside of the segmentation range a value of -0.5*Dx()
// or -0.5*Dz() is returned.

    const Float_t kconv = 1.0E-04; // converts microns to cm.

//Aggiunge mezza cella al centro
//cout<<"Sono in IlcTARGETsegmentationSPD2 7 al layer "<<lay<<endl;
    
    x = -0.5*kconv*Dx(lay); // default value.
    z = -0.5*kconv*Dz(lay); // default value.
    
    if(ix<0 || ix>=Npx(lay)) return; // outside of detector
    if(iz<0 || iz>=Npz(lay)) return; // outside of detctor
    
    //cout << "Arriva qui 4"<<endl;
    
    Float_t *arrX=((TArrayF *)fCellSizeX->UncheckedAt(lay))->GetArray();
    
    //cout << "Arriva qui 5"<<endl;
    
    Float_t *arrZ=((TArrayF *)fCellSizeZ->UncheckedAt(lay))->GetArray();
    
    //cout << "Arriva qui 6"<<endl;
    //for(i=0;i<ix;i++) x += arrX[i]; // sum up to cell ix-1
    
    x +=ix*arrX[0]*kconv;
    x += 0.5*arrX[0]*kconv; // add 1/2 of cell ix for center location.
      

   
    //for(j=0;j<iz;j++) z += arrZ[j]; // sum up cell iz-1
    z +=iz*arrZ[0]*kconv;
    z += 0.5*arrZ[0]*kconv; // add 1/2 of cell iz for center location.
//     x*=kconv;
//     z*=kconv;
    return; // Found x and z, return.
   
}
//______________________________________________________________________
void IlcTARGETsegmentationSPD2::CellBoundries(Int_t lay,Int_t ix,Int_t iz,
					  Double_t &xl,Double_t &xu,
					  Double_t &zl,Double_t &zu) const
{
// Transformation from Detector cell coordiantes to Geant detector centerd 
// local coordinates (cm).
// Input:
// Int_t    ix        detector x cell coordinate. Has the range 0<=ix<fNpx.
// Int_t    iz        detector z cell coordinate. Has the range 0<=iz<fNpz.
// Output:
// Double_t   xl       detector local coordinate cell lower bounds x in cm
//                    with respect to the center of the sensitive volume.
// Double_t   xu       detector local coordinate cell upper bounds x in cm 
//                    with respect to the center of the sensitive volume.
// Double_t   zl       detector local coordinate lower bounds z in cm with
//                    respect to the center of the sensitive volulme.
// Double_t   zu       detector local coordinate upper bounds z in cm with 
//                    respect to the center of the sensitive volulme.
// If ix and or iz is outside of the segmentation range a value of -0.5*Dx()
// and -0.5*Dx() or -0.5*Dz() and -0.5*Dz() are returned.
    Int_t i,j;
    const Float_t kconv = 1.0E-04; // converts microns to cm.
    Float_t x,z;
    
//cout<<"Sono in IlcTARGETsegmentationSPD2 8"<<endl;

    xl = xu = x = -0.5*kconv*Dx(lay); // default value.
    zl = zu = z = -0.5*kconv*Dz(lay); // default value.
    if(ix<0 || ix>=Npx(lay)) return; // outside of detector
    if(iz<0 || iz>=Npz(lay)) return; // outside of detctor
    for(i=0;i<ix;i++) x += kconv*((TArrayF *)fCellSizeX->UncheckedAt(lay))->At(i); // sum up to cell ix-1
    xl = x;
    x += kconv*((TArrayF *)fCellSizeX->UncheckedAt(lay))->At(ix);
    xu = x;
    for(j=0;j<iz;j++) z += kconv*((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(j); // sum up cell iz-1
    zl = z;
    z += kconv*((TArrayF *)fCellSizeZ->UncheckedAt(lay))->At(iz);
    zu = z;
    return; // Found x and z, return.
}
