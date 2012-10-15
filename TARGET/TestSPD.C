Int_t TestSPD(Int_t n = 10,Char_t SlowOrFast='s'){
   
   IlcLoader::SetDebug(kTRUE);//set it to kTRUE for debug print-out
   gIlc->SetDebug(100);
   /**********************************************/
   /************ G E N E R A T I O N *************/
   /**********************************************/
      
   gROOT->LoadMacro("$(ILC_ROOT)/macros/grun.C");
   grun(n,"$(ILC_ROOT)/TARGET/ConfigSPD02.C");

  
   /**********************************************/
   /******************* I T S ********************/
   /**********************************************/
   //The following part is just core of IlcTARGETtestV2.C
   if (SlowOrFast=='f') {
      cerr<<"Fast IlcTARGETRecPoint(s) !\n";
      gROOT->LoadMacro("$(ILC_ROOT)/TARGET/IlcTARGETHits2FastRecPoints.C");
      IlcTARGETHits2FastRecPoints();
   } else {
      cerr<<"Slow IlcTARGETRecPoint(s) !\n";
      gROOT->LoadMacro("$(ILC_ROOT)/TARGET/IlcTARGETHits2SDigits.C");
      IlcTARGETHits2SDigits();
      gROOT->LoadMacro("$(ILC_ROOT)/TARGET/IlcTARGETSDigits2Digits.C");
      IlcTARGETSDigits2Digits();
      gROOT->LoadMacro("$(ILC_ROOT)/TARGET/IlcTARGETDigits2RecPoints.C");
      IlcTARGETDigits2RecPoints();
   }
        // The following may not work properly.
//   gROOT->LoadMacro("$(ILC_ROOT)/TARGET/IlcTARGETFindClustersV2.C");
//   if (rc=IlcTARGETFindClustersV2(SlowOrFast)) return rc;

//   gROOT->LoadMacro("$(ILC_ROOT)/TARGET/IlcTARGETFindTracksV2.C");
//   if (rc=IlcTARGETFindTracksV2()) return rc;

   ::Info("NewIO test","Everything seems to be OK");   
   ::Info("NewIO test","You can try now display.C");
}
