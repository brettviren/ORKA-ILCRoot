ReadTrigger( TString inFile = "gilc.root" )
{
   // Dynamically link some shared libs
   if( gClassTable->GetID( "IlcRun" ) < 0 ) {
      gROOT->ProcessLine( ".x $(ILC_ROOT)/macros/loadlibs.C" );
   } else if( gIlc ) {
      delete IlcRunLoader::Instance();
      delete gIlc;
      gIlc=0;
   }

   IlcRunLoader* rl = IlcRunLoader::Open( inFile.Data() );
   if( rl == 0x0 ) {
      cerr << "ReadTrigger.C : Can not open session RunLoader=NULL"
           << endl;
       return 3;
   }

   // Read and Print Trigger

   rl->LoadTrigger();
   IlcCentralTrigger *aCTP = rl->GetTrigger();
   aCTP->Print();

   // Loop over event and print trigger info
   Int_t nevent = rl->GetNumberOfEvents();
   for( Int_t i=0; i<nevent; i++ ) {
      rl->GetEvent( i );
      cout << endl << "Event " << i
           << " Global Trigger Class Mask: 0x" << hex << aCTP->GetClassMask() << endl;

      // Read trigger inputs from detector. Example. ITS
      IlcLoader * loader = rl->GetDetectorLoader( "ITS" );
      if( loader ) {
         IlcDataLoader * dataLoader = loader->GetDigitsDataLoader();
         if( !dataLoader->IsFileOpen() ) 
            dataLoader->OpenFile( "READ" );
         IlcTriggerDetector* trgdet = (IlcTriggerDetector*)dataLoader->GetDirectory()->Get( "Trigger" );
         if( trgdet ) {
            trgdet->Print();
         } else {
            cerr << "There is not trigger object for " << loader->GetName() << endl;
         }
      }
   }
}
