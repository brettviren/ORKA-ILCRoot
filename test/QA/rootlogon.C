{
  printf("\nWELCOME to ILC QA\n\n"); 
  gSystem->SetIncludePath("-I$ROOTSYS/include -I$ILC_ROOT/include -I$ILC_ROOT/TRD -I$ILC_ROOT/RAW") ; 
  //load library neccesary for TPC alone 
//  gSystem->Load("$(ROOTSYS)/lib/libEG");
//  gSystem->Load("$(ROOTSYS)/lib/libGeom");  
//  gSystem->Load("$(ROOTSYS)/lib/libVMC");
  //gSystem->Load("$(ILC_ROOT)/lib/tgt_$(ILC_TARGET)/libTRACKING");
  
  
}

