#ifndef ILCLHCDATA_H
#define ILCLHCDATA_H

/********************************************************************************
*                                                                               *
*   IlcLHCData: summary of the LHC related information from LHC DIP.            *
*   Created from the TMap provided by the IlcLHCReader with optional beginning  *
*                                                                               *
*   The data are (wrapped in the IlcLHCDipValT):                                *
*   made of TimeStamp (double) and array of values                              *
*                                                                               *
*   Multiple entries for each type of data are possible. To obtaine number of   *
*   records (with distinct timestamp) for give type od records use:             *
*   int GetNBunchConfigMeasured(int beam) (with beam=0,1) etc.                  *
*                                                                               *
*   To get i-th entry, use brec= IlcLHCDipValI* GetBunchConfigMeasured(bm,i);   *
*   Note: exact type of templated IlcLHCDipValT pointer depends on the record   *
*   type, concult getters to know it.                                           *
*                                                                               *
*   Then, once the pointer is obtained, details can be accessed:                *
*   int nBunches = brec->GetSize();                                             *
*   for (int i=0;i<nBunches;i++) printf("Bunch#%d: %d\n",i,(*brec)[i]);         *
*                                                                               *
*   ATTENTION: Bunch RFBucked is NEGATIVE for bunches interacting at IR2        *
*                                                                               *
*                                                                               *
*                                                                               *
*   Author: ruben.shahoyan@cern.ch                                              *
*                                                                               *
********************************************************************************/

#include "IlcLHCDipValT.h"
#include "TObject.h"
class TObjArray;
//class IlcLHCDipValT;

class IlcDCSArray;
class TString;
class TMap;
class IlcLHCReader;


