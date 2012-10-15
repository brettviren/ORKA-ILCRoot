 /**************************************************************************
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

/* $Id: IlcTriggerScalersRecord.cxx 22322 2007-11-22 11:43:14Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
//
// Class to define the ILC Trigger Scalers Record 
//
// Each record consists of 1 time stamp (4 words)  (IlcTimeStamp)
// and an array with the scalers (IlcTriggerScalers) for each trigger class 
// in partition  
//
//////////////////////////////////////////////////////////////////////////////

#include <Riostream.h>
#include <TObjArray.h>
#include "IlcLog.h"  
#include "IlcTimeStamp.h"
#include "IlcTriggerScalers.h"
#include "IlcTriggerScalersRecord.h"

ClassImp( IlcTriggerScalersRecord )
//_____________________________________________________________________________
IlcTriggerScalersRecord::IlcTriggerScalersRecord():
  fTimestamp(),
  fScalers(),
  fTimeGroup(0)
{
 //Default constructor
}

//_____________________________________________________________________________
void IlcTriggerScalersRecord::SetTimeStamp( UInt_t orbit, UInt_t period, 
                                            UInt_t seconds, UInt_t microsecs )
{
   fTimestamp.SetTimeStamp( orbit, period, seconds, microsecs );
}

//_____________________________________________________________________________
void IlcTriggerScalersRecord::AddTriggerScalers( IlcTriggerScalers* scaler ) 
{ 
  fScalers.AddLast( scaler ); 
  fScalers.Sort(); 
}

//_____________________________________________________________________________
void IlcTriggerScalersRecord::AddTriggerScalers( UChar_t classIndex, UInt_t LOCB, UInt_t LOCA,        
                                         UInt_t L1CB, UInt_t L1CA, UInt_t L2CB, UInt_t L2CA )
{
    AddTriggerScalers( new IlcTriggerScalers( classIndex, LOCB, LOCA, L1CB, L1CA, L2CB, L2CA ) );
    fScalers.Sort();
} 

//_____________________________________________________________________________
Int_t IlcTriggerScalersRecord::Compare( const TObject* obj ) const
{
  // Compare  timestamps
  
  return fTimestamp.Compare( &(((IlcTriggerScalersRecord*)obj)->fTimestamp) );
}
//_____________________________________________________________________________
const IlcTriggerScalers* IlcTriggerScalersRecord::GetTriggerScalersForClass( const Int_t classindex ) const
{
   // Find Trigger scaler with class ID = classindex using a brutal force 

   Int_t   position, last;
   IlcTriggerScalers *op2 = 0;
   position = 0;
   last = fScalers.GetEntriesFast();
   while (position < last) {
      op2 = (IlcTriggerScalers *)fScalers.At(position);
      if( op2 && (op2->GetClassIndex() == classindex )) break;
      op2=0;
      position++;
   }
   return op2;   
}

//_____________________________________________________________________________
IlcTriggerScalers* IlcTriggerScalersRecord::GetTriggerScalersForClassBinary( const Int_t classindex )
{
   // Find Trigger scaler with class ID = classindex using a binary search. 

   Int_t   base, position, last, result = 0;
   IlcTriggerScalers *op2 = NULL;
   
   fScalers.Sort(); 
   
   base = 0;
   last = fScalers.GetEntriesFast();

   while (last >= base) {
      result = 0;
      position = (base+last) / 2;
      op2 = (IlcTriggerScalers *)fScalers.At(position);
      if( op2 && op2->GetClassIndex() > classindex ) result = -1;
      if( op2 && op2->GetClassIndex() < classindex ) result = 1;
  
      if (op2 && result == 0)
         return op2;
      if (!op2 || result < 0)
         last = position-1;
      else
         base = position+1;
      op2 = NULL;   
   }
   return op2;   
}
                                      
//_____________________________________________________________________________
void IlcTriggerScalersRecord::Print( const Option_t* ) const
{
   // Print
  cout << "Trigger Scalers Record: " << endl;
  fTimestamp.Print();
  for( Int_t i=0; i<fScalers.GetEntriesFast(); ++i ) 
     ((IlcTriggerScalers*)fScalers.At(i))->Print();
}
