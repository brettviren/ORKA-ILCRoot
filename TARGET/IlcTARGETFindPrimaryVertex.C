#if !defined(__CINT__) || defined(__MAKECINT__)
#include <TClassTable.h>
#include <TFile.h>
#include <TTree.h>
#include <Riostream.h>
#include <IlcRun.h>
#include <IlcHeader.h>
#include <IlcGenEventHeader.h>
#include <IlcTARGETVertexerIons.h>
#include <IlcRunLoader.h>
#include <IlcTARGETVertexerIons.h>
#include <IlcTARGETLoader.h>
#include <unistd.h>

#endif

void IlcTARGETFindPrimaryVertex(Int_t evNumber1=0,Int_t NumbofEv=1, const char *filename="gilc.root") {

  Int_t evNumber2 = evNumber1+NumbofEv;
  
  if (gClassTable->GetID("IlcRun") < 0) {
    gROOT->Macro("loadlibs.C");
  } else {
    if(gIlc){
      delete gIlc->GetRunLoader();
      delete gIlc;
      gIlc=0;
    }
  }
  
  IlcRunLoader* rl = IlcRunLoader::Open("gilc.root");
  if (rl == 0x0){
    ::Error("IlcTARGETFindPrimaryVertex.C","Can not open session RL=NULL");
    return;
  }
  
  // Open output file for vertices (default name: TARGET.Vertex.root 
  // and Create vertexer

  IlcTARGETVertexerIons *vertexer = new IlcTARGETVertexerIons("default");
  //vertexer->SetDebug(1);
  
  IlcESDVertex *V;
  //   Loop over events 
   
  for (int nev=evNumber1; nev< evNumber2; nev++) {
    cout<<"=============================================================\n";
    cout<<" Processing event "<<nev<<endl;
    cout<<"=============================================================\n";
    rl->GetEvent(nev);
    IlcHeader *header = rl->GetHeader();
    IlcGenEventHeader* genEventHeader = header->GenEventHeader();
    TArrayF primaryVertex(3);
    genEventHeader->PrimaryVertex(primaryVertex);
    
    IlcGenHijingEventHeader* hijingHeader = (IlcGenHijingEventHeader*)  genEventHeader;
    Float_t b = hijingHeader->ImpactParameter();   
    cout << "Impact parameter = " << b << " fm" << endl;

    TStopwatch timer;
    timer.Start();

    V=vertexer->FindVertexForCurrentEvent(nev);

    TVector3 vtrue(primaryVertex[0],primaryVertex[1],primaryVertex[2]);
    TVector3 vfound(V->GetXv(),V->GetYv(),V->GetZv());
    TVector3 dif=vtrue-vfound;
    cout << "True vertex coordinates (cm) = " << vtrue.X() << " " << vtrue.Y() << " " << vtrue.Z() << endl;
    cout << "Found vertex coordinates  (cm) = " << vfound.X() << " " << vfound.Y() << " " << vfound.Z() << endl;    cout << "Difference true - found (cm) = " << dif.Mag() << " " << dif.X() << " " << dif.Y() << " " << dif.Z() << endl;
    
    if(V){
      Double_t pos[3];
      for(Int_t kk=0;kk<3;kk++)pos[kk]=(Double_t)primaryVertex[kk];
      V->SetTruePos(pos);
    }
    timer.Stop();
    timer.Print();
    
    vertexer->WriteCurrentVertex(); 
  } 
  

}

