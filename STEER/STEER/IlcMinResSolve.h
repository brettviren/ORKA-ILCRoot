#ifndef ILCMINRESSOLVE_H
#define ILCMINRESSOLVE_H

/**********************************************************************************************/
/* General class for solving large system of linear equations                                 */
/* Includes MINRES, FGMRES methods as well as a few precondiotiong methods                    */
/*                                                                                            */ 
/* Author: ruben.shahoyan@cern.ch                                                             */
/*                                                                                            */ 
/**********************************************************************************************/

#include <TObject.h>
#include <TVectorD.h>
class IlcMatrixSq;
class IlcMatrixSparse;
class IlcSymBDMatrix;


class IlcMinResSolve : public TObject {
  //
 public:
  enum {kPreconBD=1,kPreconILU0=100,kPreconILU10=kPreconILU0+10,kPreconsTot};
  enum {kSolMinRes,kSolFGMRes,kNSolvers};
 public:
  IlcMinResSolve();
  IlcMinResSolve(const IlcMatrixSq *mat, const TVectorD* rhs);
  IlcMinResSolve(const IlcMatrixSq *mat, const double  * rhs);
  IlcMinResSolve(const IlcMinResSolve& src);
  ~IlcMinResSolve();
  IlcMinResSolve& operator=(const IlcMinResSolve& rhs);
  //
  // ---------  MINRES method (for symmetric matrices)
  Bool_t SolveMinRes(Double_t* VecSol,Int_t precon=0,int itnlim=2000,double rtol=1e-12);
  Bool_t SolveMinRes(TVectorD &VecSol,Int_t precon=0,int itnlim=2000,double rtol=1e-12);
  //
  // ---------  FGMRES method (for general symmetric matrices)
  Bool_t SolveFGMRES(Double_t* VecSol,Int_t precon=0,int itnlim=2000,double rtol=1e-12, int nkrylov=60);
  Bool_t SolveFGMRES(TVectorD &VecSol,Int_t precon=0,int itnlim=2000,double rtol=1e-12, int nkrylov=60);  
  //
  Bool_t InitAuxMinRes();
  Bool_t InitAuxFGMRES(int nkrylov);
  void   ApplyPrecon(const TVectorD& vecRHS, TVectorD& vecOut)     const;
  void   ApplyPrecon(const double*   vecRHS, double*   vecOut)     const;
  //
  Int_t  BuildPrecon(Int_t val=0);
  Int_t  GetPrecon()                                               const    {return fPrecon;} 
  void   ClearAux();
  //
  Int_t  BuildPreconBD(Int_t hwidth);
  Int_t  BuildPreconILUK(Int_t lofM);
  Int_t  BuildPreconILUKDense(Int_t lofM);
  Int_t  PreconILUKsymb(Int_t lofM);
  Int_t  PreconILUKsymbDense(Int_t lofM);
  //
 protected:
  //
  Int_t               fSize;                             // dimension of the input matrix
  Int_t               fPrecon;                           // preconditioner type
  IlcMatrixSq*        fMatrix;                           // matrix defining the equations
  Double_t*           fRHS;                              // right hand side
  //
  Double_t            *fPVecY;                           // aux. space
  Double_t            *fPVecR1;                          // aux. space
  Double_t            *fPVecR2;                          // aux. space
  Double_t            *fPVecV;                           // aux. space
  Double_t            *fPVecW;                           // aux. space
  Double_t            *fPVecW1;                          // aux. space
  Double_t            *fPVecW2;                          // aux. space
  Double_t            **fPvv;                            // aux. space
  Double_t            **fPvz;                            // aux. space
  Double_t            **fPhh;                            // aux. space
  Double_t            *fDiagLU;                          // aux space
  IlcMatrixSparse     *fMatL;                            // aux. space
  IlcMatrixSparse     *fMatU;                            // aux. space
  IlcSymBDMatrix      *fMatBD;                           // aux. space
  //
  ClassDef(IlcMinResSolve,0)
};

#endif
