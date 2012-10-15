///////////////////////////////////////////////////////////////////////
//
// IlcTARGETneuralTracker:
//
// neural network MFT algorithm
// for track finding in TARGET stand alone
// according to the Denby-Peterson model with adaptments to the
// ILC multiplicity
//
///////////////////////////////////////////////////////////////////////

#ifndef ILCTARGETNEURALTRACKER_H
#define ILCTARGETNEURALTRACKER_H

#include <TTree.h>

class TObjArray;
class TCanvas;

#include "IlcTARGETNeuralPoint.h"

class IlcTARGETNeuralTracker : public TObject {

public:

	IlcTARGETNeuralTracker();
	IlcTARGETNeuralTracker(const IlcTARGETNeuralTracker &t);
	IlcTARGETNeuralTracker& operator=(const IlcTARGETNeuralTracker&);
	
	virtual ~IlcTARGETNeuralTracker();

	// ******************************************************************************
	// * Embedded utility class --> >>> NODE <<<
	// ******************************************************************************
	// * This class inherits from IlcTARGETNeuralPoint and adds some
	// * utility pointers for quick path-finding among neurons.
	// ******************************************************************************
	class IlcTARGETNode : public IlcTARGETNeuralPoint {
	public:
		IlcTARGETNode() {fInnerOf = fOuterOf = fMatches = 0; fNext = fPrev = 0;}
		IlcTARGETNode(const IlcTARGETNode &t);
		IlcTARGETNode(IlcTARGETNeuralPoint *p, Bool_t init = kTRUE) // declared inline
			: IlcTARGETNeuralPoint(p) { fInnerOf = fOuterOf = fMatches = 0; fNext = fPrev = 0;
			if (init) { fInnerOf = new TObjArray; fOuterOf = new TObjArray; fMatches = new TObjArray;}}
		IlcTARGETNode(IlcTARGETRecPoint *p, IlcTARGETgeomMatrix *gm)
			: IlcTARGETNeuralPoint(p,gm) {fInnerOf = fOuterOf = fMatches = 0; fNext = fPrev = 0;}

		virtual  ~IlcTARGETNode() 
			{fInnerOf = fOuterOf = fMatches = 0; fNext = fPrev = 0;}
			
		IlcTARGETNode& operator=(const IlcTARGETNode&);
		
		Double_t  ThetaDeg()                    {return GetTheta()*180.0/TMath::Pi();}

		Int_t     GetSector(Double_t secwidth) const {return (Int_t)(GetPhi()/secwidth);}
		Int_t     GetThetaCell()                {return (Int_t)(ThetaDeg());}
		
		Int_t&     PosInTree() {return fPosInTree;}
		
		TObjArray*&  InnerOf() {return fInnerOf;}
		TObjArray*&  OuterOf() {return fOuterOf;}
		TObjArray*&  Matches() {return fMatches;}
		
		IlcTARGETNode*& Next() {return fNext;}
		IlcTARGETNode*& Prev() {return fPrev;}
		
	private:
		
		Int_t        fPosInTree;  // position in tree of converted points

		TObjArray   *fInnerOf; //!
		TObjArray   *fOuterOf; //! 
		TObjArray   *fMatches; //!

		IlcTARGETNode  *fNext; //!
		IlcTARGETNode  *fPrev; //!
	};
	// ******************************************************************************



	// ******************************************************************************
	// * Embedded utility class --> >>> NEURON <<<
	// ******************************************************************************
	// * Simple class implementing the neural unit.
	// * Contains the activation and some other pointers
	// * for purposes similar to the ones in IlcTARGETnode.
	// ******************************************************************************
	class IlcTARGETneuron : public TObject {
	public:
		IlcTARGETneuron():fUsed(0),fActivation(0.),fInner(0),fOuter(0),fGain(0) { }
		IlcTARGETneuron(const IlcTARGETneuron &t); 
		virtual    ~IlcTARGETneuron() {fInner=fOuter=0;fGain=0;}
		
		IlcTARGETneuron& operator=(const IlcTARGETneuron&);

		Double_t    Weight(IlcTARGETneuron *n);
		void        Add2Gain(IlcTARGETneuron *n, Double_t multconst, Double_t exponent);
		
		Int_t&       Used() {return fUsed;}
		Double_t&    Activation() {return fActivation;}
		IlcTARGETNode*& Inner() {return fInner;}
		IlcTARGETNode*& Outer() {return fOuter;}
		TObjArray*&  Gain()  {return fGain;}
		
	private:

		Int_t             fUsed;        //  utility flag
		Double_t          fActivation;  //  Activation value
		IlcTARGETNode       *fInner;       //! inner point
		IlcTARGETNode       *fOuter;       //! outer point
		TObjArray        *fGain;        //! list of sequenced units
	};
	// ******************************************************************************



	// ******************************************************************************
	// * Embedded utility class --> >>> CONNECTION <<<
	// ******************************************************************************
	// * Used to implement the neural weighted connection
	// * in such a way to speed up the retrieval of the
	// * links among neuron, for a fast update procedure.
	// ******************************************************************************
	class IlcTARGETlink : public TObject {
	public:
		IlcTARGETlink() : fWeight(0.), fLinked(0) { }
		IlcTARGETlink(const IlcTARGETlink &t);
		virtual ~IlcTARGETlink()   {fLinked = 0;}
		
