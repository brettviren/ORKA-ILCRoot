//---------------------------------------------------------------------------------
// The IlcKFParticle class
// .
// @author  S.Gorbunov, I.Kisel
// @version 1.0
// @since   13.05.07
// 
// Class to reconstruct and store the decayed particle parameters.
// The method is described in CBM-SOFT note 2007-003, 
// ``Reconstruction of decayed particles based on the Kalman filter'', 
// http://www.gsi.de/documents/DOC-2007-May-14-1.pdf
//
// This class is ILC interface to general mathematics in IlcKFParticleBase
// 
//  -= Copyright &copy ILC HLT Group =-
//_________________________________________________________________________________

#ifndef ILCKFPARTICLE_H
#define ILCKFPARTICLE_H

#include "IlcKFParticleBase.h"
#include "TMath.h"

class IlcVTrack;
class IlcVVertex;

class IlcKFParticle :public IlcKFParticleBase
{
  
 public:

  //*
  //*  INITIALIZATION
  //*

  //* Set magnetic field for all particles
  
  static void SetField( Double_t Bz );

  //* Constructor (empty)

  IlcKFParticle():IlcKFParticleBase(){ ; }

  //* Destructor (empty)

  ~IlcKFParticle(){ ; }

  //* Construction of mother particle by its 2-3-4 daughters

  IlcKFParticle( const IlcKFParticle &d1, const IlcKFParticle &d2, Bool_t gamma = kFALSE );

  IlcKFParticle( const IlcKFParticle &d1, const IlcKFParticle &d2, 
		 const IlcKFParticle &d3 );

  IlcKFParticle( const IlcKFParticle &d1, const IlcKFParticle &d2, 
		 const IlcKFParticle &d3, const IlcKFParticle &d4 );
 
 //* Initialisation from "cartesian" coordinates ( X Y Z Px Py Pz )
 //* Parameters, covariance matrix, charge and PID hypothesis should be provided 

  void Create( const Double_t Param[], const Double_t Cov[], Int_t Charge, Int_t PID );

 //* Initialisation from ILC track, PID hypothesis shoould be provided 

  IlcKFParticle( const IlcVTrack &track, Int_t PID );

  //* Initialisation from VVertex 

  IlcKFParticle( const IlcVVertex &vertex );

  //* Initialise covariance matrix and set current parameters to 0.0 

  void Initialize();

  //* Set decay vertex parameters for linearisation 

  void SetVtxGuess( Double_t x, Double_t y, Double_t z );

  //*
  //*  ACCESSORS
  //*

  //* Simple accessors 

  Double_t GetX    () const ; //* x of current position
  Double_t GetY    () const ; //* y of current position
  Double_t GetZ    () const ; //* z of current position
  Double_t GetPx   () const ; //* x-compoment of 3-momentum
  Double_t GetPy   () const ; //* y-compoment of 3-momentum
  Double_t GetPz   () const ; //* z-compoment of 3-momentum
  Double_t GetE    () const ; //* energy
  Double_t GetS    () const ; //* decay length / momentum
  Int_t    GetQ    () const ; //* charge
  Double_t GetChi2 () const ; //* chi^2
  Int_t    GetNDF  () const ; //* Number of Degrees of Freedom

  const Double_t& X    () const { return fP[0]; }
  const Double_t& Y    () const { return fP[1]; }
  const Double_t& Z    () const { return fP[2]; }
  const Double_t& Px   () const { return fP[3]; }
  const Double_t& Py   () const { return fP[4]; }
  const Double_t& Pz   () const { return fP[5]; }
  const Double_t& E    () const { return fP[6]; }
  const Double_t& S    () const { return fP[7]; }
  const Int_t   & Q    () const { return fQ;    }
  const Double_t& Chi2 () const { return fChi2; }
  const Int_t   & NDF  () const { return fNDF;  }
  
  Double_t GetParameter ( int i ) const ;
  Double_t GetCovariance( int i ) const ;
  Double_t GetCovariance( int i, int j ) const ;

  //* Accessors with calculations, value returned w/o error flag
  
  Double_t GetP           () const; //* momentum
  Double_t GetPt          () const; //* transverse momentum
  Double_t GetEta         () const; //* pseudorapidity
  Double_t GetPhi         () const; //* phi
  Double_t GetMomentum    () const; //* momentum (same as GetP() )
  Double_t GetMass        () const; //* mass
  Double_t GetDecayLength () const; //* decay length
  Double_t GetDecayLengthXY () const; //* decay length in XY
  Double_t GetLifeTime    () const; //* life time
  Double_t GetR           () const; //* distance to the origin

