// IlcTARGETtrackerANN header file.
// Class definition for the Neural Tracking implementation
// for the ILC TARGET stand-alone.
// Author: A. Pulvirenti
// Email : alberto.pulvirenti@ct.infn.it

#ifndef ILCTARGETTRACKERANN_H
#define ILCTARGETTRACKERANN_H

#include "IlcTARGETtrackerV2.h"

class IlcTARGETgeom;
class TArrayI;

class IlcTARGETtrackerANN : public IlcTARGETtrackerV2 
{
public:


	/* Constructors */
        IlcTARGETtrackerANN();
	IlcTARGETtrackerANN(const IlcTARGETgeom *geom, Int_t msglev = 0);
	IlcTARGETtrackerANN(const IlcTARGETtrackerANN &n);
	IlcTARGETtrackerANN& operator=(const IlcTARGETtrackerANN& arg);
	
        virtual Int_t Clusters2Tracks(IlcESDEvent *event){IlcFatal("Not implemented"); return 0;}
        virtual Int_t PropagateBack(IlcESDEvent *event){IlcFatal("Not implemented"); return 0;}
        virtual Int_t RefitInward(IlcESDEvent *event){IlcFatal("Not implemented"); return 0;}

        /* Destructor */
	// virtual ~IlcTARGETtrackerANN();
	
	
	/*********************************************
	  >> IlcTARGETnode <<
	  ----------------
	  An TARGET point in the global reference frame.                      
	 *********************************************/
	class IlcTARGETnode : public TObject {
	public:
	
		IlcTARGETnode();
		~IlcTARGETnode();
		IlcTARGETnode(const IlcTARGETnode &n); 
 
		IlcTARGETnode& operator=(const IlcTARGETnode& /*arg*/)
		{ return *this; }
	
		/* (NODE) Function to extract layer info from cluster index */
		Int_t  GetLayer() const {Int_t lay = (fClusterRef & kClusterLayerMask) >> kClusterLayerShift; return lay;}
		
		/* (NODE) Getter and Setter for the 'fUsed' flag */
		Bool_t IsUsed() const {return fUsed;}
		void   Use()          {fUsed = kTRUE;}
		
		/* (NODE) Geometrical functions */
		Double_t  GetR2()    const  {return TMath::Sqrt(GetR2sq());}   // xy  radius
		Double_t  GetR3()    const  {return TMath::Sqrt(GetR3sq());}   // xyz radius
		Double_t  GetR2sq()  const  {return fX*fX+fY*fY;}              // xy  radius^2
		Double_t  GetR3sq()  const  {return GetR2sq()+fZ*fZ;}          // xyz radius^2
		Double_t  GetPhi()   const;
		Double_t  GetTheta() const  {return TMath::ATan2(GetR2(),fZ);} // polar angle
		Double_t  GetError(Option_t *opt);                             // errors on coords
		
		/* Data members references */
		Double_t& X()            {return fX;}
		Double_t& Y()            {return fY;}
		Double_t& Z()            {return fZ;}
		Double_t& ErrX2()        {return fEX2;}
		Double_t& ErrY2()        {return fEY2;}
		Double_t& ErrZ2()        {return fEZ2;}
		Int_t&    ClusterRef()   {return fClusterRef;}
		
		TObjArray*&   Matches()  {return fMatches;}
		TObjArray*&   InnerOf()  {return fInnerOf;}
		TObjArray*&   OuterOf()  {return fOuterOf;}
		IlcTARGETnode*&  Next()     {return fNext;}
		IlcTARGETnode*&  Prev()     {return fPrev;}
		
	private:
		
		Double_t    fX;           // X in global reference system
		Double_t    fY;           // Y in global reference system
		Double_t    fZ;           // Z in global reference system
		Double_t    fEX2;         // X error in global reference system
		Double_t    fEY2;         // Y error in global reference system
		Double_t    fEZ2;         // Z error in global reference system
		
