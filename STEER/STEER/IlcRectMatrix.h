#ifndef ILCRECTMATRIX_H
#define ILCRECTMATRIX_H

/*********************************************************************************/
/*                                                                               */ 
/* Class for rectangular matrix used for                                         */
/* millepede2 operation.                                                         */
/* Author: ruben.shahoyan@cern.ch                                                */
/*                                                                               */ 
/*********************************************************************************/

#include "TObject.h"
class TString;

class IlcRectMatrix : public TObject {
  //
 public:
  IlcRectMatrix();
  IlcRectMatrix(Int_t nrow,Int_t ncol);
  IlcRectMatrix(const IlcRectMatrix &src);
  virtual ~IlcRectMatrix();
  //
  Int_t         GetNRows()                            const {return fNRows;}
  Int_t         GetNCols()                            const {return fNCols;}
  //
  Double_t      Query(Int_t rown, Int_t coln)         const {return operator()(rown,coln);}

  IlcRectMatrix& operator=(const IlcRectMatrix& src);
  Double_t      operator()(Int_t rown, Int_t coln)    const;
  Double_t&     operator()(Int_t rown, Int_t coln);
  Double_t*     operator()(Int_t row)                 const {return GetRow(row);}
  Double_t*     GetRow(Int_t row)                     const {return fRows[row];}
  //
  void          Reset() const;
  //
  virtual void  Print(Option_t* option="")           const;
  //
 protected:
  //
  Int_t   fNRows;       // Number of rows
  Int_t   fNCols;       // Number of columns
  Double_t **fRows;     // pointers on rows
  //
  ClassDef(IlcRectMatrix,0) //Rectangular Matrix Class
};

//___________________________________________________________
inline Double_t IlcRectMatrix::operator()(Int_t row, Int_t col) const
{
  return (const Double_t&) GetRow(row)[col];
}

//___________________________________________________________
inline Double_t& IlcRectMatrix::operator()(Int_t row, Int_t col)
{
  return (Double_t&) fRows[row][col];
}


#endif