  //* Accessors to estimated errors

  Double_t GetErrX           () const ; //* x of current position 
  Double_t GetErrY           () const ; //* y of current position
  Double_t GetErrZ           () const ; //* z of current position
  Double_t GetErrPx          () const ; //* x-compoment of 3-momentum
  Double_t GetErrPy          () const ; //* y-compoment of 3-momentum
  Double_t GetErrPz          () const ; //* z-compoment of 3-momentum
  Double_t GetErrE           () const ; //* energy
  Double_t GetErrS           () const ; //* decay length / momentum
  Double_t GetErrP           () const ; //* momentum
  Double_t GetErrPt          () const ; //* transverse momentum
  Double_t GetErrEta         () const ; //* pseudorapidity
  Double_t GetErrPhi         () const ; //* phi
  Double_t GetErrMomentum    () const ; //* momentum
  Double_t GetErrMass        () const ; //* mass
  Double_t GetErrDecayLength () const ; //* decay length
  Double_t GetErrDecayLengthXY () const ; //* decay length in XY
  Double_t GetErrLifeTime    () const ; //* life time
  Double_t GetErrR           () const ; //* distance to the origin

  //* Accessors with calculations( &value, &estimated sigma )
  //* error flag returned (0 means no error during calculations) 

  int GetP           ( Double_t &P, Double_t &SigmaP ) const ;   //* momentum
  int GetPt          ( Double_t &Pt, Double_t &SigmaPt ) const ; //* transverse momentum
  int GetEta         ( Double_t &Eta, Double_t &SigmaEta ) const ; //* pseudorapidity
  int GetPhi         ( Double_t &Phi, Double_t &SigmaPhi ) const ; //* phi
  int GetMomentum    ( Double_t &P, Double_t &SigmaP ) const ;   //* momentum
  int GetMass        ( Double_t &M, Double_t &SigmaM ) const ;   //* mass
  int GetDecayLength ( Double_t &L, Double_t &SigmaL ) const ;   //* decay length
  int GetDecayLengthXY ( Double_t &L, Double_t &SigmaL ) const ;   //* decay length in XY
   int GetLifeTime    ( Double_t &T, Double_t &SigmaT ) const ;   //* life time
  int GetR           ( Double_t &R, Double_t &SigmaR ) const ; //* R


  //*
  //*  MODIFIERS
  //*
  
  Double_t & X    () ;
  Double_t & Y    () ;
  Double_t & Z    () ;
  Double_t & Px   () ;
  Double_t & Py   () ;
  Double_t & Pz   () ;
  Double_t & E    () ;
  Double_t & S    () ;
  Int_t    & Q    () ;
  Double_t & Chi2 () ;
  Int_t    & NDF  () ;

  Double_t & Parameter ( int i ) ;
  Double_t & Covariance( int i ) ;
  Double_t & Covariance( int i, int j ) ;
  Double_t * Parameters () ;
  Double_t * CovarianceMatrix() ;

  //* 
  //* CONSTRUCTION OF THE PARTICLE BY ITS DAUGHTERS AND MOTHER
  //* USING THE KALMAN FILTER METHOD
  //*


  //* Add daughter to the particle 

  void AddDaughter( const IlcKFParticle &Daughter );

  //* Add daughter via += operator: ex.{ D0; D0+=Pion; D0+= Kaon; }

  void operator +=( const IlcKFParticle &Daughter );  

  //* Set production vertex 

  void SetProductionVertex( const IlcKFParticle &Vtx );

  //* Set mass constraint 

  void SetMassConstraint( Double_t Mass, Double_t SigmaMass = 0  );
  
  //* Set no decay length for resonances

  void SetNoDecayLength();

  //* Everything in one go  

  void Construct( const IlcKFParticle *vDaughters[], int NDaughters, 
		  const IlcKFParticle *ProdVtx=0,   Double_t Mass=-1, Bool_t IsConstrained=0  );

  //*
  //*                   TRANSPORT
  //* 
  //*  ( main transportation parameter is S = SignedPath/Momentum )
  //*  ( parameters of decay & production vertices are stored locally )
  //*

  //* Transport the particle to its decay vertex 

  void TransportToDecayVertex();

  //* Transport the particle to its production vertex 

