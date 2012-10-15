/*************************************************************************
 * 2012-2013 Infrastructure for Large Collider Experiment.       *
 * All rights reserved.                                                   *
 * Author: The International Lepton Collider Off-line Project.            *
 *                                                                        *
 * Part of the code has been developed by ILC Off-line Project.         *
 * Contributors are mentioned in the code where appropriate.              *
*                                                                        *
* Permission to use, copy, modify and distribute this software and its   *
* documentation strictly for non-commercial purposes is hereby granted   *
* without fee, provided that the above copyright notice appears in all   *
* copies and that both the copyright notice and this permission notice   *
* appear in the supporting documentation. The authors make no claims     *
* about the suitability of this software for any purpose. It is          *
* provided "as is" without express or implied warranty.                  * 
**************************************************************************/


#include <Riostream.h>
#include <TObjArray.h>
#include <IlcTimeStamp.h>
#include "IlcLog.h"  
#include "IlcTriggerScalersESD.h"
#include "IlcTriggerScalersRecordESD.h"

ClassImp( IlcTriggerScalersRecordESD )
//_____________________________________________________________________________
IlcTriggerScalersRecordESD::IlcTriggerScalersRecordESD():
TObject(),
fTimestamp(),
fScalers(),
fTimeGroup(0)
{
}

//_____________________________________________________________________________
void IlcTriggerScalersRecordESD::AddTriggerScalers( IlcTriggerScalersESD* scaler ) 
{ 
  fScalers.AddLast( scaler ); 
}

//_____________________________________________________________________________
void IlcTriggerScalersRecordESD::AddTriggerScalers( UChar_t classIndex, ULong64_t LOCB, ULong64_t LOCA,        
                                         ULong64_t L1CB, ULong64_t L1CA, ULong64_t L2CB, ULong64_t L2CA )
{
    AddTriggerScalers( new IlcTriggerScalersESD( classIndex, LOCB, LOCA, L1CB, L1CA, L2CB, L2CA ) );
} 

//_____________________________________________________________________________
IlcTriggerScalersRecordESD::IlcTriggerScalersRecordESD( const IlcTriggerScalersRecordESD &rec ) :
TObject(rec),
fTimestamp(rec.fTimestamp),
fScalers(),
fTimeGroup(rec.fTimeGroup)
{
//copy constructor
for (Int_t i = 0; i < rec.fScalers.GetEntriesFast(); i++) {
    if (rec.fScalers[i]) fScalers.Add(rec.fScalers[i]->Clone());
  }
}
//_____________________________________________________________________________
IlcTriggerScalersRecordESD& IlcTriggerScalersRecordESD:: operator=(const IlcTriggerScalersRecordESD& rec)
{
//asignment operator
if(&rec == this) return *this;
((TObject *)this)->operator=(rec);
fTimestamp=rec.fTimestamp;
fScalers.Delete();
for (Int_t i = 0; i < rec.fScalers.GetEntriesFast(); i++) {
    if (rec.fScalers[i]) fScalers.Add(rec.fScalers[i]->Clone());
  }
fTimeGroup=rec.fTimeGroup;
return *this;
} 

//_____________________________________________________________________________
void IlcTriggerScalersRecordESD::Reset()
{
fScalers.SetOwner();
fScalers.Clear();
fTimeGroup=0;
} 

//_____________________________________________________________________________
const IlcTriggerScalersESD* IlcTriggerScalersRecordESD::GetTriggerScalersForClass( const Int_t classindex ) const
{
   // Find Trigger scaler with class ID = classmask using a binary search. 

   Int_t   base, last;
   IlcTriggerScalersESD *op2 = NULL;
   
   base = 0;
   last = fScalers.GetEntriesFast();

   while (base < last) {
      op2 = (IlcTriggerScalersESD *)fScalers.At(base);
      if( op2->GetClassIndex()  == classindex ) return op2;
      base++;
   }
   return op2;   
}
                                      
//_____________________________________________________________________________
void IlcTriggerScalersRecordESD::Print( const Option_t* ) const
{
   // Print
  cout << "Trigger Scalers Record, time group: "<< fTimeGroup << endl;
  for( Int_t i=0; i<fScalers.GetEntriesFast(); ++i ) 
     ((IlcTriggerScalersESD*)fScalers.At(i))->Print();
}
