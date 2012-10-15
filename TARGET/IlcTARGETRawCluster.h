#ifndef ILCTARGETRAWCLUSTER_H
#define ILCTARGETRAWCLUSTER_H

////////////////////////////////////////////////////
//  Cluster classes for set:TARGET                   //
////////////////////////////////////////////////////

#include <TObject.h>
// this class is subject to changes !!! - info used for declustering
// and  eventual debugging

class IlcTARGETRawCluster : public TObject {  
 public:
  IlcTARGETRawCluster();
    virtual ~IlcTARGETRawCluster() {// destructor
    }
    virtual Bool_t IsSortable() const {// is sortable
	return kTRUE;}
    virtual void SetMultiplicity(Int_t m) {fMultiplicity=m;}
 protected:
    Int_t       fMultiplicity;        // cluster multiplicity
  
    ClassDef(IlcTARGETRawCluster,1)  // IlcTARGETRawCluster class
};

#endif
