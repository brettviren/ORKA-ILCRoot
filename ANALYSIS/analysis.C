void analysis(Int_t first = -1, Int_t last = -1, const char* directory=".")
{

  gSystem->Load("$(ILC_ROOT)/lib/tgt_$(ILC_TARGET)/libANALYSIS");

  IlcVAODParticle::SetDebug(0);
  IlcRunAnalysis* analysis = new IlcRunAnalysis();
  
  ::Info("analysis.C","Setting dirs");
  TObjArray* dirs=0;
  if ( ((first >= 0) && (last >= 0) ) && ( (last-first)>=0 ) )
  {//read from many dirs dirs
    char buff[50];
    dirs = new TObjArray(last-first+1);
    for (Int_t i = first; i<=last; i++)
     {
//     sprintf(buff,"%s/%s/%s/%05.5d",basedir,field,serie,i);
       printf("%s/%d\n",directory,i);
       sprintf(buff,"%s/%d",directory,i);
       TObjString *odir= new TObjString(buff);
       dirs->Add(odir);
     }
   }

  IlcReaderAOD* reader = new IlcReaderAOD("AOD.root");
  reader->SetDirs(dirs);
//  IlcReaderESD* reader = new IlcReaderESD(dirs);
//  reader->ReadSimulatedData(kTRUE);
//  reader->SetReadMostProbableOnly(kTRUE);

/*  
   //example PID cuts
  IlcAODParticleCut* partcut = new  IlcAODParticleCut();
  partcut->SetPID(kPiPlus);//here we define the incarnation 
  IlcAODPIDCut* pidcut = new IlcAODPIDCut(kPiPlus,0.5);//accept all particles types that have PID prob > 50%
  partcut->AddBasePartCut(pidcut);//
  reader->AddParticleCut(partcut);//This guy makes a copy of a cut for himself so we can modify it here
  
  partcut->SetPID(kPiMinus);//here we define that particle has incarnation PiMinus
  pidcut->SetPID(kPiMinus);//here we define to check if PID probability of being kPiMinus is greater thann 0.5 (number defined few lines above)
  reader->AddParticleCut(partcut);
  
  pidcut->SetPID(kKPlus);
  pidcut->SetPID(kKPlus);
  reader->AddParticleCut(partcut);
  
  pidcut->SetPID(kKMinus);
  pidcut->SetPID(kKMinus);
  reader->AddParticleCut(partcut);
*/  
  
  IlcFlowAnalysis* flow = new IlcFlowAnalysis();
  analysis->SetReader(reader);
  
  analysis->Add(flow);
  analysis->Run();
  delete analysis;
}
