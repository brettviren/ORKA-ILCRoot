void IlcPVBARCalibHistoProducer(const char* file="2006run2211.root") 
{
  // Script is to be run at DAQ computers (LDC, GDC, HLT or MOOD);
  // it fills the histograms with amplitudes per channel.
  // These histograms will be processed in the Shuttle preprocessor
  // e.g., to extract mean amplitudes needed for relative channel calibration
  //
  // This example assumes that the input data is supplied from the
  // raw data file in the ROOT format.
  //
  // Author: Boris Polichtchouk, 4 October 2006

  // Load PVBAR shuttle library as it is not linked to ilcroot
  gSystem->Load("libPVBARshuttle");

  IlcRawReaderRoot* rf = new IlcRawReaderRoot(file);
  IlcPVBARCalibHistoProducer hp(rf);
  hp.Run();
}
