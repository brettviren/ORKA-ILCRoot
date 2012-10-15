#ifndef ILCTARGETTABLESSD_H
#define ILCTARGETTABLESSD_H
/* Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 * See cxx source for full Copyright notice     */

#include "Rtypes.h"
#include <TObject.h>

class IlcTARGETTableSSD : public TObject{
 public:
    IlcTARGETTableSSD(); // Default constructor
    IlcTARGETTableSSD(const IlcTARGETTableSSD & source); //Copy constructor
    IlcTARGETTableSSD(Int_t noelem); // Standard Constructor
    virtual ~IlcTARGETTableSSD(); //destructor
    IlcTARGETTableSSD& operator=(const IlcTARGETTableSSD &source);// = operator
    void Add(Int_t side, Int_t strip); // add an element to the table
    void Clear(); // Clears the contents of the table
    void DumpTable(); // it dumps the contents of the table
    Int_t Use(Int_t side); // use current element - returns -1 if none
    
    virtual void Clear(Option_t*)  {TObject::Clear();}

 private:
    Int_t SearchValue(Int_t *arr, Int_t refer, Int_t max){
	for(Int_t i=0;i<max;i++)if(arr[i]==refer)return i;
	return -1;}
 private:
    Int_t fDim;        //! dimension of the table 
    Int_t * fArray;     //! table
    Int_t fCurrUse[2];    //! current element in use (0: P side; 1: N side)
    Int_t fCurrFil[2];    //! element to be filled (0: P side; 1: N side)
    ClassDef(IlcTARGETTableSSD,0) // SSD table
};
#endif
