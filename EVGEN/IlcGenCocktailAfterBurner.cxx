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

/* $Id: IlcGenCocktailAfterBurner.cxx 54029 2012-01-20 14:07:51Z hristov $ */

// 
// Container class for IlcGenerator and AfterBurners 
// (which are IlcGenerators as well) through recursion.
// The container is itself an IlcGenerator a
// what is stored are not the pointers to the generators directly 
// but to objects of type
// IlcGenCocktailAfterBurner entry.   
// The class provides also iterator functionality.  
// Author: andreas.morsch@cern.ch and piotr.skowronski@cern.ch
//
// 24.09.2001  Piotr Skowronski
//             debug -> gDebug,
//             fNEvents replaced with IlcRunLoader::GetNumberOfEvents()
//


#include <Riostream.h>

#include <TList.h>
#include <TObjArray.h>
#include <TParticle.h>

#include "IlcGenCocktailAfterBurner.h"
#include "IlcGenCocktailEntry.h"
#include "IlcGenCocktailEventHeader.h"
#include "IlcCollisionGeometry.h"
#include "IlcStack.h"
#include "IlcMC.h"
#include "IlcRun.h"


ClassImp(IlcGenCocktailAfterBurner)
/*********************************************************************/ 
/*********************************************************************/ 

    IlcGenCocktailAfterBurner::IlcGenCocktailAfterBurner():
	fNAfterBurners(0),
	fAfterBurnerEntries(0),
	fGenerationDone(kFALSE),
	fInternalStacks(0),
	fCollisionGeometries(0),
	fHeaders(0),
	fCurrentEvent(0),
	fActiveStack(0),
	fActiveEvent(-1),
	fCurrentGenerator(0),
	fNBgEvents(0)
{
// Constructor
    if (gDebug > 0) 
	cout<<"IlcGenCocktailAfterBurner::IlcGenCocktailAfterBurner()"<<endl;
    SetName("IlcGenCocktailAfterBurner");
    SetTitle("IlcGenCocktailAfterBurner");
}

/*********************************************************************/ 

IlcGenCocktailAfterBurner::~IlcGenCocktailAfterBurner()
  {
//destructor

    if (fInternalStacks) //delete stacks
     { 
       fInternalStacks->SetOwner();
       delete fInternalStacks;
    }
    if (fAfterBurnerEntries) delete fAfterBurnerEntries; //delete entries
    Int_t numberOfEvents = IlcRunLoader::Instance()->GetNumberOfEventsPerRun();
    if (fCollisionGeometries) {
      for (Int_t i = 0; i < (numberOfEvents + fNBgEvents); i++)
	if (fCollisionGeometries[i]) delete fCollisionGeometries[i];
      delete[] fCollisionGeometries;
    }
    if (fHeaders) {
      for (Int_t i = 0; i < (numberOfEvents + fNBgEvents); i++)
	if (fHeaders[i]) delete fHeaders[i];
      delete[] fHeaders;
    }
  }
/*********************************************************************/ 
/*********************************************************************/ 

