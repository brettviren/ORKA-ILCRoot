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

/* $Id: IlcTimeStamp.cxx 22322 2007-11-22 11:43:14Z cvetan $ */

///////////////////////////////////////////////////////////////////////////////
//
//  Class to define Event Timestamp from : 
//
//               Orbit
//               Period counter
//               Seconds    |
//                  +       |   <===> Bunch cross 
//               Microsecs  | 
//
//////////////////////////////////////////////////////////////////////////////

#include <Riostream.h>


#include "TObject.h"

#include "IlcLog.h"   
#include "IlcTimeStamp.h"

ClassImp(IlcTimeStamp)
   
//_____________________________________________________________________________
   
const Int_t IlcTimeStamp::fNanosecPerBC = 25;   // nanosecs per bunch cross
   
//_____________________________________________________________________________
IlcTimeStamp::IlcTimeStamp():
  fOrbit(0),        
  fPeriod(0),        
  fBunchCross(0)
{
 // Default constructor
}     

//_____________________________________________________________________________
IlcTimeStamp::IlcTimeStamp( UInt_t orbit, UInt_t period, 
                            ULong64_t bunchcross ):
  fOrbit(orbit),        
  fPeriod(period),        
  fBunchCross( bunchcross )
{
}
//_____________________________________________________________________________
IlcTimeStamp::IlcTimeStamp( UInt_t orbit, UInt_t period, 
                            UInt_t seconds, UInt_t microsecs):
  fOrbit(orbit),        
  fPeriod(period),        
  fBunchCross( (ULong64_t)((seconds*1000000.+microsecs)*1000./fNanosecPerBC+0.5) )
{
}     
//___________________________________________________________________________
IlcTimeStamp::IlcTimeStamp(const IlcTimeStamp &stamp):
 TObject(stamp),
 fOrbit(stamp.fOrbit),
 fPeriod(stamp.fPeriod),
 fBunchCross(stamp.fBunchCross)
{
 // copy constructor
}
//_____________________________________________________________________________
IlcTimeStamp& IlcTimeStamp::operator=(const IlcTimeStamp &stamp)
{
 //assignment operator
 if(this==&stamp) return *this;
 ((TObject *)this)->operator=(stamp);
 fOrbit=stamp.fOrbit;
 fPeriod=stamp.fPeriod;
 fBunchCross=stamp.fBunchCross;
 return *this;
}
//_____________________________________________________________________________
void IlcTimeStamp::SetTimeStamp( UInt_t orbit, UInt_t period, 
                            ULong64_t bunchcross )
{
  fOrbit = orbit;        
  fPeriod = period;  
  fBunchCross = bunchcross;
}
 
//_____________________________________________________________________________
void IlcTimeStamp::SetTimeStamp( UInt_t orbit, UInt_t period, 
                            UInt_t seconds, UInt_t microsecs )
{
  fOrbit = orbit;        
  fPeriod = period;  
  fBunchCross = (ULong64_t)((seconds*1000000.+microsecs)*1000./fNanosecPerBC+0.5);
}
                            
     

//_____________________________________________________________________________
Int_t IlcTimeStamp::Compare( const TObject* obj ) const
{
  // Compare 

  if( fPeriod > ((IlcTimeStamp*)obj)->fPeriod )  return 1;
  else { if( fPeriod < ((IlcTimeStamp*)obj)->fPeriod )  return -1;
  else { if( fOrbit > ((IlcTimeStamp*)obj)->fOrbit )  return 1;
  else { if( fOrbit < ((IlcTimeStamp*)obj)->fOrbit )  return -1;
  else { if( fBunchCross > ((IlcTimeStamp*)obj)->fBunchCross )  return 1;
  else { if( fBunchCross < ((IlcTimeStamp*)obj)->fBunchCross )  return -1;
  else return 0;
}}}}}

}

//_____________________________________________________________________________
void IlcTimeStamp::Print( const Option_t* ) const
{
   // Print
  cout << "Timestamp: " << endl;
  cout << "  Orbit: " << fOrbit << " Period: " << fPeriod << endl;
  cout << "  Bunch Cross: " << GetBunchCross() << endl;
  cout << "  Seconds: " << GetSeconds() << " MicroSecs: " << GetMicroSecs() << endl;
}
