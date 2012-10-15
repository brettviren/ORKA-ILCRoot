#ifndef ILCTARGETCLUSTERFINDERSPD_H
#define ILCTARGETCLUSTERFINDERSPD_H

////////////////////////////////////////////////
//  TARGET Cluster Finder Class                 //
////////////////////////////////////////////////

#include "IlcTARGETClusterFinder.h"
#include "IlcTARGETDetTypeRec.h"

class IlcTARGETMapA1;
class IlcTARGETsegmentation;
class IlcTARGETsegmentationSPD;
class IlcTARGETresponse;
class IlcTARGETresponseSPD;
class TClonesArray;


class IlcTARGETClusterFinderSPD : public IlcTARGETClusterFinder{
 public:
    IlcTARGETClusterFinderSPD();
    IlcTARGETClusterFinderSPD(IlcTARGETDetTypeRec* dettyp);
    IlcTARGETClusterFinderSPD(IlcTARGETDetTypeRec* dettyp,
			   TClonesArray *digits,TClonesArray *recpoints);
    virtual ~IlcTARGETClusterFinderSPD(){}// destructor
    //Returns fSegmentation
    virtual IlcTARGETsegmentationSPD* GetSeg()const{
        return (IlcTARGETsegmentationSPD*)fDetTypeRec->GetSegmentationModelSPD(0);}  
    virtual void SetDx(Double_t dx=1.) {fDx=dx;}// set dx
    virtual void SetDz(Double_t dz=0.) {fDz=dz;}// set dz
    // Search for clusters
    virtual void FindRawClusters(Int_t module); 
    void  DigitToPoint(Int_t nclus, Double_t *xcenter, Double_t *zcenter,
                       Double_t *errxcenter,Double_t *errzcenter,
                       Int_t *tr1clus, Int_t *tr2clus, Int_t *tr3clus);
    void  ClusterFinder(Int_t ndigits,Int_t digx[],Int_t digz[],
                        Int_t digtr1[],Int_t digtr2[],Int_t digtr3[],
                        Int_t digtr4[],
                        Int_t &nclus,
                        Double_t xcenter[],Double_t zcenter[],
                        Double_t errxcenter[],Double_t errzcenter[],  
                        Int_t tr1clus[],Int_t tr2clus[], Int_t tr3clus[]);
 protected:
    // copy constructor
    IlcTARGETClusterFinderSPD(const IlcTARGETClusterFinderSPD &source);
    // assignment operator
    IlcTARGETClusterFinderSPD& operator=(const IlcTARGETClusterFinderSPD &source);

    Double_t             fDz;            // dz
    Double_t             fDx;            // dx
    Int_t               fMinNCells;     // min num of cells in the cluster
  
    ClassDef(IlcTARGETClusterFinderSPD,2)  // SPD clustering
};
#endif