  void TransportToProductionVertex();

  //* Transport the particle close to xyz[] point 

  void TransportToPoint( const Double_t xyz[] );

  //* Transport the particle close to VVertex  

  void TransportToVertex( const IlcVVertex &v );

  //* Transport the particle close to another particle p 

  void TransportToParticle( const IlcKFParticle &p );

  //* Transport the particle on dS parameter (SignedPath/Momentum) 

  void TransportToDS( Double_t dS );

  //* Get dS to a certain space point 

  Double_t GetDStoPoint( const Double_t xyz[] ) const ;
  
  //* Get dS to other particle p (dSp for particle p also returned) 

  void GetDStoParticle( const IlcKFParticle &p, 
			Double_t &DS, Double_t &DSp ) const ;
  
  //* Get dS to other particle p in XY-plane

  void GetDStoParticleXY( const IlcKFParticleBase &p, 
			  Double_t &DS, Double_t &DSp ) const ;
  
  //* 
  //* OTHER UTILITIES
  //*


  //* Calculate distance from another object [cm]

  Double_t GetDistanceFromVertex( const Double_t vtx[] ) const ;
  Double_t GetDistanceFromVertex( const IlcKFParticle &Vtx ) const ;
  Double_t GetDistanceFromVertex( const IlcVVertex &Vtx ) const ;
  Double_t GetDistanceFromParticle( const IlcKFParticle &p ) const ;

  //* Calculate sqrt(Chi2/ndf) deviation from another object
  //* ( v = [xyz]-vertex, Cv=[Cxx,Cxy,Cyy,Cxz,Cyz,Czz]-covariance matrix )

  Double_t GetDeviationFromVertex( const Double_t v[], const Double_t Cv[]=0 ) const ;
  Double_t GetDeviationFromVertex( const IlcKFParticle &Vtx ) const ;
  Double_t GetDeviationFromVertex( const IlcVVertex &Vtx ) const ;
  Double_t GetDeviationFromParticle( const IlcKFParticle &p ) const ;
 
  //* Calculate distance from another object [cm] in XY-plane

  Bool_t GetDistanceFromVertexXY( const Double_t vtx[], Double_t &val, Double_t &err ) const ;
  Bool_t GetDistanceFromVertexXY( const Double_t vtx[], const Double_t Cv[], Double_t &val, Double_t &err ) const ;
  Bool_t GetDistanceFromVertexXY( const IlcKFParticle &Vtx, Double_t &val, Double_t &err ) const ;
  Bool_t GetDistanceFromVertexXY( const IlcVVertex &Vtx, Double_t &val, Double_t &err ) const ;

  Double_t GetDistanceFromVertexXY( const Double_t vtx[] ) const ;
  Double_t GetDistanceFromVertexXY( const IlcKFParticle &Vtx ) const ;
  Double_t GetDistanceFromVertexXY( const IlcVVertex &Vtx ) const ;
  Double_t GetDistanceFromParticleXY( const IlcKFParticle &p ) const ;

  //* Calculate sqrt(Chi2/ndf) deviation from another object in XY plane
  //* ( v = [xyz]-vertex, Cv=[Cxx,Cxy,Cyy,Cxz,Cyz,Czz]-covariance matrix )

  Double_t GetDeviationFromVertexXY( const Double_t v[], const Double_t Cv[]=0 ) const ;
  Double_t GetDeviationFromVertexXY( const IlcKFParticle &Vtx ) const ;
  Double_t GetDeviationFromVertexXY( const IlcVVertex &Vtx ) const ;
  Double_t GetDeviationFromParticleXY( const IlcKFParticle &p ) const ;

  //* Calculate opennig angle between two particles

  Double_t GetAngle  ( const IlcKFParticle &p ) const ;
  Double_t GetAngleXY( const IlcKFParticle &p ) const ;
  Double_t GetAngleRZ( const IlcKFParticle &p ) const ;

  //* Subtract the particle from the vertex  

  void SubtractFromVertex( IlcKFParticle &v ) const ;

  //* Special method for creating gammas

  void ConstructGamma( const IlcKFParticle &daughter1,
		       const IlcKFParticle &daughter2  );
  
    // * Pseudo Proper Time of decay = (r*pt) / |pt| * M/|pt|
    // @primVertex - primary vertex
    // @mass - mass of the mother particle (in the case of "Hb -> JPsi" it would be JPsi mass)
    // @*timeErr2 - squared error of the decay time. If timeErr2 = 0 it isn't calculated
  Double_t GetPseudoProperDecayTime( const IlcKFParticle &primVertex, const Double_t& mass, Double_t* timeErr2 = 0 ) const;
  
