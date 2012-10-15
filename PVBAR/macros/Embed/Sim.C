void Sim(const Int_t nevents=10,
	 const Int_t debLevel=0)
{
  //make simulation of MC events and merging
  //with previouly reconstructed Digits from 
  //raw event. We assume they are in dir "Bg"

  IlcPVBARSimParam * PVBARSimParam = IlcPVBARSimParam::GetInstance() ;

  IlcLog::SetGlobalDebugLevel(debLevel);
  IlcSimulation sim;
  sim.SetMakeSDigits("PVBAR") ;
  sim.SetMakeDigits("PVBAR") ;
//  sim.SetMakeDigitsFromHits("ITS TPC");
//  sim.SetMakeTrigger("PVBAR");
  //Set path to reconstricted raw digits
  //and set number of simulated events per one raw
  sim.MergeWith("Bg/gilc.root",1) ;
  PVBARSimParam->SetStreamDigits(1) ; //Mark this stream as digits and not SDigits

  sim.Run(nevents);


}
