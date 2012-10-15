#ifndef ILCPVECRECPOINT_H
#define ILCPVECRECPOINT_H
/* 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * See cxx source for full Copyright notice                               */
//_________________________________________________________________________
//  Base Class for PVEC Reconstructed Points  
//  A recpoint being equivalent to a cluster in encal terminology                 
//*-- Author: Gines Martinez (SUBATECH)

// --- ROOT system ---
#include <TVector3.h>

// --- IlcRoot header files ---
#include "IlcCluster.h"

class TClonesArray ;
class IlcPVECDigit ;
class IlcDigitNew;
class TMAtrixF; 

class IlcPVECRecPoint : public IlcCluster {

 public:
  
  typedef TObjArray RecPointsList ; 

  IlcPVECRecPoint() ;                   // ctor         
  IlcPVECRecPoint(const char * opt) ;   // ctor 

  IlcPVECRecPoint(const IlcPVECRecPoint &rp);
  IlcPVECRecPoint& operator= (const IlcPVECRecPoint &rp);

  
  virtual ~IlcPVECRecPoint();

  virtual void Clear(const Option_t* /*option*/ ="") { delete[] fDigitsList; fDigitsList=0; delete[] fTracksList; fTracksList=0; }

//  virtual  void   AddDigit(IlcDigitNew &){
//   Fatal("AddDigit", "use AddDigit(IlcPVECDigit & digit, Float_t Energy)") ; 
//  }
  virtual  void   AddDigit(IlcPVECDigit & digit, Float_t Energy, Float_t time=0) = 0 ; 
  virtual Int_t   Compare(const TObject * obj) const = 0 ;   
  virtual Int_t   DistancetoPrimitive(Int_t px, Int_t py);
  virtual void    Draw(Option_t * option="") ;
  virtual void    ExecuteEvent(Int_t event, Int_t px, Int_t py) ;
  virtual void    EvalAll(TClonesArray * digits) ;  
  void            EvalLocal2TrackingCSTransform();
  virtual void    EvalPVECMod(IlcPVECDigit * digit) ;  
  virtual int *   GetDigitsList(void) const { return fDigitsList ; }  
  virtual Float_t GetEnergy() const {return fAmp; }
  virtual void    GetLocalPosition(TVector3 & pos) const ;   
  virtual void    GetGlobalPosition(TVector3 & gpos, TMatrixF & gmat) const ; // return global position in ILC
  virtual Int_t   GetPVECMod(void) const {return fPVECMod ; }
  virtual Int_t * GetPrimaries(Int_t & number) const {number = fMulTrack ; 
                                                      return fTracksList ; }
  virtual Int_t   GetDigitsMultiplicity(void) const { return fMulDigit ; }
  Int_t           GetIndexInList() const { return fIndexInList ; }
  virtual Bool_t  IsEmc(void)const { return kTRUE ;  } 
  virtual Bool_t  IsSortable() const { 
    // tells that this is a sortable object
    return kTRUE ; 
  }  
  void            SetIndexInList(Int_t val) { fIndexInList = val ; }
  virtual void    Paint(Option_t * option="");
  virtual void    Print(Option_t *) const {
    // Print prototype
  } 

protected:
  
  Int_t     fPVECMod ;    // PVEC Module number in which the RecPoint is found
  Int_t     fMulTrack ;   // total multiplicity of tracks to which the point was assigned
  Int_t     fMaxDigit ;   //! max initial size of digits array (not saved)
  Int_t     fMulDigit ;   // total multiplicity of digits
  Int_t     fMaxTrack ;   //! max initial size of tracks array (not saved)
  Int_t*    fDigitsList ; //[fMulDigit] list of digit's indexes from which the point was reconstructed 
  Int_t*    fTracksList ; //[fMulTrack] list of tracks to which the point was assigned 
  Float_t   fAmp ;        // summed amplitude of digits 
  Int_t     fIndexInList ;// the index of this RecPoint in the list stored in TreeR (to be set by analysis)  
  TVector3  fLocPos ;     // local position in the sub-detector coordinate


  ClassDef(IlcPVECRecPoint,3) // RecPoint for PVEC (Base Class)
 
};

#endif // IlcPVECRECPOINT_H