 protected: 
  
  //*
  //*  INTERNAL STUFF
  //* 

  //* Method to access ILC field 
 
  static Double_t GetFieldIlc();
  
  //* Other methods required by the abstract IlcKFParticleBase class 
  
  void GetFieldValue( const Double_t xyz[], Double_t B[] ) const ;
  void GetDStoParticle( const IlcKFParticleBase &p, Double_t &DS, Double_t &DSp )const ;
  void Transport( Double_t dS, Double_t P[], Double_t C[] ) const ;
  static void GetExternalTrackParam( const IlcKFParticleBase &p, Double_t &X, Double_t &Alpha, Double_t P[5]  ) ;

  //void GetDStoParticleILC( const IlcKFParticleBase &p, Double_t &DS, Double_t &DS1 ) const;


 private:

  static Double_t fgBz;  //* Bz compoment of the magnetic field

  ClassDef( IlcKFParticle, 1 );

};



//---------------------------------------------------------------------
//
//     Inline implementation of the IlcKFParticle methods
//
//---------------------------------------------------------------------


#ifdef WIN32
#else
inline void IlcKFParticle::SetField( Double_t Bz )
{ 
  fgBz = Bz;
}
#endif


inline IlcKFParticle::IlcKFParticle( const IlcKFParticle &d1, 
				     const IlcKFParticle &d2, 
				     const IlcKFParticle &d3 )
{
  IlcKFParticle mother;
  mother+= d1;
  mother+= d2;
  mother+= d3;
  *this = mother;
}

inline IlcKFParticle::IlcKFParticle( const IlcKFParticle &d1, 
				     const IlcKFParticle &d2, 
				     const IlcKFParticle &d3, 
				     const IlcKFParticle &d4 )
{
  IlcKFParticle mother;
  mother+= d1;
  mother+= d2;
  mother+= d3;
  mother+= d4;
  *this = mother;
}


inline void IlcKFParticle::Initialize()
{ 
  IlcKFParticleBase::Initialize(); 
}

inline void IlcKFParticle::SetVtxGuess( Double_t x, Double_t y, Double_t z )
{
  IlcKFParticleBase::SetVtxGuess(x,y,z);
}  

inline Double_t IlcKFParticle::GetX    () const 
{ 
  return IlcKFParticleBase::GetX();    
}

inline Double_t IlcKFParticle::GetY    () const 
{ 
  return IlcKFParticleBase::GetY();    
}

inline Double_t IlcKFParticle::GetZ    () const 
{ 
  return IlcKFParticleBase::GetZ();    
}

inline Double_t IlcKFParticle::GetPx   () const 
{ 
  return IlcKFParticleBase::GetPx();   
}

inline Double_t IlcKFParticle::GetPy   () const 
{ 
  return IlcKFParticleBase::GetPy();   
}

inline Double_t IlcKFParticle::GetPz   () const 
{ 
  return IlcKFParticleBase::GetPz();   
}

inline Double_t IlcKFParticle::GetE    () const 
{ 
  return IlcKFParticleBase::GetE();    
}

inline Double_t IlcKFParticle::GetS    () const 
{ 
  return IlcKFParticleBase::GetS();    
}

inline Int_t    IlcKFParticle::GetQ    () const 
{ 
  return IlcKFParticleBase::GetQ();    
}

inline Double_t IlcKFParticle::GetChi2 () const 
{ 
  return IlcKFParticleBase::GetChi2(); 
}

inline Int_t    IlcKFParticle::GetNDF  () const 
{ 
  return IlcKFParticleBase::GetNDF();  
}

inline Double_t IlcKFParticle::GetParameter ( int i ) const 
{ 
  return IlcKFParticleBase::GetParameter(i);  
}

inline Double_t IlcKFParticle::GetCovariance( int i ) const 
{ 
  return IlcKFParticleBase::GetCovariance(i); 
}

inline Double_t IlcKFParticle::GetCovariance( int i, int j ) const 
{ 
  return IlcKFParticleBase::GetCovariance(i,j);
}


inline Double_t IlcKFParticle::GetP    () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetMomentum( par, err ) ) return 0;
  else return par;
}

inline Double_t IlcKFParticle::GetPt   () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetPt( par, err ) ) return 0;
  else return par;
}

