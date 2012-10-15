void PetiteMacroDeNuit()
{
Int_t evt = 0 ; 
RecAna * t = new RecAna("junk.root"); 
t->GetEvent(evt);   
TObjArray * lp = t->PVBARPpsdRP ; 
 cout << "Tree macro = " << lp << endl ; 
for (int i = 0 ; i < lp->GetEntries() ; i++ ) {
  IlcPVBARPpsdRecPoint * rpp = (IlcPVBARPpsdRecPoint *)lp->At(i)   ; 
  rpp.Print(); 
}
TObjArray * le = t->PVBAREmcRP ; 
for (int i = 0 ; i < le->GetEntries() ; i++ ) {
  IlcPVBAREmcRecPoint * rp = (IlcPVBAREmcRecPoint *)le->At(i)   ; 
  rp->Print(); 
}

IlcPVBARIndexToObject * please = IlcPVBARIndexToObject::GetInstance() ;

for (int i = 0 ; i < (t->PVBARTS_-1) ; i++) {
  cout << "TrackSegment # " << i << endl 
       << "====================" << endl ; 
  int index = t->PVBARTS_fEmcRecPoint[i] ;
  IlcPVBAREmcRecPoint * emrp = (IlcPVBAREmcRecPoint *) ( please->GimeRecPoint(index, TString("emc") ) ) ; 
  emrp->Print() ; 
  index = t->PVBARTS_fPpsdLowRecPoint[i] ;
  IlcPVBARPpsdRecPoint * ppsdl = (IlcPVBARPpsdRecPoint *) ( please->GimeRecPoint(index, TString("ppsd") ) ) ; 
  if (ppsdl)
    ppsdl->Print() ; 
  index = t->PVBARTS_fPpsdUpRecPoint[i] ;
  IlcPVBARPpsdRecPoint * ppsdu = (IlcPVBARPpsdRecPoint *) ( please->GimeRecPoint(index, TString("ppsd") ) ) ; 
  if (ppsdu)
    ppsdu->Print() ; 
}
for (int i = 0 ; i < (t->PVBARRP_-1) ; i++) {
  cout << "RecParticles # " << i << endl 
       << "====================" << endl ; 
  cout << "type = " << t->PVBARRP_fType[i] << " energy = " << t->PVBARRP_fE[i] << endl ;
}  
delete t ; 
}