		Bool_t      fUsed;        //  usage flag
		Int_t       fClusterRef;  //  reference index for related cluster
		TObjArray  *fMatches;     //! array of well-matched cluster indexes
		TObjArray  *fInnerOf;     //! array of neurons starting from *this
		TObjArray  *fOuterOf;     //! array of neurons entering *this
		IlcTARGETnode *fNext;        //! pointer to the next node in the found track
		IlcTARGETnode *fPrev;        //! pointer to the previous node in the found track
		
	};
	/* End of IlcTARGETnode class */
	
		
	/***************************************
	  >> IlcTARGETneuron <<
	  ------------------
	  A single unit in the neural network.
	 ***************************************/
	class IlcTARGETneuron : public TObject {
	public:
	
		/* (NEURON) Constructor: alocates the sequenced units array */
		IlcTARGETneuron() : fUsed(0), fActivation(0.), fInner(0), fOuter(0), fGain(0) 
			{fGain = new TObjArray;}
		IlcTARGETneuron(IlcTARGETnode *inner, IlcTARGETnode *outer, Double_t minAct, Double_t maxAct);
		IlcTARGETneuron(const IlcTARGETneuron &n);
		IlcTARGETneuron& operator=(const IlcTARGETneuron& arg)
		{this->~IlcTARGETneuron();new(this) IlcTARGETneuron(arg);
		return *this; }
		
								
		/* (NEURON) Destructor: frees memory from dynamic allocated objects */
		virtual ~IlcTARGETneuron()	
			{ fInner = fOuter = 0; fGain->SetOwner(kFALSE); fGain->Clear(); delete fGain; }

		/* Compute neural activation from the network */
		Double_t  Activate(Double_t temperature);
		
		/* Data members references */
		Int_t&        Used()        {return fUsed;}
		Double_t&     Activation()  {return fActivation;}
		IlcTARGETnode*&  Inner()       {return fInner;}
		IlcTARGETnode*&  Outer()       {return fOuter;}
		TObjArray*&   Gain()        {return fGain;}
		
	private:
	
		Int_t             fUsed;        //  utility flag
		Double_t          fActivation;  //  Activation value
		IlcTARGETnode       *fInner;       //! inner point
		IlcTARGETnode       *fOuter;       //! outer point
		TObjArray        *fGain;        //! list of sequenced units
		
	};
	/* End of NEURON class */
	
	
	/**************************************************
	  >> IlcTARGETlink <<
	  ----------------
	  A synaptic connected unit in the neural network.
	 **************************************************/
	class IlcTARGETlink : public TObject {
	public:
	
		IlcTARGETlink():fWeight(0.0),fLinked() {}
		IlcTARGETlink(Double_t w, IlcTARGETneuron *n) : fWeight(w), fLinked(n) { }
		virtual ~IlcTARGETlink() {fLinked = 0;}
		IlcTARGETlink(const IlcTARGETlink &n) : TObject((TObject&)n),fWeight(n.fWeight),fLinked(n.fLinked) {}
		IlcTARGETlink& operator=(const IlcTARGETlink& arg)
		{ this->~IlcTARGETlink();new(this) IlcTARGETlink(arg);
		return *this; }
		
		/* contribution */
		Double_t Contribution() {return fLinked->Activation() * fWeight;}
		
	private:
	
		Double_t      fWeight;  //  Weight value
		IlcTARGETneuron *fLinked;  //! Connected neuron
	
	};
	/* End of IlcTARGETlink class */
	
	/**************************************
	  >> IlcTARGETtrackANN <<
	  ----------------
	  A track found by the neural network.
	 **************************************/
	class IlcTARGETtrackANN : public TObject {
	public:
	
		IlcTARGETtrackANN(Int_t dim = 0);
		IlcTARGETtrackANN(const IlcTARGETtrackANN &n); 
		IlcTARGETtrackANN& operator=(const IlcTARGETtrackANN& /*arg*/)
		{ return *this; }
	