inline Double_t IlcKFParticle::GetEta   () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetEta( par, err ) ) return 0;
  else return par;
}

inline Double_t IlcKFParticle::GetPhi   () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetPhi( par, err ) ) return 0;
  else return par;
}

inline Double_t IlcKFParticle::GetMomentum    () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetMomentum( par, err ) ) return 0;
  else return par;
}

inline Double_t IlcKFParticle::GetMass        () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetMass( par, err ) ) return 0;
  else return par;
}

inline Double_t IlcKFParticle::GetDecayLength () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetDecayLength( par, err ) ) return 0;
  else return par;
}

inline Double_t IlcKFParticle::GetDecayLengthXY () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetDecayLengthXY( par, err ) ) return 0;
  else return par;
}

inline Double_t IlcKFParticle::GetLifeTime    () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetLifeTime( par, err ) ) return 0;
  else return par;
}

inline Double_t IlcKFParticle::GetR   () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetR( par, err ) ) return 0;
  else return par;
}

inline Double_t IlcKFParticle::GetErrX           () const 
{
  return TMath::Sqrt(TMath::Abs( GetCovariance(0,0) ));
}

inline Double_t IlcKFParticle::GetErrY           () const 
{
  return TMath::Sqrt(TMath::Abs( GetCovariance(1,1) ));
}

inline Double_t IlcKFParticle::GetErrZ           () const 
{
  return TMath::Sqrt(TMath::Abs( GetCovariance(2,2) ));
}

inline Double_t IlcKFParticle::GetErrPx          () const 
{
  return TMath::Sqrt(TMath::Abs( GetCovariance(3,3) ));
}

inline Double_t IlcKFParticle::GetErrPy          () const 
{
  return TMath::Sqrt(TMath::Abs( GetCovariance(4,4) ));
}

inline Double_t IlcKFParticle::GetErrPz          () const 
{
  return TMath::Sqrt(TMath::Abs( GetCovariance(5,5) ));
}

inline Double_t IlcKFParticle::GetErrE           () const 
{
  return TMath::Sqrt(TMath::Abs( GetCovariance(6,6) ));
}

inline Double_t IlcKFParticle::GetErrS           () const 
{
  return TMath::Sqrt(TMath::Abs( GetCovariance(7,7) ));
}

inline Double_t IlcKFParticle::GetErrP    () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetMomentum( par, err ) ) return 1.e10;
  else return err;
}

inline Double_t IlcKFParticle::GetErrPt    () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetPt( par, err ) ) return 1.e10;
  else return err;
}

inline Double_t IlcKFParticle::GetErrEta    () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetEta( par, err ) ) return 1.e10;
  else return err;
}

inline Double_t IlcKFParticle::GetErrPhi    () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetPhi( par, err ) ) return 1.e10;
  else return err;
}

inline Double_t IlcKFParticle::GetErrMomentum    () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetMomentum( par, err ) ) return 1.e10;
  else return err;
}

inline Double_t IlcKFParticle::GetErrMass        () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetMass( par, err ) ) return 1.e10;
  else return err;
}

inline Double_t IlcKFParticle::GetErrDecayLength () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetDecayLength( par, err ) ) return 1.e10;
  else return err;
}

inline Double_t IlcKFParticle::GetErrDecayLengthXY () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetDecayLengthXY( par, err ) ) return 1.e10;
  else return err;
}

inline Double_t IlcKFParticle::GetErrLifeTime    () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetLifeTime( par, err ) ) return 1.e10;
  else return err;
}

inline Double_t IlcKFParticle::GetErrR    () const
{
  Double_t par, err;
  if( IlcKFParticleBase::GetR( par, err ) ) return 1.e10;
  else return err;
}


inline int IlcKFParticle::GetP( Double_t &P, Double_t &SigmaP ) const 
{
  return IlcKFParticleBase::GetMomentum( P, SigmaP );
}

inline int IlcKFParticle::GetPt( Double_t &Pt, Double_t &SigmaPt ) const 
{
  return IlcKFParticleBase::GetPt( Pt, SigmaPt );
}

inline int IlcKFParticle::GetEta( Double_t &Eta, Double_t &SigmaEta ) const 
{
  return IlcKFParticleBase::GetEta( Eta, SigmaEta );
}