class IlcLHCData : public TObject
{
 public:
  enum          {kStart,kNStor};
  enum BeamID_t {kBeam1,kBeam2};
  enum Proj_t   {kX,kY};
  enum Side_t   {kLeft,kRight};
  enum Collim_t {kTCTVB4L2, kTCTVB4R2, kTCLIA4R2, kNCollimators};
  enum ColJaw_t {kGapDn,kGapUp,kLeftDn,kLeftUp,kRightDn,kRightUp,kNJaws};
  enum          {kMaxBSlots = 3564, kOffsBeam1=346, kOffsBeam2 = 3019};
  enum          {kMarginSOR = 60*60*24*30, // use margin of 30 days for SOR, when looking for the 1st record
		 kMarginEOR = 60*15};      // use margin of 15 min for EOR, when looking for the last record
  //
  enum {kIntTot,kIntTotAv,kIntBunchAv,
	kLumAcqMode,kLumTot,kLumTotErr,kLumBunch,kLumBunchErr,kLumCrossAng,kLumCrossAngErr,
	kBunchConf,kFillNum,kBunchLgtNB,kBunchLgt,kBunchLgtFillB,
	kRCInjSch,kRCBeta,kRCCrossAng,kRCVang,
	kBeamSzAcqMode,kBeamSzSigH,kBeamSzSigV,kBeamSzEmittH,kBeamSzEmittV,kBeamSzSigHErr,kBeamSzSigVErr,
	kCollPos};
  //
  //le
 public:
  //
 IlcLHCData() : fTMin(0),fTMax(1e10),fFillNumber(0),fData(0),fkFile2Process(0),fkMap2Process(0) {Clear();}
  IlcLHCData(const TMap*   dcsMap,  double tmin=0, double tmax=1.e10);
  IlcLHCData(const Char_t* dcsFile, double tmin=0, double tmax=1.e10);
  virtual ~IlcLHCData() {}
  //
  Bool_t                FillData(const TMap*   dcsMap,  double tmin=0, double tmax=1.e20);
  Bool_t                FillData(const Char_t* dcsFile, double tmin=0, double tmax=1.e20);
  Double_t              GetTMin()                                    const {return fTMin;}
  Double_t              GetTMax()                                    const {return fTMax;}
  Int_t                 GetFillNumber()                              const {return fFillNumber;}
  void                  SetFillNumber(Int_t fill)                          {fFillNumber = fill;}
  void                  SetTMin(Double_t t)                                {fTMin = t<0?0:(t>1e10?1e10:t);}
  void                  SetTMax(Double_t t)                                {fTMax = t<0?0:(t>1e10?1e10:t);}
  //
  virtual void          Print(const Option_t *opt="")                const;
  //
  Int_t GetNBunchConfigMeasured(int bm)           const {return GoodPairID(bm)?fBunchConfMeas[bm][kNStor]:-1;}
  Int_t GetNBunchConfigDeclared(int bm)           const {return GoodPairID(bm)?fBunchConfDecl[bm][kNStor]:-1;}
  Int_t GetNBunchLengths(int bm)                  const {return GoodPairID(bm)?fBunchLengths[bm][kNStor]:-1;}
  Int_t GetNTotalIntensity(int bm)                const {return GoodPairID(bm)?fIntensTotal[bm][kNStor]:-1;}
  Int_t GetNTotalIntensityAv(int bm)              const {return GoodPairID(bm)?fIntensTotalAv[bm][kNStor]:-1;}
  Int_t GetNIntensityPerBunch(int bm)             const {return GoodPairID(bm)?fIntensPerBunch[bm][kNStor]:-1;}
  Int_t GetNEmittanceH(int bm)                    const {return GoodPairID(bm)?fEmittanceH[bm][kNStor]:-1;}
  Int_t GetNEmittanceV(int bm)                    const {return GoodPairID(bm)?fEmittanceV[bm][kNStor]:-1;}
  Int_t GetNBeamSigmaH(int bm)                    const {return GoodPairID(bm)?fBeamSigmaH[bm][kNStor]:-1;}
  Int_t GetNBeamSigmaV(int bm)                    const {return GoodPairID(bm)?fBeamSigmaV[bm][kNStor]:-1;}
  //
  Int_t GetNLuminosityTotal(int lr)               const {return GoodPairID(lr)?fLuminTotal[lr][kNStor]:-1;}
  Int_t GetNLuminosityPerBunch(int lr)            const {return GoodPairID(lr)?fLuminPerBC[lr][kNStor]:-1;}
  Int_t GetNLuminosityAcqMode(int lr)             const {return GoodPairID(lr)?fLuminAcqMode[lr][kNStor]:-1;}
  Int_t GetNCrossingAngle(int lr)                 const {return GoodPairID(lr)?fCrossAngle[lr][kNStor]:-1;}
  //
  Int_t GetNInjectionScheme()                     const {return fRCInjScheme[kNStor];}
  Int_t GetNRCBetaStar()                          const {return fRCBeta[kNStor];}
  Int_t GetNRCAngleH()                            const {return fRCAngH[kNStor];}
  Int_t GetNRCAngleV()                            const {return fRCAngV[kNStor];}
  //
  Int_t GetNCollimatorJawPos(int coll,int jaw)    const;
  //
  IlcLHCDipValI* GetBunchConfigMeasured(int bm, int i=0)  const;
  IlcLHCDipValF* GetBunchLengths(int bm, int i=0)         const;
  IlcLHCDipValI* GetBunchConfigDeclared(int bm, int i=0)  const;
  IlcLHCDipValF* GetTotalIntensity(int bm, int i=0)       const;
  IlcLHCDipValF* GetTotalIntensityAv(int bm, int i=0)     const;
  IlcLHCDipValF* GetIntensityPerBunch(int bm, int i=0)    const;
  IlcLHCDipValF* GetEmittanceH(int bm, int i=0)           const;
  IlcLHCDipValF* GetEmittanceV(int bm, int i=0)           const;
  IlcLHCDipValF* GetBeamSigmaH(int bm, int i=0)           const;
  IlcLHCDipValF* GetBeamSigmaV(int bm, int i=0)           const;
  IlcLHCDipValF* GetLuminosityTotal(int lr, int i=0)      const;
  IlcLHCDipValF* GetLuminosityPerBunch(int lr, int i=0)   const;
  IlcLHCDipValI* GetLuminosityAcqMode(int lr, int i=0)    const;
  IlcLHCDipValF* GetCrossAngle(int lr, int i=0)           const;
  IlcLHCDipValC* GetInjectionScheme(int i=0)              const;
  IlcLHCDipValF* GetRCBetaStar(int i=0)                   const;
  IlcLHCDipValF* GetRCAngleH(int i=0)                     const; 
  IlcLHCDipValF* GetRCAngleV(int i=0)                     const; 
  IlcLHCDipValF* GetCollimJawPos(int coll, int jaw, int i=0) const;
  //
  void           FlagInteractingBunches(const Int_t beam1[2],const Int_t beam2[2]);
  TObject*       FindRecValidFor(int start,int nrec, double tstamp) const;
  IlcLHCDipValI* GetBunchConfigMeasured(int beam,double tstamp)  const;
  IlcLHCDipValI* GetBunchConfigDeclared(int beam,double tstamp)  const;
  Int_t          GetNInteractingBunchesMeasured(int i=0)         const;
  Int_t          GetNInteractingBunchesDeclared(int i=0)         const;
  Int_t          IsPilotPresent(int i=0)                         const;
  //
  // return array with beginning [0] and number of records for corresponding info (in the fData)
  const Int_t* GetOffsBunchConfigMeasured(int bm)         const {return GoodPairID(bm)?fBunchConfMeas[bm]:0;}
  const Int_t* GetOffsBunchConfigDeclared(int bm)         const {return GoodPairID(bm)?fBunchConfDecl[bm]:0;}
  const Int_t* GetOffsBunchLengths(int bm)                const {return GoodPairID(bm)?fBunchLengths[bm]:0;}
  const Int_t* GetOffsTotalIntensity(int bm)              const {return GoodPairID(bm)?fIntensTotal[bm]:0;}
  const Int_t* GetOffsTotalIntensityAv(int bm)            const {return GoodPairID(bm)?fIntensTotalAv[bm]:0;}
  const Int_t* GetOffsIntensityPerBunch(int bm)           const {return GoodPairID(bm)?fIntensPerBunch[bm]:0;}
  const Int_t* GetOffsEmittanceH(int bm)                  const {return GoodPairID(bm)?fEmittanceH[bm]:0;}
  const Int_t* GetOffsEmittanceV(int bm)                  const {return GoodPairID(bm)?fEmittanceV[bm]:0;}
  const Int_t* GetOffsBeamSigmaH(int bm)                  const {return GoodPairID(bm)?fBeamSigmaH[bm]:0;}
  const Int_t* GetOffsBeamSigmaV(int bm)                  const {return GoodPairID(bm)?fBeamSigmaV[bm]:0;}
  //
  const Int_t* GetOffsLuminosityTotal(int lr)             const {return GoodPairID(lr)?fLuminTotal[lr]:0;}
  const Int_t* GetOffsLuminosityPerBunch(int lr)          const {return GoodPairID(lr)?fLuminPerBC[lr]:0;}
  const Int_t* GetOffsLuminosityAcqMode(int lr)           const {return GoodPairID(lr)?fLuminAcqMode[lr]:0;}
  const Int_t* GetOffsCrossingAngle(int lr)               const {return GoodPairID(lr)?fCrossAngle[lr]:0;}
  //
  const Int_t* GetOffsInjectionScheme()                   const {return fRCInjScheme;}
  const Int_t* GetOffsRCBetaStar()                        const {return fRCBeta;}
  const Int_t* GetOffsRCAngleH()                          const {return fRCAngH;}
  const Int_t* GetOffsRCAngleV()                          const {return fRCAngV;}
  //
  const Int_t* GetOffsCollimatorJawPos(int coll,int jaw)  const;
  //
  const TObjArray&  GetData()                             const {return fData;}
  //
  // analysis methods
  Int_t GetMeanIntensity(int beamID, Double_t &colliding, Double_t &noncolliding, const TObjArray* bcmasks=0) const;
  static Int_t GetBCId(int bucket, int beamID)                  {return (TMath::Abs(bucket)/10 + (beamID==0 ? kOffsBeam1:kOffsBeam2))%kMaxBSlots;}
  //
 protected:
  //
  Bool_t                FillData(double tmin=0, double tmax=1.e20);
  virtual void          Clear(const Option_t *opt="");
  void                  PrintAux(Bool_t full,const Int_t refs[2],const Option_t *opt="") const;
  TObjArray*            GetDCSEntry(const char* key,int &entry,int &last,double tmin,double tmax) const;
  Int_t                 FillScalarRecord(  int refs[2], const char* rec, const char* recErr=0, Double_t maxAbsVal=1.e30);
  Int_t                 FillBunchConfig(   int refs[2], const char* rec);
  Int_t                 FillStringRecord(  int refs[2], const char* rec);
  Int_t                 FillAcqMode(       int refs[2], const char* rec);
  Int_t                 FillBunchInfo(     int refs[2], const char* rec,int ibm, Bool_t inRealSlots, Double_t maxAbsVal=1.e30);
  Int_t                 FillBCLuminosities(int refs[2], const char* rec, const char* recErr, Int_t useBeam, Double_t maxAbsVal=1.e30);
  //
  Int_t                 ExtractInt(IlcDCSArray* dcsArray,Int_t el)    const;
  Double_t              ExtractDouble(IlcDCSArray* dcsArray,Int_t el) const;
  TString&              ExtractString(IlcDCSArray* dcsArray)          const;
 IlcLHCData(const IlcLHCData& src) : TObject(src),fTMin(0),fTMax(0),fFillNumber(0),fData(0),fkFile2Process(0),fkMap2Process(0) { /*dummy*/ }
  IlcLHCData& operator=(const IlcLHCData& ) { /*dummy*/ return *this;}
  Int_t                 TimeDifference(double v1,double v2,double tol=0.9) const;
  Bool_t                IzZero(double val, double tol=1e-16)         const {return TMath::Abs(val)<tol;}
  Bool_t                GoodPairID(int beam)                         const;
  //
 protected:
  //
  Double_t        fTMin;                              // selection timeMin
  Double_t        fTMax;                              // selection timeMax
  Int_t           fFillNumber;                        // fill number           : kFillNum
  //
  //---------------- Last index gives: 0 - beginning of the records in fData, 1 - number of records
  //
  //              infrormation from RunControl
  Int_t           fRCInjScheme[2];                    // active injection scheme                       : String |kRCInjScheme
  Int_t           fRCBeta[2];                         // target beta                                   : Float  |kRCBeta
  Int_t           fRCAngH[2];                         // horisontal angle                              : Float  |kRCCrossAng
  Int_t           fRCAngV[2];                         // vertical angle                                : Float  |kRCVang
  Int_t           fBunchConfDecl[2][2];               // declared beam configuration                   : Float  |kBunchConf                
  //
  //              measured information
  Int_t           fBunchConfMeas[2][2];               // measured beam configuration                   : Int    |kBunchLgtFillB
  Int_t           fBunchLengths[2][2];                // measured beam lenghts                         : Float  |kBunchLgt
  Int_t           fIntensTotal[2][2];                 // total beam intensities                        : Float  |kIntTot
  Int_t           fIntensTotalAv[2][2];               // total beam intensities from bunch averages    : Float  |kIntTotAv
  Int_t           fIntensPerBunch[2][2];              // bunch-by-bunch intensities                    : Float  |kIntBunchAv
  //
  Int_t           fCrossAngle[2][2];                  // crossing angle   at IP2 and its error         : Float  |kLimCrossAng, kLumCrossAngErr
  Int_t           fEmittanceH[2][2];                  // beam H emittances                             : Float  |kBeamSzEmittH
  Int_t           fEmittanceV[2][2];                  // beam V emittances                             : Float  |kBeamSzEmittV
  Int_t           fBeamSigmaH[2][2];                  // beam H sigma and error                        : Float  |kBeamSzSigH,kBeamSzSigHErr
  Int_t           fBeamSigmaV[2][2];                  // beam V sigma and error                        : Float  |kBeamSzSigV,kBeamSzSigVErr
  //
  Int_t           fLuminTotal[2][2];                  // total luminosity at IP2 and its error         : Float  |kLumTot, kLumTotErr
  Int_t           fLuminPerBC[2][2];                  // luminosity at IP2 for each BC and its error   : Float  |kLumBunch,kLumBunchErr
  Int_t           fLuminAcqMode[2][2];                // luminosity acquisition mode                   : Int    | kLumAcqMode
  //
  Int_t           fCollimators[kNCollimators][kNJaws][2];// collimator jaws positions                  : Float  |kCollPos
  //
  TObjArray       fData;                              // single storage for various records
  //
  static const Char_t *fgkDCSNames[];                 // beam related DCS names to extract
  static const Char_t *fgkDCSColNames[];              // collimators to extract
  static const Char_t *fgkDCSColJaws[];               // names of collimator pieces
  //
 private:
  // non-persistent objects used at the filling time
  const Char_t*   fkFile2Process;                      //! name of DCS file
  const TMap*     fkMap2Process;                       //! DCS map to process 