		Double_t&  XCenter()                 {return fXCenter;}
		Double_t&  YCenter()                 {return fYCenter;}
		Double_t&  Radius()                  {return fRadius;}
		Double_t&  Curv()                    {return fCurv;}
		Double_t&  ImpactParameterTrans()    {return fDTrans;}
		Double_t&  ImpactParameterLong()     {return fDLong;}
		Double_t&  TanLambda()               {return fTanLambda;}
		Double_t&  Phi()                     {return fPhi;}
		
		IlcTARGETnode* operator[](Int_t i) const {return ((i>=0&&i<fNPoints)?fNode[i]:0);}
		IlcTARGETnode* GetNode(Int_t i) const {return ((i>=0&&i<fNPoints)?fNode[i]:0);}
		void        SetNode(Int_t i, IlcTARGETnode*& ref) {if (i>=0&&i<fNPoints) fNode[i] = ref;}
		
		Int_t  CheckOccupation() const;
		Bool_t RiemannFit();
	
	private:
	
		Int_t      fNPoints;    // nuber of track elements
		
		Double_t   fXCenter;    // X of bending circle center
		Double_t   fYCenter;    // Y of bending circle center
		Double_t   fRadius;     // radius of bending circle
		Double_t   fCurv;       // signed curvature of bending circle 
		Double_t   fDTrans;     // transverse impact parameter
		Double_t   fDLong;      // longitudinal impact parameter
		Double_t   fTanLambda;  // tangent of dip angle
		Double_t   fPhi;        // initial direction of transverse momentum
		
		IlcTARGETnode **fNode;     // pointers to track elements
		
	};
	/* End of IlcTARGETtrackANN class */
	
	/* Geometry setter */
	void SetTARGETgeom(IlcTARGETgeom *geom)             {fGeom = (IlcTARGETgeom*)geom;}
	
	/* Flag setter */
	void SetMessageLevel(Int_t lev)               {fMsgLevel = lev;}
		
	/* Cut setters */
	void SetCuts(Int_t ncurv = 0, Double_t *curv = 0, // curvature (number of cuts and array)
	             Double_t *theta2D = 0,               // cut in theta 2D (rho-z)
                Double_t *theta3D = 0,               // cut in theta 3D (x-y-z)
                Double_t *helix = 0);                // cut for helix-matching
	
	/* Setter for overall Theta cut */
	void SetPolarInterval(Double_t dtheta)        {fPolarInterval=dtheta;}

	/* Neural work-flow setters */
	void SetActThreshold(Double_t val)            {fActMinimum = val;}
	void SetWeightExponent(Double_t a)            {fExponent = a;}
	void SetGainToCostRatio(Double_t a)           {fGain2CostRatio = a;}
	void SetInitInterval(Double_t a, Double_t b)  {fEdge1 = a; fEdge2 = b;}
	void SetTemperature(Double_t a)               {fTemperature = a;}
	void SetVariationLimit(Double_t a)            {fStabThreshold = a;}
	
	/* Vertex setter */
	void SetVertex(Double_t x, Double_t y, Double_t z) {fVertexX=x;fVertexY=y;fVertexZ=z;}
	
	/* Node management for neural network creation */
	Bool_t      GetGlobalXYZ(Int_t refClust, 
	                         Double_t &x, Double_t &y, Double_t &z,
									 Double_t &ex2, Double_t &ey2, Double_t &ez2);
	IlcTARGETnode* AddNode(Int_t index);
	void        CreateArrayStructure(Int_t nsecs);
	Int_t       ArrangePoints(char *exportFile = 0);
	void        StoreOverallMatches();
	Bool_t      PassCurvCut(IlcTARGETnode *p1, IlcTARGETnode *p2, Int_t curvStep, Double_t vx, Double_t vy, Double_t vz);
	Int_t       PassAllCuts(IlcTARGETnode *p1, IlcTARGETnode *p2, Int_t curvStep, Double_t vx, Double_t vy, Double_t vz);
	void        PrintMatches(Bool_t stop = kTRUE);
	
