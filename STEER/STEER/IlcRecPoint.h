#ifndef ILCRECPOINT_H
#define ILCRECPOINT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcRecPoint.h 53922 2012-01-16 09:19:28Z fca $ */

////////////////////////////////////////////////
//  Base class for Reconstructed Point        //
//  Version 0.1                               //
//  Author Yves Schutz     SUBATECH           //
//                                            //  
//                                            //
////////////////////////////////////////////////

// --- Standard library ---


// --- ROOT system ---

#include <TObject.h>
#include <TMatrixFfwd.h>
#include <TVector3.h>

// --- IlcRoot header files ---

class IlcDigitNew;
class IlcGeometry;

class IlcRecPoint : public TObject {

public:

  IlcRecPoint() ;                   // ctor            
  IlcRecPoint(const char * opt) ;                   // ctor            
  IlcRecPoint(const IlcRecPoint &recp);  // copy ctor
  virtual ~IlcRecPoint() ;          // dtor
 
  virtual void  AddDigit(IlcDigitNew & digit) ;  // add a digit to the digit's indexes list  
  //  virtual void  AddTrack(IlcTrack & track) ;  // add a track to the tracks list  
  virtual void  GetCovarianceMatrix(TMatrixF & mat) const;
  virtual IlcGeometry * GetGeom() const { return fGeom; } 
  virtual void  GetGlobalPosition(TVector3 & gpos, TMatrixF & gmat) const ; // return global position in ILC
  virtual int * GetDigitsList(void) const { return fDigitsList ; }
  //  virtual int * GetTracksList(void) const { return fTracksList ; }
  virtual Float_t GetEnergy() const {return fAmp; } 
  virtual void  GetLocalPosition(TVector3 & pos) const ;
  virtual Int_t GetDigitsMultiplicity(void) const { return fMulDigit ; }
  Int_t         GetIndexInList() const { return fIndexInList ; } 
  virtual Int_t GetMaximumDigitMultiplicity() const { return  fMaxDigit; } 
  virtual Int_t GetMaximumTrackMultiplicity() const { return  fMaxTrack; } 
  virtual Int_t GetTracksMultiplicity(void) const { return fMulTrack ; }
  IlcRecPoint & operator= (const IlcRecPoint &recp);

  void          SetIndexInList(Int_t val) { fIndexInList = val ; } 


protected:
  void  Copy(TObject &recp) const;

  Float_t       fAmp ;        // summed amplitude of digits 
  IlcGeometry * fGeom ;       //! pointer to the geometry class 
  Int_t         fIndexInList ;// the index of this RecPoint in the list stored in TreeR (to be set by analysis)
  TVector3      fLocPos ;     // local position in the sub-detector coordinate
  TMatrixF *     fLocPosM ;    // covariance matrix ;  
  Int_t         fMaxDigit ;   //! max initial size of digits array (not saved)
  Int_t         fMulDigit ;   // total multiplicity of digits
  Int_t         fMaxTrack ;   //! max initial size of tracks array (not saved)
  Int_t         fMulTrack ;   // total multiplicity of tracks
  Int_t *       fDigitsList ; //[fMulDigit] list of digit's indexes from which the point was reconstructed 
  Int_t *       fTracksList ; //[fMulTrack] list of tracks to which the point was assigned 

  ClassDef(IlcRecPoint,1) // Base class for reconstructed space points
 
};

#endif // ILCRECPOINT_H