inline int IlcKFParticle::GetPhi( Double_t &Phi, Double_t &SigmaPhi ) const 
{
  return IlcKFParticleBase::GetPhi( Phi, SigmaPhi );
}

inline int IlcKFParticle::GetMomentum( Double_t &P, Double_t &SigmaP ) const 
{
  return IlcKFParticleBase::GetMomentum( P, SigmaP );
}

inline int IlcKFParticle::GetMass( Double_t &M, Double_t &SigmaM ) const 
{
  return IlcKFParticleBase::GetMass( M, SigmaM );
}

inline int IlcKFParticle::GetDecayLength( Double_t &L, Double_t &SigmaL ) const 
{
  return IlcKFParticleBase::GetDecayLength( L, SigmaL );
}

inline int IlcKFParticle::GetDecayLengthXY( Double_t &L, Double_t &SigmaL ) const 
{
  return IlcKFParticleBase::GetDecayLengthXY( L, SigmaL );
}

inline int IlcKFParticle::GetLifeTime( Double_t &T, Double_t &SigmaT ) const 
{
  return IlcKFParticleBase::GetLifeTime( T, SigmaT );
}

inline int IlcKFParticle::GetR( Double_t &R, Double_t &SigmaR ) const 
{
  return IlcKFParticleBase::GetR( R, SigmaR );
}

inline Double_t & IlcKFParticle::X() 
{ 
  return IlcKFParticleBase::X();    
}

inline Double_t & IlcKFParticle::Y()
{ 
  return IlcKFParticleBase::Y();    
}

inline Double_t & IlcKFParticle::Z() 
{ 
  return IlcKFParticleBase::Z();    
}

inline Double_t & IlcKFParticle::Px() 
{ 
  return IlcKFParticleBase::Px();   
}

inline Double_t & IlcKFParticle::Py() 
{ 
  return IlcKFParticleBase::Py();   
}

inline Double_t & IlcKFParticle::Pz() 
{ 
  return IlcKFParticleBase::Pz();   
}

inline Double_t & IlcKFParticle::E() 
{ 
  return IlcKFParticleBase::E();    
}

inline Double_t & IlcKFParticle::S() 
{ 
  return IlcKFParticleBase::S();    
}

inline Int_t    & IlcKFParticle::Q() 
{ 
  return IlcKFParticleBase::Q();    
}

inline Double_t & IlcKFParticle::Chi2() 
{ 
  return IlcKFParticleBase::Chi2(); 
}

inline Int_t    & IlcKFParticle::NDF() 
{ 
  return IlcKFParticleBase::NDF();  
}

inline Double_t & IlcKFParticle::Parameter ( int i )        
{ 
  return IlcKFParticleBase::Parameter(i);
}

inline Double_t & IlcKFParticle::Covariance( int i )        
{ 
  return IlcKFParticleBase::Covariance(i);
}

inline Double_t & IlcKFParticle::Covariance( int i, int j ) 
{ 
  return IlcKFParticleBase::Covariance(i,j); 
}

inline Double_t * IlcKFParticle::Parameters ()
{
  return fP;
}

inline Double_t * IlcKFParticle::CovarianceMatrix()
{
  return fC;
}


inline void IlcKFParticle::operator +=( const IlcKFParticle &Daughter )
{
  IlcKFParticleBase::operator +=( Daughter );
}
  

inline void IlcKFParticle::AddDaughter( const IlcKFParticle &Daughter )
{
  IlcKFParticleBase::AddDaughter( Daughter );
}

inline void IlcKFParticle::SetProductionVertex( const IlcKFParticle &Vtx )
{
  IlcKFParticleBase::SetProductionVertex( Vtx );
}

inline void IlcKFParticle::SetMassConstraint( Double_t Mass, Double_t SigmaMass )
{
  IlcKFParticleBase::SetMassConstraint( Mass, SigmaMass );
}
    
inline void IlcKFParticle::SetNoDecayLength()
{
  IlcKFParticleBase::SetNoDecayLength();
}

inline void IlcKFParticle::Construct( const IlcKFParticle *vDaughters[], int NDaughters, 
			       const IlcKFParticle *ProdVtx,   Double_t Mass, Bool_t IsConstrained  )
{    
  IlcKFParticleBase::Construct( ( const IlcKFParticleBase**)vDaughters, NDaughters, 
			 ( const IlcKFParticleBase*)ProdVtx, Mass, IsConstrained );
}

inline void IlcKFParticle::TransportToDecayVertex()
{ 
  IlcKFParticleBase::TransportToDecayVertex(); 
}

