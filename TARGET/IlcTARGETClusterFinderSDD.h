#ifndef ILCTARGETCLUSTERFINDERSDD_H
#define ILCTARGETCLUSTERFINDERSDD_H

////////////////////////////////////////////////
//  TARGET Cluster Finder Class                 //
////////////////////////////////////////////////
/*
  $Id: IlcTARGETClusterFinderSDD.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $
*/

#include "IlcTARGETClusterFinder.h"
#include "IlcTARGETDetTypeRec.h"

// class IlcTARGETMapA2;
class IlcTARGETCalibration;
class IlcTARGETCalibrationSDD;
class IlcTARGETsegmentation;
class IlcTARGETsegmentationSDD;
class TFile;

class IlcTARGETClusterFinderSDD : public IlcTARGETClusterFinder{
  public:
    IlcTARGETClusterFinderSDD
        (IlcTARGETDetTypeRec* dettyp,
         TClonesArray *digits,TClonesArray *recpoints);
    IlcTARGETClusterFinderSDD();
    virtual ~IlcTARGETClusterFinderSDD(){};
    
    virtual void  SetCutAmplitude(Int_t mod,Double_t nsigma=4);
    virtual Int_t CutAmplitude(Int_t anode) const {// get cut amplitude
        return fCutAmplitude[anode];}
    virtual void SetDAnode(Double_t danode=4.2) {// setDAnode
        fDAnode=danode;}
    virtual Double_t DAnode() const {// get DAnode
        return fDAnode;}
    virtual void SetDTime(Double_t dtime=75) {// SetDTime
        fDTime=dtime;}
    virtual Double_t DTime() const {// get DTime
        return fDTime;}
    virtual void SetMinPeak(Int_t minpeak=10) {// SetMinPeak
        fMinPeak=minpeak;}
    virtual Int_t MinPeak() const {// get MinPeak
        return fMinPeak;}
    virtual void SetMinCharge(Int_t mincharge=30) {// SetMinCharge
        fMinCharge=mincharge;}
    virtual Int_t MinCharge() const {// get MinCharge
        return fMinCharge;}
    virtual void SetMinNCells(Int_t minc=3) {// setNCells
        fMinNCells=minc;}
    virtual Int_t MinNCells() const {// get MinNCells
        return fMinNCells;}
    virtual void SetMaxNCells(Int_t maxc=10) {// setNCells
        fMaxNCells=maxc;}
    virtual Int_t MaxNCells() const {// get MaxNCells
        return fMaxNCells;}
    virtual void SetTimeCorr(Double_t timec=19.3) {// setNCells
        fTimeCorr=timec;}
    virtual Double_t TimeCorr() const{// get Time Correction (ns)
        return fTimeCorr;}

    // Search for clusters
    virtual void FindRawClusters(Int_t mod=0);
    void  Find1DClusters();
    void  Find1DClustersE();
    void  GroupClusters();
    void  SelectClusters();
    void  GetRecPoints();
    void  ResolveClusters(); // Ernesto Lopez Torres
    Int_t SearchPeak(Double_t *spect,Int_t xdim,Int_t zdim,Int_t *peakX,
                     Int_t *peakZ,Double_t *peakAmp,Double_t minpeak);//Ernesto
    Int_t NoLinearFit( Int_t xdim, Int_t zdim, Double_t *param, Double_t *spe,
                       Int_t *niter, Double_t *chir );
    void  Minim(Int_t xdim,Int_t zdim,Double_t *param,Double_t *prm0,
             Double_t *steprm,Double_t *chisqr,Double_t *spe,Double_t *speFit);
    Double_t ChiSqr(Int_t xdim,Int_t zdim,Double_t *spe,Double_t *speFit)const;
    void  PeakFunc( Int_t xdim, Int_t zdim, Double_t *par, Double_t *spe,
                    Double_t *Integral=0 );
    void  PrintStatus() const;

  private:
    virtual IlcTARGETCalibrationSDD* GetResp(Int_t mod)const{
        return (IlcTARGETCalibrationSDD*) fDetTypeRec->GetCalibrationModel(mod);}//Return Response
    //Returns fSegmentation
    virtual IlcTARGETsegmentationSDD* GetSeg()const{
        return (IlcTARGETsegmentationSDD*)fDetTypeRec->GetSegmentationModel(1);} 
    IlcTARGETClusterFinderSDD(const IlcTARGETClusterFinderSDD &source); // copy ctor
    IlcTARGETClusterFinderSDD& operator=(const IlcTARGETClusterFinderSDD &source);
  private:
    Int_t               fNclusters;     //! num of clusters
    Double_t            fDAnode;        //! fDanode
    Double_t            fDTime;         //! fDtime
    Double_t            fTimeCorr;      //! Correction factor along time coord
    TArrayI             fCutAmplitude;  //! cut amplitude
    Int_t               fMinPeak;       //! min peak
    Int_t               fMinCharge;     //! min charge
    Int_t               fMinNCells;     //! min num of cells
    Int_t               fMaxNCells;     //! max num of cells

    ClassDef(IlcTARGETClusterFinderSDD,3) // SDD clustering - Piergiorgio C. algo
    };

#endif
