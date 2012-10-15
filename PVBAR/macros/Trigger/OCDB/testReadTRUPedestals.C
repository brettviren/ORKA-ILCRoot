void testReadTRUPedestals()
{
  gROOT->LoadMacro("IlcPVBARTriggerParameters.cxx+g");
  gROOT->LoadMacro("readTRUPedestals.C+g");

  IlcPVBARTriggerParameters* parameters = new IlcPVBARTriggerParameters("PVBARTParametersDat");

  readTRUPedestals(parameters);
  drawTRUPedestals(parameters);

  parameters->SaveAs("PVBARTParams.root");
}
  
