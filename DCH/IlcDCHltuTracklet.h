#ifndef ILCDCHLTUTRACKLET_H
#define ILCDCHLTUTRACKLET_H

#include <TObject.h>

class IlcDCHltuTracklet : public TObject {
  
 public:

  enum { kNplan = 6 };

  IlcDCHltuTracklet(Int_t det, 
		    Int_t row, 
		    Float_t rowz,
		    Float_t slope, 
		    Float_t offset, 
		    Float_t time, 
		    Int_t ncl,
		    Int_t label,
		    Float_t q);

  virtual ~IlcDCHltuTracklet();

  Bool_t  IsSortable()   const { return kTRUE; }
  virtual Int_t   Compare(const TObject *o) const;

  Int_t   GetDetector()  const { return fDetector; };
  Int_t   GetPlane(Int_t det) const { return ((Int_t) (det % kNplan)); };
  Int_t   GetRow()       const { return fRow; };
  Int_t   GetNclusters() const { return fNclusters; };
  Float_t GetSlope()     const { return fSlope; };
  Float_t GetOffset()    const { return fY; };
  Float_t GetTime0()     const { return fX; };
  Float_t GetRowz()      const { return fRowz; };
  Float_t GetYproj(Float_t xpl) const;
  Float_t GetZproj(Float_t xpl) const;
  Int_t   GetLabel()     const { return fLabel; };
  Float_t GetPt(Float_t field) const;
  Float_t GetQ() const { return fQ; };

 protected:

  Float_t fX;                              // distance vertex to entrance window
  Float_t fY;                              // tracklet offset at entrance window
  Float_t fSlope;                          // tracklet slope
  Float_t fRowz;                           // z coordinate of the pad row center
  Int_t   fDetector;                       // detector number
  Int_t   fRow;                            // pad row number 
  Int_t   fNclusters;                      // number of clusters
  Int_t   fLabel;                          // mc track label
  Float_t fQ;                              // charge sum divided by number of clusters

  ClassDef(IlcDCHltuTracklet,2)

};

#endif