  ClassDef(IlcLHCData,2)
};


//_____________________________________________________________________________
inline Int_t IlcLHCData::GetNCollimatorJawPos(int coll,int jaw) const {// get n records
  return (coll>=0&&coll<kNCollimators&&jaw>=0&&jaw<kNJaws)? fCollimators[coll][jaw][kNStor]:0;
}

inline const Int_t* IlcLHCData::GetOffsCollimatorJawPos(int coll,int jaw)  const { // offset array
  return (coll>=0&&coll<kNCollimators&&jaw>=0&&jaw<kNJaws)? fCollimators[coll][jaw]:0;
}

inline IlcLHCDipValI* IlcLHCData::GetBunchConfigMeasured(int bm, int i) const { // get record
  return (GoodPairID(bm) && i>=0 && i<fBunchConfMeas[bm][kNStor]) ? (IlcLHCDipValI*)fData[fBunchConfMeas[bm][kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetBunchLengths(int bm, int i) const { // get record
  return (GoodPairID(bm) && i>=0 && i<fBunchLengths[bm][kNStor]) ? (IlcLHCDipValF*)fData[fBunchLengths[bm][kStart]+i]:0;
}

inline IlcLHCDipValI* IlcLHCData::GetBunchConfigDeclared(int bm, int i) const { // get record
  return (GoodPairID(bm) && i>=0 && i<fBunchConfDecl[bm][kNStor]) ? (IlcLHCDipValI*)fData[fBunchConfDecl[bm][kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetTotalIntensity(int bm, int i) const { // get record
  return (GoodPairID(bm) && i>=0 && i<fIntensTotal[bm][kNStor]) ? (IlcLHCDipValF*)fData[fIntensTotal[bm][kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetTotalIntensityAv(int bm, int i) const { // get record
  return (GoodPairID(bm) && i>=0 && i<fIntensTotalAv[bm][kNStor]) ? (IlcLHCDipValF*)fData[fIntensTotalAv[bm][kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetIntensityPerBunch(int bm, int i) const { // get record
  return (GoodPairID(bm) && i>=0 && i<fIntensPerBunch[bm][kNStor]) ? (IlcLHCDipValF*)fData[fIntensPerBunch[bm][kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetEmittanceH(int bm, int i) const { // get record
  return (GoodPairID(bm) && i>=0 && i<fEmittanceH[bm][kNStor]) ? (IlcLHCDipValF*)fData[fEmittanceH[bm][kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetEmittanceV(int bm, int i) const { // get record
  return (GoodPairID(bm) && i>=0 && i<fEmittanceV[bm][kNStor]) ? (IlcLHCDipValF*)fData[fEmittanceV[bm][kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetBeamSigmaH(int bm, int i) const { // get record
  return (GoodPairID(bm) && i>=0 && i<fBeamSigmaH[bm][kNStor]) ? (IlcLHCDipValF*)fData[fBeamSigmaH[bm][kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetBeamSigmaV(int bm, int i) const { // get record
  return (GoodPairID(bm) && i>=0 && i<fBeamSigmaV[bm][kNStor]) ? (IlcLHCDipValF*)fData[fBeamSigmaV[bm][kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetLuminosityTotal(int lr, int i) const { // get record
  return (GoodPairID(lr) && i>=0 && i<fLuminTotal[lr][kNStor]) ? (IlcLHCDipValF*)fData[fLuminTotal[lr][kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetLuminosityPerBunch(int lr, int i) const { // get record
  return (GoodPairID(lr) && i>=0 && i<fLuminPerBC[lr][kNStor]) ? (IlcLHCDipValF*)fData[fLuminPerBC[lr][kStart]+i]:0;
}

inline IlcLHCDipValI* IlcLHCData::GetLuminosityAcqMode(int lr, int i) const { // get record
  return (GoodPairID(lr) && i>=0 && i<fLuminAcqMode[lr][kNStor]) ? (IlcLHCDipValI*)fData[fLuminAcqMode[lr][kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetCrossAngle(int lr, int i) const { // get record
  return (GoodPairID(lr) && i>=0 && i<fCrossAngle[lr][kNStor]) ? (IlcLHCDipValF*)fData[fCrossAngle[lr][kStart]+i]:0;
}

inline IlcLHCDipValC* IlcLHCData::GetInjectionScheme(int i) const { // get record
  return (i>=0 && i<fRCInjScheme[kNStor]) ? (IlcLHCDipValC*)fData[fRCInjScheme[kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetRCBetaStar(int i) const { // get record
  return (i>=0 && i<fRCBeta[kNStor]) ? (IlcLHCDipValF*)fData[fRCBeta[kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetRCAngleH(int i) const { // get record
  return (i>=0 && i<fRCAngH[kNStor]) ? (IlcLHCDipValF*)fData[fRCAngH[kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetRCAngleV(int i) const { // get record
  return (i>=0 && i<fRCAngV[kNStor]) ? (IlcLHCDipValF*)fData[fRCAngV[kStart]+i]:0;
}

inline IlcLHCDipValF* IlcLHCData::GetCollimJawPos(int coll, int jaw, int i) const { // get record
  return (coll>=0 && coll<kNCollimators && jaw>=0 && jaw<kNJaws && 
	  i>=0 && i<fCollimators[coll][jaw][kNStor]) ? (IlcLHCDipValF*)fData[fCollimators[coll][jaw][kStart]+i]:0;
}


#endif