inline void IlcKFParticle::TransportToProductionVertex()
{
  IlcKFParticleBase::TransportToProductionVertex();
}

inline void IlcKFParticle::TransportToPoint( const Double_t xyz[] )
{ 
  TransportToDS( GetDStoPoint(xyz) );
}

inline void IlcKFParticle::TransportToVertex( const IlcVVertex &v )
{       
  TransportToPoint( IlcKFParticle(v).fP );
}

inline void IlcKFParticle::TransportToParticle( const IlcKFParticle &p )
{ 
  Double_t dS, dSp;
  GetDStoParticle( p, dS, dSp );
  TransportToDS( dS );
}

inline void IlcKFParticle::TransportToDS( Double_t dS )
{
  IlcKFParticleBase::TransportToDS( dS );
} 

inline Double_t IlcKFParticle::GetDStoPoint( const Double_t xyz[] ) const 
{
  return IlcKFParticleBase::GetDStoPointBz( GetFieldIlc(), xyz );
}

  
inline void IlcKFParticle::GetDStoParticle( const IlcKFParticle &p, 
					    Double_t &DS, Double_t &DSp ) const 
{
  GetDStoParticleXY( p, DS, DSp );
}


inline Double_t IlcKFParticle::GetDistanceFromVertex( const Double_t vtx[] ) const
{
  return IlcKFParticleBase::GetDistanceFromVertex( vtx );
}

inline Double_t IlcKFParticle::GetDeviationFromVertex( const Double_t v[], 
						       const Double_t Cv[] ) const
{
  return IlcKFParticleBase::GetDeviationFromVertex( v, Cv);
}

inline Double_t IlcKFParticle::GetDistanceFromVertex( const IlcKFParticle &Vtx ) const
{
  return IlcKFParticleBase::GetDistanceFromVertex( Vtx );
}

inline Double_t IlcKFParticle::GetDeviationFromVertex( const IlcKFParticle &Vtx ) const
{
  return IlcKFParticleBase::GetDeviationFromVertex( Vtx );
}

inline Double_t IlcKFParticle::GetDistanceFromVertex( const IlcVVertex &Vtx ) const
{
  return GetDistanceFromVertex( IlcKFParticle(Vtx) );
}

inline Double_t IlcKFParticle::GetDeviationFromVertex( const IlcVVertex &Vtx ) const
{
  return GetDeviationFromVertex( IlcKFParticle(Vtx) );
}
 
inline Double_t IlcKFParticle::GetDistanceFromParticle( const IlcKFParticle &p ) const 
{
  return IlcKFParticleBase::GetDistanceFromParticle( p );
}

inline Double_t IlcKFParticle::GetDeviationFromParticle( const IlcKFParticle &p ) const 
{
  return IlcKFParticleBase::GetDeviationFromParticle( p );
}

inline void IlcKFParticle::SubtractFromVertex( IlcKFParticle &v ) const 
{
  IlcKFParticleBase::SubtractFromVertex( v );
}

#ifdef WIN32
#else
inline Double_t IlcKFParticle::GetFieldIlc()
{ 
  return fgBz; 
}
#endif

inline void IlcKFParticle::GetFieldValue( const Double_t * /*xyz*/, Double_t B[] ) const 
{    
  B[0] = B[1] = 0;
  B[2] = GetFieldIlc();
}

inline void IlcKFParticle::GetDStoParticle( const IlcKFParticleBase &p, 
					    Double_t &DS, Double_t &DSp )const
{
  GetDStoParticleXY( p, DS, DSp );
}

inline void IlcKFParticle::GetDStoParticleXY( const IlcKFParticleBase &p, 
				       Double_t &DS, Double_t &DSp ) const
{ 
  IlcKFParticleBase::GetDStoParticleBz( GetFieldIlc(), p, DS, DSp ) ;
  //GetDStoParticleILC( p, DS, DSp ) ;
}

inline void IlcKFParticle::Transport( Double_t dS, Double_t P[], Double_t C[] ) const 
{
  IlcKFParticleBase::TransportBz( GetFieldIlc(), dS, P, C );
}

inline void IlcKFParticle::ConstructGamma( const IlcKFParticle &daughter1,
					   const IlcKFParticle &daughter2  )
{
  IlcKFParticleBase::ConstructGammaBz( daughter1, daughter2, GetFieldIlc() );
}

#endif 
