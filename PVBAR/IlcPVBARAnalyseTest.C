#include <iostream>

#include "IlcPVBARGetter.h"

void Go(){
  cout << "IlcPVBAR:> Single File default reconstruction analysing" << endl ;
  IlcPVBARGetter* gime=IlcPVBARGetter::GetInstance("gilc.root") ;  
  gime->Event(0,"SDR") ;
  if((gime->EmcRecPoints()==0)||(gime->EmcRecPoints()->At(0)==0)){
    cout << "        No EmcRecPoint  !!!!! " << endl ;
  }    
  if((gime->CpvRecPoints()==0)||(gime->CpvRecPoints()->At(0)==0)){
    cout << "        No CpvRecPoint  !!!!! " << endl ;
  }    
  if((gime->TrackSegments()==0)||(gime->TrackSegments()->At(0)==0)){
    cout << "        No TrackSegments !!!! " << endl ;
  }    
  if((gime->RecParticles()==0)||(gime->RecParticles()->At(0)==0)){
    cout << "        No RecParticles !!!!! " << endl ;
  }    
  cout << "IlcPVBAR:> Single File default analysing finished" << endl ;
  
  
  cout << "IlcPVBAR:> Single File branch TEST analyzing started" << endl ;
  gime=IlcPVBARGetter::GetInstance("gilc.root","test") ;  
  gime->Event(0,"SDR") ;
  if((gime->EmcRecPoints()==0)||(gime->EmcRecPoints()->At(0)==0)){
    cout << "No EmcRecPoint " << endl ;
  }    
  if((gime->CpvRecPoints()==0)||(gime->CpvRecPoints()->At(0)==0)){
    cout << "No CpvRecPoint " << endl ;
  }    
  if((gime->TrackSegments()==0)||(gime->TrackSegments()->At(0)==0)){
    cout << "No TrackSegments " << endl ;
  }    
  if((gime->RecParticles()==0)||(gime->RecParticles()->At(0)==0)){
    cout << "No RecParticles " << endl ;
  }    
  cout << "IlcPVBAR:> Single File branch TEST reconstruction ended" << endl ;
  
  
  cout << "IlcPVBAR:> Split File default reconstruction started" << endl ;
  gime=IlcPVBARGetter::GetInstance("gilc.root","Default",kTRUE) ;  
  gime->Event(0,"SDR") ;
  if((gime->EmcRecPoints()==0)||(gime->EmcRecPoints()->At(0)==0)){
    cout << "No EmcRecPoint " << endl ;
  }    
  if((gime->CpvRecPoints()==0)||(gime->CpvRecPoints()->At(0)==0)){
    cout << "No CpvRecPoint " << endl ;
  }    
  if((gime->TrackSegments()==0)||(gime->TrackSegments()->At(0)==0)){
    cout << "No TrackSegments " << endl ;
  }    
  if((gime->RecParticles()==0)||(gime->RecParticles()->At(0)==0)){
    cout << "No RecParticles " << endl ;
  }    
  cout << "IlcPVBAR:> Split File default reconstruction ended" << endl ;
  cout << "--------IlcPVBAR:> Reconstruction OK------------------"<< endl ;
  
}
