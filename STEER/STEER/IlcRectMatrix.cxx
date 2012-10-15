/*********************************************************************************/
/*                                                                               */ 
/* Class for rectangular matrix used for                                         */
/* millepede2 operation.                                                         */
/* May be sparse or dense.                                                       */ 
/* -----------------------                                                       */ 
/* Author: ruben.shahoyan@cern.ch                                                */
/*                                                                               */ 
/*********************************************************************************/

#include "IlcRectMatrix.h"
#include <TString.h>
//

ClassImp(IlcRectMatrix)


//___________________________________________________________
IlcRectMatrix::IlcRectMatrix() 
: fNRows(0),fNCols(0),fRows(0)
{}

//___________________________________________________________
IlcRectMatrix::IlcRectMatrix(Int_t nrow,Int_t ncol)
  : fNRows(nrow),fNCols(ncol),fRows(0)
{
  // c-tor
  fRows = new Double_t*[fNRows];
  for (int i=fNRows;i--;) {
    fRows[i] = new Double_t[fNCols];
    memset(fRows[i],0,fNCols*sizeof(Double_t));
  }
  //
}

//___________________________________________________________
IlcRectMatrix::IlcRectMatrix(const IlcRectMatrix &src)
  : TObject(src),fNRows(src.fNRows), fNCols(src.fNCols), fRows(0)
{
  // copy c-tor
  fRows = new Double_t*[fNRows];
  for (int i=fNRows;i--;) {
    fRows[i] = new Double_t[fNCols];
    memcpy(fRows[i], src.fRows[i], fNCols*sizeof(Double_t));
  }
}

//___________________________________________________________
IlcRectMatrix::~IlcRectMatrix()
{
  // dest-tor
  if (fNRows) for (int i=fNRows;i--;) delete[] fRows[i];
  delete[] fRows;
}

//___________________________________________________________
IlcRectMatrix& IlcRectMatrix::operator=(const IlcRectMatrix& src)
{
  // assignment op-r
  if (&src == this) return *this;
  if (fNRows) for (int i=fNRows;i--;) delete[] fRows[i];
  delete[] fRows;
  fNRows = src.fNRows;
  fNCols = src.fNCols;
  fRows = new Double_t*[fNRows];
  for (int i=fNRows;i--;) {
    fRows[i] = new Double_t[fNCols];
    memcpy(fRows[i], src.fRows[i], fNCols*sizeof(Double_t));
  }
  //
  return *this;
}

//___________________________________________________________
void IlcRectMatrix::Print(Option_t* option) const
{
  // print itself
  printf("Rectangular Matrix:  %d rows %d columns\n",fNRows,fNCols);
  TString opt = option; opt.ToLower();
  if (opt.IsNull()) return;
  for (int i=0;i<fNRows;i++) {
    for (Int_t j=0;j<=fNCols;j++) printf("%+.3e|",Query(i,j));
    printf("\n");
  }
}


//___________________________________________________________
void IlcRectMatrix::Reset() const
{
  // reset all
  for (int i=fNRows;i--;) {
    double *row = GetRow(i); 
    for (int j=fNCols;j--;)  row[j] = 0.;
  }
}
