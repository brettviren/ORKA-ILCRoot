void rootlogon() {
   printf("\nWELCOME to ILCroot for ORKA\n\n");
//    printf("\nYou are using ILCroot $Revision$\n\n");
   TInterpreter * mycint = gROOT->GetInterpreter();
   mycint->AddIncludePath("$ILC_ROOT/include");
}

