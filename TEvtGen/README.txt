//------------------------------------------------------------------------------//
   EvtGen is a particle decay simulator specifically designed for the needs of 
   B-physics studies. Complete manual of EvtGen is available at address:               
   http://robbep.home.cern.ch/robbep/EvtGen/GuideEvtGen.pdf                    
//------------------------------------------------------------------------------//

[14/12/2009] 
 1)This module contains: 

   - EvtGen original code organized in three directories:
     EvtGenBase-EvtGenModels-EvtGen. EvtGenModels directory contains all models  
     avilable in EvtGen to decay particles.
 
   - The interface classes IlcGenEvtGen and IlcDecayerEvtGen for IlcRoot users:
     the methods to decay particles are in IlcDecayerEvtGen which represents 
     the implementation of IlcDecayer using EvtGen package

 2)EvtGen required libphotos.so to generate Final State Radiation. Photos code is
   located in PHOTOS directory.  

 3)The implemented cases to force beauty hadrons interest those channels: 
   B->J/psi+X - B->J/psi+X,J/psi->e-e+ - B->J/psi+X,J/psi->mu-mu+ - B->e+X.
   Those are decayed by exclusive models of EvtGen, and all parameters and
   models used to decay them are not modified with respect to the official 
   EvtGen release. Some of particles produced in the decay chain (i.e. Lamda_c
   from Lambda_b, Xi_c0 from Xi_b-, etc...) are decayed by Pythia with the 
   configuration setting in the IlcDecayerPythia.

 4)Polarization is still to be implemented in the interface classes.
   It would be done soon.

 5)To use EvtGen to decay beauty particles adjust those lines in the Config.C:

 //----- load libraries (after loading libpythia6.so) 
  gSystem->Load("libphotos.so");
  gSystem->Load("libEvtGenBase");
  gSystem->Load("libEvtGenModels");
  gSystem->Load("libEvtGen");
  gSystem->Load("libTEvtGen");

 //----- declare an IlcGenCocktail 
  IlcGenCocktail *generCock=new IlcGenCocktail();
  generCock->UsePerEventRates();

 //----- declare Pythia configuration: switch-off beauty decays in Pythia
  IlcGenPythia *pythia= .....
  .... 
  pythia->SetForceDecay(kNoDecayBeauty);
 
 //----- declare EvtGen configuration and put the two generators in the cocktail
  IlcGenEvtGen *gene = new IlcGenEvtGen();
  gene->SetForceDecay(kBJpsiDiElectron);
  gene->SetParticleSwitchedOff(IlcGenEvtGen::kBeautyPart);
  generCock->AddGenerator(pythia, "Pythia", 1.);
  generCock->AddGenerator(gene,"gene",1.);
