#if !defined(__CINT__) || defined(__MAKECINT__)
#include <iostream>

#include "TString.h"

#include "PVBAR/IlcPVBARReconstructor.h"
#endif

void Go(TString deb = ""){
  IlcPVBARReconstructor * a ;   

  cout << "IlcPVBAR:> Single File default reconstruction started" << endl ;
  a = new IlcPVBARReconstructor("gilc.root") ;  //first -single file default reconstruction
  a->ExecuteTask(deb.Data()) ;
  cout << "IlcPVBAR:> Single File default reconstruction finished" << endl ;
  // delete a ; 

  cout << "IlcPVBAR:> Single File branch TEST reconstruction started" << endl ;
  a = new IlcPVBARReconstructor("gilc.root","test") ;  //another branch single file recontruction
  a->ExecuteTask(deb.Data()) ;
  cout << "IlcPVBAR:> Single File branch TEST reconstruction ended" << endl ;
  //delete a ; 
  
  cout << "IlcPVBAR:> Split File default reconstruction started" << endl ;
  a = new IlcPVBARReconstructor("gilc.root","Default",kTRUE) ; //Split file default reconstruction
  a->ExecuteTask(deb.Data()) ;
  cout << "IlcPVBAR:> Split File default reconstruction ended" << endl ;
  //delete a ; 

  cout << "--------IlcPVBAR:> Reconstruction OK------------------"<< endl ;
}
