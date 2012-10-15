#ifndef ILCMATRIXSPARSE_H
#define ILCMATRIXSPARSE_H

#include "IlcMatrixSq.h"
#include "IlcVectorSparse.h"


/**********************************************************************************************/
/* Sparse matrix class, used as a global matrix for IlcMillePede2                             */
/*                                                                                            */ 
/* Author: ruben.shahoyan@cern.ch                                                             */
/*                                                                                            */ 
/**********************************************************************************************/


//
class IlcMatrixSparse : public IlcMatrixSq 
{
 public:
  IlcMatrixSparse() : fVecs(0) {}
  IlcMatrixSparse(Int_t size);
  IlcMatrixSparse(const IlcMatrixSparse &mat);
  virtual ~IlcMatrixSparse() {Clear();}
  //
  IlcVectorSparse* GetRow(Int_t ir)        const {return (ir<fNcols) ? fVecs[ir] : 0;}
  IlcVectorSparse* GetRowAdd(Int_t ir);
  //
  virtual Int_t   GetSize()                            const {return fNrows;}
  virtual Int_t   GetNRows()                           const {return fNrows;}
  virtual Int_t   GetNCols()                           const {return fNcols;}
  //
  void Clear(Option_t* option="");
  void Reset()                            {for (int i=fNcols;i--;) GetRow(i)->Reset();}
  void Print(Option_t* option="")                      const;
  IlcMatrixSparse& operator=(const IlcMatrixSparse& src);
  Double_t&        operator()(Int_t row,Int_t col);
  Double_t         operator()(Int_t row,Int_t col)     const;
  void             SetToZero(Int_t row,Int_t col);
  Float_t          GetDensity()                        const;
  //
  Double_t         DiagElem(Int_t r)                   const;
  Double_t&        DiagElem(Int_t r);
  void             SortIndices(Bool_t valuesToo=kFALSE);
  //
  void MultiplyByVec(const TVectorD &vecIn, TVectorD &vecOut) const; 
  void MultiplyByVec(const Double_t* vecIn, Double_t* vecOut) const;
  //
  void AddToRow(Int_t r, Double_t *valc,Int_t *indc,Int_t n);
  //
 protected:
  //
  IlcVectorSparse** fVecs;
  //
  ClassDef(IlcMatrixSparse,0)
};

//___________________________________________________
inline void IlcMatrixSparse::MultiplyByVec(const TVectorD &vecIn, TVectorD &vecOut) const 
{
  // multiplication
  MultiplyByVec((Double_t*)vecIn.GetMatrixArray(),(Double_t*)vecOut.GetMatrixArray());
}

//___________________________________________________
inline void IlcMatrixSparse::SetToZero(Int_t row,Int_t col)
{
  //  set existing element to 0
  if (IsSymmetric() && col>row) Swap(row,col); 
  IlcVectorSparse* rowv = GetRow(row);
  if (rowv) rowv->SetToZero(col);
}

//___________________________________________________
inline Double_t IlcMatrixSparse::operator()(Int_t row,Int_t col) const
{
  //  printf("M: find\n");
  if (IsSymmetric() && col>row) Swap(row,col); 
  IlcVectorSparse* rowv = GetRow(row);
  if (!rowv) return 0;
  return rowv->FindIndex(col);
}

//___________________________________________________
inline Double_t& IlcMatrixSparse::operator()(Int_t row,Int_t col)
{
  //  printf("M: findindexAdd\n");
  if (IsSymmetric() && col>row) Swap(row,col); 
  IlcVectorSparse* rowv = GetRowAdd(row);
  if (col>=fNcols) fNcols = col+1;
  return rowv->FindIndexAdd(col);
}

//___________________________________________________
inline Double_t IlcMatrixSparse::DiagElem(Int_t row) const
{
  // get diag elem
  IlcVectorSparse* rowv = GetRow(row);
  if (!rowv) return 0;
  if (IsSymmetric()) return (rowv->GetNElems()>0 && rowv->GetLastIndex()==row) ? rowv->GetLastElem() : 0.;
  else return rowv->FindIndex(row);
  //
}

//___________________________________________________
inline Double_t &IlcMatrixSparse::DiagElem(Int_t row)
{
  // get diag elem
  IlcVectorSparse* rowv = GetRowAdd(row);
  if (row>=fNcols) fNcols = row+1;
  if (IsSymmetric()) {
    return (rowv->GetNElems()>0 && rowv->GetLastIndex()==row) ? 
		       rowv->GetLastElem() : rowv->FindIndexAdd(row);
  }
  else return rowv->FindIndexAdd(row);
  //
}


#endif