void IlcGenCocktailAfterBurner::
AddAfterBurner(IlcGenerator *AfterBurner, char* Name, Float_t RateExp)
{
//
//  Forward parameters to the new AfterBurner
    
    if (gDebug>0)cout<<"IlcGenCocktailAfterBurner::AddAfterBurner  Named "<<Name<<endl;

    if(TestBit(kPtRange) && !(AfterBurner->TestBit(kPtRange)) && !(AfterBurner->TestBit(kMomentumRange))) 
	AfterBurner->SetPtRange(fPtMin,fPtMax);
    if(TestBit(kMomentumRange) && !(AfterBurner->TestBit(kPtRange)) && !(AfterBurner->TestBit(kMomentumRange)))
	AfterBurner->SetMomentumRange(fPMin,fPMax);
    
    if (TestBit(kYRange) && !(AfterBurner->TestBit(kYRange)))
	AfterBurner->SetYRange(fYMin,fYMax);
    if (TestBit(kPhiRange) && !(AfterBurner->TestBit(kPhiRange)))
	AfterBurner->SetPhiRange(fPhiMin*180/TMath::Pi(),fPhiMax*180/TMath::Pi());
    if (TestBit(kThetaRange) && !(AfterBurner->TestBit(kThetaRange)) && !(AfterBurner->TestBit(kEtaRange)))
	AfterBurner->SetThetaRange(fThetaMin*180/TMath::Pi(),fThetaMax*180/TMath::Pi());
    if (!(AfterBurner->TestBit(kVertexRange))) {
	AfterBurner->SetOrigin(fOrigin[0], fOrigin[1], fOrigin[2]);
	AfterBurner->SetSigma(fOsigma[0], fOsigma[1], fOsigma[2]);
	AfterBurner->SetVertexSmear(fVertexSmear);
	AfterBurner->SetVertexSource(kContainer);
	AfterBurner->SetTimeOrigin(fTimeOrigin);
    }
    AfterBurner->SetTrackingFlag(fTrackIt);
    //AfterBurner->SetContainer(this);

//
//  Add AfterBurner to list   
    
    IlcGenCocktailEntry *entry = 
	new IlcGenCocktailEntry(AfterBurner, Name, RateExp);
    if (!fAfterBurnerEntries) fAfterBurnerEntries = new TList();
    
    fAfterBurnerEntries->Add(entry);
    fNAfterBurners++;
//
    
}
/*********************************************************************/ 
/*********************************************************************/ 

void IlcGenCocktailAfterBurner::Init()
{
// Initialisation
  Int_t numberOfEvents = IlcRunLoader::Instance()->GetNumberOfEventsPerRun();
    fGenerationDone = kFALSE;
    if (fInternalStacks) //delete stacks
     { 
       fInternalStacks->SetOwner();
       fInternalStacks->Delete(); //clean after previous generation cycle
     }

    if (fCollisionGeometries) {
      for (Int_t i = 0; i < (numberOfEvents + fNBgEvents); i++)
	if (fCollisionGeometries[i]) delete fCollisionGeometries[i];
      delete[] fCollisionGeometries;
    }
    if (fHeaders) {
      for (Int_t i = 0; i < (numberOfEvents + fNBgEvents); i++)
	if (fHeaders[i]) delete fHeaders[i];
      delete[] fHeaders;
    }

    this->IlcGenCocktail::Init(); 
    
    if (gDebug>0) cout<<"IlcGenCocktailAfterBurner::Init"<<endl;
    TIter next(fAfterBurnerEntries);
    IlcGenCocktailEntry *entry;
    //
    // Loop over generators and initialize
    while((entry = (IlcGenCocktailEntry*)next())) {
	entry->Generator()->Init();
    }  
}
/*********************************************************************/ 
/*********************************************************************/ 

