#ifndef ILCDCHPADPLANE_H
#define ILCDCHPADPLANE_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice                               */

/* $Id: IlcDCHpadPlane.h,v */

//////////////////////////////////////////////////
//                                              //
//  DCH pad plane class                         //
//                                              //
//  Contains the information on pad postions,   //
//  pad dimensions, tilting angle, etc.         //
//  It also provides methods to identify the    //
//  current pad number from global coordinates. //
//                                              //
//////////////////////////////////////////////////

#include <TObject.h>

class IlcDCHgeometry;

//_____________________________________________________________________________
class IlcDCHpadPlane : public TObject {

 public:

  IlcDCHpadPlane();
  IlcDCHpadPlane(Int_t p, Int_t c);
  IlcDCHpadPlane(const IlcDCHpadPlane &p);
  virtual           ~IlcDCHpadPlane();
  IlcDCHpadPlane    &operator=(const IlcDCHpadPlane &p);
  virtual void       Copy(TObject &p) const;

  Int_t    GetPadRowNumber(Double_t z) const;
  Int_t    GetPadColNumber(Double_t rphi, Double_t rowOffset) const;
  Double_t GetTiltOffset(Double_t rowOffset) const 
                                             { return fTiltingTan * (rowOffset - 0.5*fLengthIPad); };
  Double_t GetPadRowOffset(Int_t row, Double_t z) const
                                             { if ((row < 0) || (row >= fNrows))
                                                 return -1.0;
                                               else 
                                                 return fPadRow[row] - z;    };

  Double_t GetPadColOffset(Int_t col, Double_t rphi) const
                                             { if ((col < 0) || (col >= fNcols))
                                                 return -1.0;
                                               else
                                                 return fPadCol[col] - rphi; };

  Double_t GetTiltingAngle() const           { return fTiltingAngle; };

  Int_t    GetNrows() const                  { return fNrows;        };
  Int_t    GetNcols() const                  { return fNcols;        };

  Double_t GetRow0() const                   { return fPadRow[0];    };
  Double_t GetCol0() const                   { return fPadCol[0];    };

  Double_t GetRowEnd() const                 { return fPadRow[fNrows-1] - fLengthOPad; };
  Double_t GetColEnd() const                 { return fPadCol[fNcols-1] - fWidthOPad;  };

  Double_t GetRowPos(Int_t row) const        { return fPadRow[row];  };
  Double_t GetColPos(Int_t col) const        { return fPadCol[col];  };
  
  Double_t GetRowSize(Int_t row) const       { if ((row == 0) || (row == fNrows-1))
                                                 return fLengthOPad;
                                               else
                                                 return fLengthIPad; };
  Double_t GetColSize(Int_t col) const       { if ((col == 0) || (col == fNcols-1))
                                                 return fWidthOPad;
                                               else
                                                  return fWidthIPad; };
  Double_t GetLengthRim() const              { return fLengthRim;    };

 protected:


 IlcDCHgeometry *fGeo;       //! DCH geometry       

  Int_t     fPla;             //  Plane number
  Int_t     fCha;             //  Chamber number

  Double_t  fLength;          //  Length of pad plane in z-direction (row)
  Double_t  fWidth;           //  Width of pad plane in rphi-direction (col)

  Double_t  fLengthRim;       //  Length of the rim in z-direction (row)
  Double_t  fWidthRim;        //  Width of the rim in rphi-direction (col)

  Double_t  fLengthOPad;      //  Length of an outer pad in z-direction (row)
  Double_t  fWidthOPad;       //  Width of an outer pad in rphi-direction (col)

  Double_t  fLengthIPad;      //  Length of an inner pad in z-direction (row)
  Double_t  fWidthIPad;       //  Width of an inner pad in rphi-direction (col)

  Double_t  fRowSpacing;      //  Spacing between the pad rows
  Double_t  fColSpacing;      //  Spacing between the pad columns

  Int_t     fNrows;           //  Number of rows
  Int_t     fNcols;           //  Number of columns

  Double_t  fTiltingAngle;    //  Pad tilting angle  
  Double_t  fTiltingTan;      //  Tangens of pad tilting angle

  Double_t *fPadRow;          //! Pad border positions in row direction
  Double_t *fPadCol;          //! Pad border positions in column direction

  ClassDef(IlcDCHpadPlane,2)  //  DCH ROC pad plane

};

#endif