	/* Neural tracking operations */
	// void     NeuralTracking(const char* filesave, TCanvas*& display);
	void     ResetNodes(Int_t isector);
	Int_t    CreateNetwork(Int_t sector, Int_t curvStep);  // create network
	Int_t    CreateNeurons(IlcTARGETnode *node, Int_t curvStep, Double_t vx, Double_t vy, Double_t vz);  // create neurons starting from given point
	Int_t    LinkNeurons();                // create neural connections
	Bool_t   Update();                     // an updating cycle
	void     FollowChains(Int_t sector);   // follows the chains of active neurons
	Int_t    SaveTracks(Int_t sector);     // saves the found tracks
	void     CleanNetwork();               // removes deactivated units and resolve competitions
	//Int_t    Save(Int_t sector);     // save found tracks for # sector
	Int_t    StoreTracks();                // save found tracks for # sector
	
	/* Fit procedures */
	void     ExportTracks(const char *file) const;
	
private:

	/* Neural synaptic weight */
	Double_t Weight(IlcTARGETneuron *nAB, IlcTARGETneuron *nBC); 

	/* Usefuls constant angle values */
	static const Double_t fgkPi; // pi
	static const Double_t fgkHalfPi; // pi / 2
	static const Double_t fgkTwoPi; // 2 * pi
	
	/* Primary vertex position */
	Double_t fVertexX; // X
	Double_t fVertexY; // Y  
	Double_t fVertexZ; // Z
	
	/* TARGET barrel sectioning */
	Int_t        fSectorNum;      //  number of azimutal sectors
	Double_t     fSectorWidth;    //  width of barrel sector (in RAD) [used internally]
	Double_t     fPolarInterval;  //  width of a polar sector (in DEGREES)

	/* Cuts */
	Double_t     fThetaCut2D[5];     //  upper edge of theta cut range (in DEGREES)
	Double_t     fThetaCut3D[5];     //  upper edge of theta cut range (in DEGREES)
	Double_t     fHelixMatchCut[5];  //  lower edge of helix matching cut range
	Int_t        fCurvNum;           //  # of curvature cut steps
	Double_t    *fCurvCut;           //! value of all curvature cuts
	
	/* Neural network work-flow parameters */
	Double_t     fActMinimum;      //  minimum activation to turn 'on' the unit at the end
	Double_t     fEdge1, fEdge2;   //  initialization interval for activations
	Double_t     fStabThreshold;   //  stability threshold
	Double_t     fTemperature;     //  slope in logistic function
	Double_t     fGain2CostRatio;  //  ratio between inhibitory and excitory contributions
	Double_t     fExponent;        //  alignment-dependent weight term
	
	/* Object arrays for data storing and related counters */
	Int_t        fNLayers;          //  Number of TARGET layers
	Int_t       *fFirstModInLayer;  //! Index of first module index for each layer
	TArrayI    **fIndexMap;         //! Map of cluster indexes (in IlcTARGETlayer) with reference to
	                                //  the location in the cluster Tree (module, pos in array)
	Int_t        fTrackClust[6];    //  Track point in layers
	TObjArray   *fFoundTracks;      //! Found tracks
	TObjArray ***fNodes;            //! recpoints arranged into sectors for processing
	TObjArray   *fNeurons;          //! neurons
	
	/* Flags */ 
	Int_t   fMsgLevel;        // To allow the on-screen printing of messages
	Bool_t  fStructureOK;     // Check if the nested TObjArray structure of nodes is created
	
	/* ILC related objects */
	IlcTARGETgeom  *fGeom;       //! TARGET Geometry
	
	/* ROOT class implementation routines */
	ClassDef(IlcTARGETtrackerANN, 1)
};

#endif
