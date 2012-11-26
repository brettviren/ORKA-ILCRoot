#ifndef ILCTARGETCLUSTERFINDER_H
#define ILCTARGETCLUSTERFINDER_H
 
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */
 
/* $Id: IlcTARGETClusterFinder.h,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

////////////////////////////////////////////////
//  TARGET Cluster Finder Class                  //
//                                            //
//                                            //
////////////////////////////////////////////////

#include <TObject.h>
#include <TClonesArray.h>

class IlcTARGETMap;
class IlcTARGETresponse;
class IlcTARGETsegmentation;
class IlcTARGETRawCluster;
class IlcTARGETgeom;
class IlcTARGETdigit;
class IlcTARGETRecPoint;
class IlcTARGETDetTypeRec;

//----------------------------------------------------------------------
class IlcTARGETClusterFinder :public TObject{
  public:
    IlcTARGETClusterFinder(); // Default constructor
    // Standard Constructor
    IlcTARGETClusterFinder(IlcTARGETDetTypeRec* dettyp);
    IlcTARGETClusterFinder(IlcTARGETDetTypeRec* dettyp,TClonesArray *digits);// Standard+ Constructor
    virtual ~IlcTARGETClusterFinder(); // Destructor
    //
    // Do the Reconstruction.
    virtual void FindRawClusters(Int_t mod=0); // Finds cluster of digits.
     //
    // Sets the debug flag for debugging output
    void SetDebug(Int_t level=1){fDebug=level;}
    // Clears the debug flag so no debugging output will be generated
    void SetNoDebug(){fDebug=0;}
    // Returns the debug flag value
    Bool_t GetDebug(Int_t level=1)const {return fDebug>=level;}
    // Digit
    virtual void SetDigits(TClonesArray *itsDigits) {// set digits
        fDigits=itsDigits;fNdigits = fDigits->GetEntriesFast();}
    virtual IlcTARGETdigit* GetDigit(Int_t i){ // Returns ith digit
        return (IlcTARGETdigit*) fDigits->UncheckedAt(i);}
    virtual TClonesArray* Digits(){return fDigits;}// Gets fDigits
    virtual Int_t   NDigits() const {return fNdigits;}// Get Number of Digits
    // clulsters
    // Set fClusters up
    virtual void SetClusters(TClonesArray *itsClusters){// set clusters
        fClusters = itsClusters;fNRawClusters = fClusters->GetEntriesFast();}
    // Get fCluters
    virtual TClonesArray* Clusters(){return fClusters;}
    // Get fCluter
    virtual IlcTARGETRawCluster* Cluster(Int_t i){
      return (IlcTARGETRawCluster*)(fClusters->UncheckedAt(i));}
    // Returns the present number of enteries
    virtual Int_t NClusters()const {return fClusters->GetEntriesFast();}
    // returns fNRawClusters
    virtual Int_t GetNRawClusters() const {return fNRawClusters;}
    // Determins if digit i has a neighbor and if so that neighor index is j.
    virtual void AddCluster(Int_t branch,IlcTARGETRawCluster *c);
    virtual void AddCluster(Int_t branch,IlcTARGETRawCluster *c,
                            IlcTARGETRecPoint &rp);
    virtual void   FillCluster(IlcTARGETRawCluster *,Int_t) {}// fiil cluster
    virtual void   FillCluster(IlcTARGETRawCluster *cluster) {// fill cluster
        FillCluster(cluster,1);}

    virtual void SetModule(Int_t module){fModule = module;}// Set module number
    virtual Int_t GetModule()const {return fModule;}// Returns module number
    //
    // RecPoints
    // Given a cluster of digits, creates the nessesary RecPoint. May also
    // do some peak separation.
    virtual void CreateRecPoints(TObjArray *,Int_t){};
    // Others
    virtual void  SetMap(Int_t lay,TObject *m) {fMap->AddAt(m,lay);}// map
    IlcTARGETMap* Map(Int_t lay){return (IlcTARGETMap*)fMap->UncheckedAt(lay);}// map
    virtual Int_t GetNperMax() const {return fNperMax;}// returns fNperMax
    // returns fDeclusterFlag
    virtual Int_t GetDeclusterFlag()const{return fDeclusterFlag;} 
    // returns fClusterSize
    virtual Int_t GetClusterSize() const {return fClusterSize;} 
    virtual Int_t GetNPeaks() const {return fNPeaks;}// returns fNPeaks
    //
    virtual Bool_t IsNeighbor(TObjArray *digs,Int_t i,Int_t j[]) const;
    virtual void Decluster(IlcTARGETRawCluster *) {}// Decluster
    // Set max. Number of cells per local cluster
    virtual void SetNperMax(Int_t npermax=3) {fNperMax = npermax;}
    //Decluster
    virtual void SetDeclusterFlag(Int_t flag=1){fDeclusterFlag=flag;}
        // Set max. cluster size ; bigger clusters will be rejected
    virtual void SetClusterSize(Int_t clsize=3) {fClusterSize = clsize;}
    virtual void CalibrateCOG() {}// Self Calibration of COG 
    virtual void CorrectCOG(){}// correct COG
    virtual Bool_t Centered(IlcTARGETRawCluster *) const {return kTRUE;}// cluster
    //split by local maxima
    virtual void SplitByLocalMaxima(IlcTARGETRawCluster *){}
    // IO functions
    void Print(ostream *os) const; // Class ascii print function
    void Read(istream *os);  // Class ascii read function
    virtual void Print(Option_t *option="") const {TObject::Print(option);}
    virtual Int_t Read(const char *name) {return TObject::Read(name);}

    virtual void SetDetTypeRec(IlcTARGETDetTypeRec* dtr) {fDetTypeRec=dtr;}
    IlcTARGETDetTypeRec* GetDetTypeRec() const {return fDetTypeRec;}

    void InitGeometry(); 
 
  protected:
    // methods 
    IlcTARGETClusterFinder(const IlcTARGETClusterFinder &source); // copy constructor
    // assignment operator
    IlcTARGETClusterFinder& operator=(const IlcTARGETClusterFinder &source);
   // data members       

   Int_t              fDebug;         //! Debug flag/level
   Int_t              fModule;        //! Module number to be reconstuctted
   TClonesArray       *fDigits;       //! digits 
   Int_t              fNdigits;       //! num of digits 
 
   IlcTARGETDetTypeRec* fDetTypeRec; //TARGET object for reconstruction
   TClonesArray       *fClusters;     //! Array of clusters
   Int_t              fNRawClusters;  //! in case we split the cluster
   // and want to keep track of 
   // the cluster which was splitted
    TObjArray          *fMap;          //! map
    Int_t              fNperMax;       //! NperMax
    Int_t              fDeclusterFlag; //! DeclusterFlag
    Int_t              fClusterSize;   //! ClusterSize
    Int_t              fNPeaks;        //! NPeaks  
    // Data members needed to fill IlcCluster objects
    Float_t fYshift[100];       // y-shifts of detector local coor. systems 
    Float_t fZshift[100];       // z-shifts of detector local coor. systems 
    Int_t fNdet[100];           // detector index  
    Int_t fNlayer[100];         // detector layer


    ClassDef(IlcTARGETClusterFinder,6) //Class for clustering and reconstruction of space points
};
// Input and output functions for standard C++ input/output.
ostream &operator<<(ostream &os,IlcTARGETClusterFinder &source);
istream &operator>>(istream &os,IlcTARGETClusterFinder &source);
#endif
