void mergeHisto(char *Signal="OccupanzaSeg.root",char *Background="OccupanzaBck.root")
{
//Load the file of histogram of the occupancy of DCH produced by OccDch_new1_4
//do the Sum of the 2 original file


TFile *f=new TFile("merge.root","RECREATE"); //new Output File

TH1F *merge=new TH1F("merge","Drift Chamber Occupancy 3",240,1,240);//new Histogram File
TH1F *merge2=new TH1F("merge2","Drift Chamber Occupancy 4",240,1.,240.); 

TFile *segnale=TFile::Open(Signal); //Signal File input
TH1F *HistMerge;  
TH1F *HistMerge2; 

HistMerge= (TH1F*)  gROOT->FindObject("Occ 3");

	if (HistMerge)    HistMerge = (TH1F*) segnale->Get("Occ 3");
	else  if (!HistMerge2){cout <<"Error: Signal Histogram doesn't exist!!!\n"<<endl;
				cout <<"Error: I can't do the merge"<<endl;continue;}
				
HistMerge2= (TH1F*)  gROOT->FindObject("Occ 4");

	if (HistMerge2)    HistMerge2 = (TH1F*) segnale->Get("Occ 4");
	else  if (!HistMerge2){cout <<"Error: Signal Histogram doesn't exist!!!\n"<<endl;
				cout <<"Error: I can't do the merge"<<endl;continue;}
				

		merge->Add(HistMerge);//add the object from Signal histogram
		merge2->Add(HistMerge2);//add the object from Signal histogram


	segnale->Close();
		cout<<"Add Signal, OK!"<<endl;

TFile *background=TFile::Open(Background); //Background File input

HistMerge= (TH1F*)  gROOT->FindObject("Occ 3"); 

	if (HistMerge)    HistMerge = (TH1F*) background->Get("Occ 3");
	else  if (!HistMerge2){cout <<"Error: Background Histogram doesn't exist!!!\n"<<endl;
				cout <<"Error: I can't do the merge"<<endl;continue;}

HistMerge2= (TH1F*)  gROOT->FindObject("Occ 4");

	if (HistMerge2)    HistMerge2 = (TH1F*) background->Get("Occ 4");
	else  if (!HistMerge2){cout <<"Error: Background Histogram doesn't exist!!!\n"<<endl;
				cout <<"Error: I can't do the merge"<<endl;continue;}

		merge->Add(HistMerge);//add the object from Background histogram
		merge2->Add(HistMerge2);//add the object from Background histogram

	background->Close();

		cout<<"Add Background, OK!"<<endl;
		
TCanvas *Occ1= new TCanvas();
Occ1->cd(1);
merge->SetXTitle("rings");
merge->SetYTitle("cell %");
merge->Draw();
TCanvas *Occ2= new TCanvas();
Occ2->cd(1);
merge2->SetXTitle("rings");
merge2->SetYTitle("cell molteplicity");
merge2->Draw();

f->cd();
merge->Write();
f->Write();
cout<<"Merging Finished!"<<endl;
return;
}