		IlcTARGETlink& operator=(const IlcTARGETlink&);
		
		Double_t& Weight() {return fWeight;}
		IlcTARGETneuron*& Linked() {return fLinked;}
		
	private:

		Double_t      fWeight;  //  Weight value
		IlcTARGETneuron *fLinked;  //! the connected neuron
	};
	// ******************************************************************************


	// Cut related setters

	void     SetHelixMatchCuts(Double_t *min, Double_t *max);
	void     SetThetaCuts2D(Double_t *min, Double_t *max);
	void     SetThetaCuts3D(Double_t *min, Double_t *max);
	void     SetCurvatureCuts(Int_t n, Double_t *cuts);
	void     SetVertex(Double_t x, Double_t y, Double_t z)  {fVX=x; fVY=y; fVZ=z;}
	void     SetPolarInterval(Double_t dtheta) {fPolarInterval=dtheta;}

	// Neural work-flow related setters

	void     SetActThreshold(Double_t val)            {fActMinimum = val;}
	void     SetWeightExponent(Double_t a)            {fAlignExponent = a;}
	void     SetGainToCostRatio(Double_t a)           {fGain2CostRatio = a;}
	void     SetInitInterval(Double_t a, Double_t b)  {fEdge1 = a; fEdge2 = b;}
	void     SetTemperature(Double_t a)               {fTemperature = a;}
	void     SetVariationLimit(Double_t a)            {fStabThreshold = a;}

	// Points array arrangement & control

	void     CreateArrayStructure(Int_t nsecs);
	Int_t    ArrangePoints(TTree *ptstree);
	void     StoreAbsoluteMatches();
	Bool_t   PassCurvCut(IlcTARGETNode *p1, IlcTARGETNode *p2, Int_t curvidx, Double_t vx, Double_t vy, Double_t vz);
	Int_t    PassAllCuts(IlcTARGETNode *p1, IlcTARGETNode *p2, Int_t curvidx, Double_t vx, Double_t vy, Double_t vz);
	void     PrintPoints();
	void     PrintMatches(Bool_t stop = kTRUE);

	// Neural tracker work-flow

	void     NeuralTracking(const char* filesave, TCanvas*& display);
	void     Display(TCanvas*& canvas) const;
	void     ResetNodes(Int_t isector);
	Int_t    CreateNeurons(Int_t sector, Int_t curv);  // create neurons
	Int_t    LinkNeurons() const;          // create neural connections
	Double_t Activate(IlcTARGETneuron* &n);   // calculates the new neural activation
	Bool_t   Update();                     // an updating cycle
	void     CleanNetwork();               // removes deactivated units and resolves competitions
	Int_t    Save(Int_t sectoridx);        // save found tracks for # sector
	TTree*   GetChains()                   {return fChains;}
	void     WriteChains()                 {fChains->Write();}

private:

	Bool_t       CheckOccupation() const; 

	Int_t        fSectorNum;            //  number of azymuthal sectors
	Double_t     fSectorWidth;          //  width of an azymuthal sector (in RADIANS) [used internally]
	Double_t     fPolarInterval;        //  width of a polar sector (in DEGREES)

	Double_t     fThetaCut2DMin[5];     //  lower edge of theta cut range (in DEGREES)
	Double_t     fThetaCut2DMax[5];     //  upper edge of theta cut range (in DEGREES)
	Double_t     fThetaCut3DMin[5];     //  lower edge of theta cut range (in DEGREES)
	Double_t     fThetaCut3DMax[5];     //  upper edge of theta cut range (in DEGREES)
	Double_t     fHelixMatchCutMin[5];  //  lower edge of helix matching cut range
	Double_t     fHelixMatchCutMax[5];  //  lower edge of helix matching cut range
	Int_t        fCurvNum;              //  # of curvature cut steps
	Double_t    *fCurvCut;              //! value of all curvature cuts

	Bool_t       fStructureOK;          // flag to avoid MANY segfault errors

	Double_t     fVX, fVY, fVZ;         //  estimated vertex coords (for helix matching cut)

	Double_t     fActMinimum;           //  minimum activation to turn 'on' the unit at the end
	Double_t     fEdge1, fEdge2;        //  initialization interval for activations

	Double_t     fTemperature;          //  logistic function temperature parameter
	Double_t     fStabThreshold;        //  stability threshold
	Double_t     fGain2CostRatio;       //  ratio between inhibitory and excitory contributions
	Double_t     fAlignExponent;        //  alignment-dependent weight term

	Int_t        fPoint[6];             //  Track point in layers
	TTree       *fChains;               //! Output tree

	TObjArray   *fPoints[6][180];       //! recpoints arranged into sectors for processing
	TObjArray   *fNeurons;              //! neurons

	ClassDef(IlcTARGETNeuralTracker, 1)
};


////////////////////////////////////////////////////////////////////////////////

#endif
