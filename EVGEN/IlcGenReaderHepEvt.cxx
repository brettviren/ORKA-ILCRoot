/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
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

//
// Realisation of IlcGenReader to be used with IlcGenExtFile
// It reads Pandora_Pythia events from a Ascii file.
// The event format is defined in Init()
// NextEvent() is used to loop over events and NextParticle() to loop over particles.  
// Author: andreas.morsch@cern.ch
//
#include <TFile.h>
#include <TParticle.h>
#include <TTree.h>
#include <TDatabasePDG.h>

#include "IlcGenReaderHepEvt.h"
#include "IlcLog.h"
#include "IlcRun.h"
#include "IlcGenerator.h"
#include "IlcStack.h"

ClassImp(IlcGenReaderHepEvt)


IlcGenReaderHepEvt::IlcGenReaderHepEvt() 
{
// Default constructor
  fNcurrent   = 0;
  fFirstEvent = 0;
}

void IlcGenReaderHepEvt::Init() 
{
//
// open a new txt file
    extfile.open(fFileName);
    IlcInfo(Form("ASCII file %s opened", fFileName));

}

Int_t IlcGenReaderHepEvt::NextEvent() 
{
  // Read the next event
  
  if(extfile.eof()) return 0;

  Int_t ntracks, eventnum;
  extfile>> eventnum >> ntracks;
  point_Ev = extfile.tellg();
  Int_t CurrentEvent = eventnum;

  while(CurrentEvent < fFirstEvent){
    fNcurrent = 0;

    IlcInfo(Form("Skipping event %d of %d", CurrentEvent, fFirstEvent-1));
    IlcInfo(Form("There are %d tracks from Event %d", ntracks, eventnum));
    if(extfile.eof()) return 0;

    while(fNcurrent <= ntracks) {
      IlcInfo(Form("Reading track %d", fNcurrent));
      string buffer;
      getline(extfile, buffer);
      point_Ev = extfile.tellg();
      fNcurrent++;
    }

    IlcInfo(Form("Skipping %d track from Event %d of %d", ntracks, CurrentEvent, fFirstEvent-1));
    extfile>> eventnum >> ntracks;
    point_Ev = extfile.tellg();
    CurrentEvent = eventnum;

  }

  fNcurrent = 0;

  IlcInfo(Form("Reading event %d", CurrentEvent));
  IlcInfo(Form("There are %d tracks from Event %d", ntracks, eventnum));

  return ntracks;

}

TParticle* IlcGenReaderHepEvt::NextParticle() 
{
  Int_t isthep, idhep, jmohep1, jmohep2, jdahep1, jdahep2;
  Double_t px, py, pz, E, m, vx, vy, vz, t;
  Int_t ipart;

  // Read the next particle
    //extfile>> isthep >> idhep >> jdahep1 >> jdahep2 >> px >> py >> pz >> m;
  extfile>> ipart >> isthep >> idhep >> jmohep1 >> jmohep2 >> jdahep1 >> jdahep2 >> px >> py >> pz >> E >> m >> vx >> vy >> vz >> t;
    string buffer;        //these 2 lines need to read the endl in the ascii file
    getline(extfile, buffer);  //these 2 lines need to read the endl in the ascii file
   
    Double_t ptot = TMath::Sqrt(px*px+py*py+pz*pz);
    Double_t amass = TDatabasePDG::Instance()->GetParticle(idhep)->Mass();
    
    IlcDebug(2,Form("N: %d Part: %s isthep: %d Px=%f, Py=%f, Pz=%f, E=%f\n", fNcurrent,  TDatabasePDG::Instance()->GetParticle(idhep)->GetName(), isthep, px, py, pz, E));
    
    IlcDebug(1,Form("N: %d Part: %s isthep=%d, jmohep1=%d, jmohep2=%d, jdahep1=%d, jdahep2=%d, px=%f, py=%f, pz=%f, E=%f, m=%f, vx=%f, vy=%f, vz=%f, t=%f\n", fNcurrent,  TDatabasePDG::Instance()->GetParticle(idhep)->GetName(),isthep,jmohep1-1,jmohep2-1,jdahep1-1,jdahep2-1,px,py,pz,E,m,vx,vy,vz,t));
    
    //TParticle* particle = new TParticle(idhep, 0, -1, -1, -1, -1, px, py, pz, E, 0., 0., 0., 0.);
    TParticle* particle = new TParticle(idhep, isthep, jmohep1-1, -1, jdahep1-1, jdahep2-1, px, py, pz, E, vx, vy, vz, t);
    
    if(isthep<=1) particle->SetBit(kTransportBit);
    
    fNcurrent++;	      
    //fNparticle++;
    
    //gIlc->Generator()->SetTrackingFlag(1);
    //if(isthep > 1) //Track only particle that have isthep of 0 or 1
    //gIlc->Generator()->SetTrackingFlag(0);


    return particle;
}



IlcGenReaderHepEvt& IlcGenReaderHepEvt::operator=(const  IlcGenReaderHepEvt& rhs)
{
// Assignment operator
    rhs.Copy(*this);
    return (*this);
}

void IlcGenReaderHepEvt::Copy(TObject&) const
{
    //
    // Copy 
    //
    IlcFatal("Not implemented!\n");
}