void IlcGenCocktailAfterBurner::Generate()
{
//
// Generate event
//  Firsts runs each generator for all events
//  than after burners ones for each event
//
//  It generates and processes all events during
//  first call only.
//  In next calls it just returns already generated 
//  and processed events to the gIlc

    if (gDebug>0)
      cout<<"#####################################"<<endl
          <<"#IlcGenCocktailAfterBurner::Generate#"<<endl
          <<"#####################################"<<endl;
    // Initialize header
    //
    Int_t i; //iterator
    IlcStack * stack;
    
    if (fGenerationDone)
    {//if generation is done (in first call) 
     //just copy particles from the stack to the gIlc
      SetTracks(++fCurrentEvent);
      fHeader = fHeaders[fCurrentEvent];
      gIlc->SetGenEventHeader(fHeader); 
      cout<<"Returning event " << fCurrentEvent<<endl;
      return;  
    }
    else
    { //Here we are in the first call of the method
	Int_t numberOfEvents = IlcRunLoader::Instance()->GetNumberOfEventsPerRun();
	cout << "Number of events per run" <<  numberOfEvents << endl;
	TArrayF eventVertex;
	eventVertex.Set(3 * (numberOfEvents + fNBgEvents));
	TArrayF eventTime;
	eventTime.Set(numberOfEvents + fNBgEvents);
	fCurrentEvent=0;
      //Create stacks
	fInternalStacks      = new TObjArray(numberOfEvents + fNBgEvents); //Create array of internal stacks
	fCollisionGeometries = new IlcCollisionGeometry*[numberOfEvents + fNBgEvents]; //Create array of collision geometries
	fHeaders             = new IlcGenCocktailEventHeader*[numberOfEvents + fNBgEvents]; //Create array of headers   

	for(i = 0; i < numberOfEvents + fNBgEvents; i++) 
       {	
	   stack = new IlcStack(10000);
	   stack->Reset();
	   fInternalStacks->Add(stack);
	   Vertex();
	   for (Int_t j = 0; j < 3; j++) eventVertex[3 * i +  j] = fVertex[j];
	   eventTime[i] = fTime;
	   fHeaders[i] = new IlcGenCocktailEventHeader();
	   fCollisionGeometries[i] = 0;
       }
/*********************************************************************/ 
      TIter next(fEntries);
      IlcGenCocktailEntry *entry;
      IlcGenCocktailEntry *e1;
      IlcGenCocktailEntry *e2;
      const TObjArray *partArray;
  //
  // Loop over generators and generate events
      Int_t igen=0;
      while((entry = (IlcGenCocktailEntry*)next())) 
      {
        igen++;
        cout<<"Generator "<<igen<<"  : "<<entry->GetName()<<endl;
/***********************************************/
//First generator for all evenets, than second for all events, etc...
        for(i = 0; i < numberOfEvents + fNBgEvents; i++) 
	{  
	    cout<<"                  EVENT "<<i << endl;
            stack = GetStack(i);
            partArray = stack->Particles();
            fCurrentGenerator = entry->Generator();
            fCurrentGenerator->SetStack(stack);
            if (igen ==1) 
	    {
                entry->SetFirst(0);
	    } 
            else 
	    {
                entry->SetFirst((partArray->GetEntriesFast())+1);
	    }
	    // Set the vertex for the generator
	    Int_t ioff = 3 * i;
	    fCurrentGenerator->SetVertex(eventVertex.At(ioff), eventVertex.At(ioff + 1), eventVertex.At(ioff + 2));
	    fCurrentGenerator->SetTime(eventTime.At(i));
	    fHeader = fHeaders[i];
	    // Set the vertex and time for the cocktail
	    TArrayF v(3);
	    for (Int_t j=0; j<3; j++) v[j] = eventVertex.At(ioff + j);
	    fHeader->SetPrimaryVertex(v);
	    fHeader->SetInteractionTime(eventTime.At(i));
	    // Generate event
	    fCurrentGenerator->Generate();
	    //
	    entry->SetLast(partArray->GetEntriesFast());
	    
	    if (fCurrentGenerator->ProvidesCollisionGeometry())  
	      fCollisionGeometries[i] = 
		new IlcCollisionGeometry(*(fCurrentGenerator->CollisionGeometry()));
	} // event loop
/***********************************************/
      } // generator loop
      next.Reset();
      while((entry = (IlcGenCocktailEntry*)next())) 
        {
          entry->PrintInfo();
        }
      for ( entry=FirstGenerator();entry;entry=NextGenerator() ) 
        {
          entry->PrintInfo();
        }
      for (FirstGeneratorPair(e1,e2); (e1&&e2); NextGeneratorPair(e1,e2) )
        {
          printf("\n -----------------------------");
          e1->PrintInfo();
          e2->PrintInfo();
        }
	
	
      /***********************************************/
      /*******After Burners Processing****************/
      /***********************************************/
      TIter nextAfterBurner(fAfterBurnerEntries);
      IlcGenCocktailEntry *afterBurnerEntry;
      Int_t iab =0; //number of current after burner / counter
      
      cout<<"\n\nRunning After Burners"<<endl;
      while((afterBurnerEntry = (IlcGenCocktailEntry*)nextAfterBurner()))
        {
          cout<<"After Burner "<<iab++<<"  :"<<afterBurnerEntry->GetName()<<endl;
          fCurrentGenerator = afterBurnerEntry->Generator();
          fCurrentGenerator->Generate();
        }
      cout<<endl<<"Finished. Processed "<<iab<<" After Burners"<<endl;

      /***********************************************/
      /***********************************************/
      /***********************************************/       
        
      fGenerationDone=kTRUE; 
      SetTracks(0); //copy event 0 to gIlc stack
	
/*********************************************************************/
      // Pass the header to gIlc
      fHeader = fHeaders[0];
      gIlc->SetGenEventHeader(fHeader); 
    } //else generated
}
/*********************************************************************/
/*********************************************************************/ 

