#ifndef ILCVECTORSPARSE_H
#define ILCVECTORSPARSE_H

#include <TObject.h>
#include <TMath.h>

/**********************************************************************************************/
/* Sparse vector class, used as row of the IlcMatrixSparse class                              */
/*                                                                                            */ 
/* Author: ruben.shahoyan@cern.ch                                                             */
/*                                                                                            */ 
/**********************************************************************************************/

///////////////////////////////////////////////////////////////////////////////////////
class IlcVectorSparse : public TObject {
 public:
  IlcVectorSparse();
  IlcVectorSparse(const IlcVectorSparse& src);
  virtual ~IlcVectorSparse() {Clear();}
  virtual void Print(Option_t* option="")                 const;
  //
  Int_t     GetNElems()                                   const {return fNElems;}
  UShort_t *GetIndices()                                  const {return fIndex;}
  Double_t *GetElems()                                    const {return fElems;}
  UShort_t& GetIndex(Int_t i)                                   {return fIndex[i];}
  Double_t& GetElem(Int_t i)                              const {return fElems[i];}
  void      Clear(Option_t* option="");
  void      Reset()                                             {memset(fElems,0,fNElems*sizeof(Double_t));}
  void      ReSize(Int_t sz,Bool_t copy=kFALSE);
  void      SortIndices(Bool_t valuesToo=kFALSE);
  void      Add(Double_t *valc,Int_t *indc,Int_t n);
  //
  IlcVectorSparse& operator=(const IlcVectorSparse& src);
  //
  virtual Double_t         operator()(Int_t ind)         const;
  virtual Double_t&        operator()(Int_t ind);
  virtual void             SetToZero(Int_t ind);
  Double_t                 FindIndex(Int_t ind)          const;
  Double_t&                FindIndexAdd(Int_t ind);
  //
  Int_t     GetLastIndex()                               const {return fIndex[fNElems-1];}
  Double_t  GetLastElem()                                const {return fElems[fNElems-1];}
  Double_t &GetLastElem()                                      {return fElems[fNElems-1];}
  //
 protected:
  Int_t            fNElems;   // Number of elements
  UShort_t*        fIndex;    // Index of stored elems
  Double_t*        fElems;    // pointer on elements
  //
  ClassDef(IlcVectorSparse,0)
};


//___________________________________________________
inline Double_t IlcVectorSparse::operator()(Int_t ind) const
{
  return FindIndex(ind);
}

//___________________________________________________
inline Double_t& IlcVectorSparse::operator()(Int_t ind)
{
  return FindIndexAdd(ind);
}

//////////////////////////////////////////////////////////////////////////////////////

#endif