IlcStack* IlcGenCocktailAfterBurner::GetStack(Int_t n) const
{
//Returns the pointer to the N'th stack (event)
  if( ( n<0 ) || ( n >= (GetNumberOfEvents()) ) )
    {
      Fatal("IlcGenCocktailAfterBurner::GetStack","Asked for non existing stack (%d)",n);
      return 0; 
    }
    return ((IlcStack*) fInternalStacks->At(n) );
}

/*********************************************************************/ 
/*********************************************************************/ 

IlcCollisionGeometry* IlcGenCocktailAfterBurner::GetCollisionGeometry(Int_t n) const
{
//Returns the pointer to the N'th stack (event)
  if( ( n<0 ) || ( n>=GetNumberOfEvents() ) )
    {
      Fatal("IlcGenCocktailAfterBurner::GetCollisionGeometry","Asked for non existing stack (%d)",n);
      return 0; 
    }
    return fCollisionGeometries[n];
}

/*********************************************************************/ 
/*********************************************************************/ 

void IlcGenCocktailAfterBurner::SetActiveEventNumber(Int_t actev)
{
//Set Active Events Number and Active Stack
//There is only one active event number
//Made fo convinience of work with AfterBurners (HBT processor)

    fActiveEvent = actev;
    fActiveStack = GetStack(actev);
}
/*********************************************************************/ 
/*********************************************************************/ 

void IlcGenCocktailAfterBurner::SetTracks(Int_t stackno)
{
//Method which copies tracks from given stack to the
//gIlc's stack
    IlcStack* instack = GetStack(stackno);
    Int_t done;
    Int_t parent; 
    Int_t pdg;
    Double_t px, py, pz, e, vx, vy, vz, tof, polx, poly, polz;
    TMCProcess mech;
    Int_t ntr;
    Float_t weight;
    TVector3 pol;
    
    TParticle * p;
    Int_t n = instack->GetNtrack();
    if (gDebug) 
    {
      cout<<"IlcGenCocktailAfterBurner::SetTracks("<<stackno<<"). Number of particles is: "<<n<<"\n";
    }
    
    for(Int_t i = 0; i < n; i++)
    {
	
      p = instack->Particle(i);
      done = !p->TestBit(kDoneBit);
      parent = p->GetMother(0);
      pdg = p->GetPdgCode();
      px = p->Px();
      py = p->Py();
      pz = p->Pz();
      e  = p->Energy();
      vx = p->Vx();
      vy = p->Vy();
      vz = p->Vz();
      tof = p->T();
      p->GetPolarisation(pol);
      polx = pol.X();
      poly = pol.Y();
      polz = pol.Z();
      mech = IlcGenCocktailAfterBurner::IntToMCProcess(p->GetUniqueID());
      weight = p->GetWeight();

      gIlc->GetMCApp()->PushTrack(done, parent, pdg, px, py, pz, e, vx, vy, vz, tof,polx, poly, polz, mech, ntr, weight);

      SetHighWaterMark(ntr) ; 

    }
}
/*********************************************************************/ 
/*********************************************************************/ 

TMCProcess IlcGenCocktailAfterBurner::IntToMCProcess(Int_t no)
{
 //Mothod used to convert uniqueID (integer) to TMCProcess type
    const TMCProcess kMCprocesses[kMaxMCProcess] = 
    {
     kPNoProcess, kPMultipleScattering, kPEnergyLoss, kPMagneticFieldL, 
     kPDecay, kPPair, kPCompton, kPPhotoelectric, kPBrem, kPDeltaRay,
     kPAnnihilation, kPHadronic, kPNoProcess, kPEvaporation, kPNuclearFission,
     kPNuclearAbsorption, kPPbarAnnihilation, kPNCapture, kPHElastic, 
     kPHInhelastic, kPMuonNuclear, kPTOFlimit,kPPhotoFission, kPNoProcess, 
     kPRayleigh, kPNoProcess, kPNoProcess, kPNoProcess, kPNull, kPStop
    };
    
    for (Int_t i = 0;i<kMaxMCProcess;i++)
    {
      if (kMCprocesses[i] == no)
        {
          return kMCprocesses[i];
        }
    } 
    return kPNoProcess;
}

