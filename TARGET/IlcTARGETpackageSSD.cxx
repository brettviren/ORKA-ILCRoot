/**************************************************************************
 * Copyright(c) 2005-2006, ILC Project Experiment, All rights reserved.   *
 *                                                                        *
// Author: The ILC Off-line Project. 
 // Part of the code has been developed by Alice Off-line Project. 
 * Contributors are mentioned in the code where appropriate.              *
 *                                                                        *
 * Permission to use, copy, modify and distribute this software and its   *
 * documentation strictly for non-commercial purposes is hereby granted   *
 * without fee, provided that the above copyright notice appears in all   *
 * copies and that both the copyright notice and this permission notice   *
 * appear in the supporting documentation. The authors make no claims     *
 * about the suitability of this software for any purpose. It is          *
 * provided "as is" without express or implied warranty.                  *
 **************************************************************************/
/* $Id: IlcTARGETpackageSSD.cxx,v 1.1.1.1 2008/03/11 14:52:50 vitomeg Exp $ */

#include <Riostream.h>
#include <TClonesArray.h>
#include "IlcTARGETpackageSSD.h"
#include "IlcTARGETclusterSSD.h"

const Bool_t IlcTARGETpackageSSD::fgkSIDEP=kTRUE;
const Bool_t IlcTARGETpackageSSD::fgkSIDEN=kFALSE;

ClassImp(IlcTARGETpackageSSD)
////////////////////////////////////////////////////////////////////////////
//Class describing set of IlcTARGEToneSideClusterSSDs, which contact each other.
//Piotr Krzysztof Skowronski
//Warsaw University of Technology
//skowron@if.pw.edu.pl
//
//--------------------------------------------------------------------------
IlcTARGETpackageSSD::IlcTARGETpackageSSD()
{
  // constructor
  fNclustersN=0;
  fClusterNIndexes = 0; 
		
  fNclustersP=0;
  fClusterPIndexes = 0;
  if (fgkDebug) cout<<"Default Ctor was used\n>>>>>>>>>>>>>><<<<<<<<<<<<<";
}


/*******************************************************/

IlcTARGETpackageSSD::IlcTARGETpackageSSD
  (TClonesArray *clustersP, TClonesArray *clustersN)
{
  // constructor
  fClustersP=clustersP;
  fClustersN=clustersN;
  	
  fNclustersN=0;
  fClusterNIndexes = new TArrayI(300); 
	
  fNclustersP=0;
  fClusterPIndexes = new TArrayI(300);		
}

/*******************************************************/


IlcTARGETpackageSSD::IlcTARGETpackageSSD
  ( Int_t len, TClonesArray *clustersP, TClonesArray *clustersN)
{	
  // constructor
  fClustersP=clustersP;
  fClustersN=clustersN;

  fNclustersN=0;
  fClusterNIndexes = new TArrayI(len); 
	
  fNclustersP=0;
  fClusterPIndexes = new TArrayI(len);		
}


/*******************************************************/

IlcTARGETpackageSSD::~IlcTARGETpackageSSD()
{
  // destructor
  delete fClusterNIndexes;
  delete fClusterPIndexes;		
}

/*******************************************************/

IlcTARGETpackageSSD::IlcTARGETpackageSSD(const IlcTARGETpackageSSD &package) : 
    TObject(package){
  // copy constractor
  Int_t i;  //iterator
 
  if (this == &package) return;
  fClustersN = package.fClustersN;
  fClustersP = package.fClustersP;
  
  fNclustersN= package.fNclustersN;
  fNclustersP= package.fNclustersP;
  
  for ( i =0; i<fNclustersN;i++)
    {
      fClusterNIndexes[i]= package.fClusterNIndexes[i]; 
    }
  
  for ( i =0; i<fNclustersP;i++)
    {
      fClusterPIndexes[i]= package.fClusterPIndexes[i]; 
    }
  
  if (fgkDebug) cout << "Copying function was used\n<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>";
  
  return; 
  
}
/*******************************************************/

IlcTARGETpackageSSD&  
IlcTARGETpackageSSD::operator=( const IlcTARGETpackageSSD & package)
{
  //  assignment operator
  //
  Int_t i;  //iterator
  
  if (this == &package) return *this;
  fClustersN = package.fClustersN;
  fClustersP = package.fClustersP;
  
  fNclustersN= package.fNclustersN;
  fNclustersP= package.fNclustersP;
  
  for ( i =0; i<fNclustersN;i++)
    {
      fClusterNIndexes[i]= package.fClusterNIndexes[i]; 
    }
  
  for ( i =0; i<fNclustersP;i++)
    {
      fClusterPIndexes[i]= package.fClusterPIndexes[i]; 
    }
  
  if (fgkDebug) cout << "Copying function was used\n<<<<<<<<<<<<<<<<<>>>>>>>>>>>>>>>>";
  
  return *this; 
  
}

/*******************************************************/

Int_t IlcTARGETpackageSSD::GetNSideClusterIdx(Int_t index) const
{
  // get N-side cluster
  // 
  if ((index>-1)&&(index<fNclustersN))
    return (*fClusterNIndexes)[index];
  else 
    {
      cout << "IlcTARGETpackageSSD::GetNSideClusterIdx  : Out of Range\n";
      return -1;
    }
}
/*******************************************************/


Int_t IlcTARGETpackageSSD::GetPSideClusterIdx(Int_t index) const
{
  // get P-side cluster
  //
  if ((index>-1)&&(index<fNclustersP))
    return (*fClusterPIndexes)[index];
  else 
    {
      cout << "IlcTARGETpackageSSD::GetPSideClusterIdx  : Out of Range\n";
      return -1;
    } 
}
/*******************************************************/
IlcTARGETclusterSSD*  
IlcTARGETpackageSSD::GetPSideCluster(Int_t index)
{
  // get Pside cluster from the TClonesArray of SSD clusters
  //
  return (IlcTARGETclusterSSD*)((*fClustersP)[GetPSideClusterIdx(index)]);
}

/*******************************************************/

IlcTARGETclusterSSD*  
IlcTARGETpackageSSD::GetNSideCluster(Int_t index)
{
  // get Nside cluster from the TClonesArray of SSD clusters
  //
  return (IlcTARGETclusterSSD*)((*fClustersN)[GetNSideClusterIdx(index)]);
}


/*******************************************************/

Bool_t IlcTARGETpackageSSD::GetClusterWithOneCross
(Int_t & index, Bool_t& side)
{
  // select clusters with on cross 
  //
  if((fNclustersP==0)||(fNclustersN==0) )
    {
      printf("Empty package ((fNclustersP==0)||(fNclustersN==0))\n");
      index = -2;
      return kFALSE;
    } 
  Int_t ind;
  
  ind =(*fClusterPIndexes)[fNclustersP-1]; 
  if (  ( ((IlcTARGETclusterSSD*)(*fClustersP)[ind]  )->GetCrossNo()  ) ==1 )
    {
      //index=ind;
      index =fNclustersP-1; 
      side=fgkSIDEP;
      return kTRUE;
    }
  
  ind =(*fClusterNIndexes)[fNclustersN-1]; 
  if (  (  ((IlcTARGETclusterSSD*)(*fClustersN)[ind]  )->GetCrossNo() ) ==1  )
    {
      //index=ind;
      index = fNclustersN-1;
      side=fgkSIDEN;
      return kTRUE;
    }
  
  
  ind =(*fClusterPIndexes)[0];
  if (  ( ((IlcTARGETclusterSSD*)(*fClustersP)[ind]  )->GetCrossNo() ) ==1 )
    {
      //index=ind;
      index = 0;
      side=fgkSIDEP;
      return kTRUE;
    }
  
  
  ind =(*fClusterNIndexes)[0];
  if (  ( ((IlcTARGETclusterSSD*)(*fClustersN)[ind]  )->GetCrossNo()  ) ==1  )
    {
      //    index=ind;
      index = 0;  
      side=fgkSIDEN;
      return kTRUE;
    }
  
  
  //Add for to be shure 
  index = -1;
  return kFALSE;
  
}
/*******************************************************/

void IlcTARGETpackageSSD::DelCluster(Int_t index, Bool_t side)
{
  // call DelPCluster or DelNCluster depending on side
  //
  if(side==fgkSIDEP) DelPCluster(index); else DelNCluster(index);
}
/*******************************************************/
void IlcTARGETpackageSSD::DelPCluster(Int_t index)
{
  //it not deletes delete given cluster physically, 
  //but only complytely erase it from package
  //all clusters are deleted automatically when TClonesArray is deleted
  
  Int_t i;
  Int_t idx;
  Int_t clToDelIdx = GetPSideClusterIdx(index); //Index of cluster in TClonesArray 
  IlcTARGETclusterSSD *clToDel = GetPSideCluster(index); //cluster to be deleted
  Int_t ncr = clToDel->GetCrossNo();
  
  for (i =0;i<ncr;i++)
    {
      idx = clToDel->GetCross(i);
      ((IlcTARGETclusterSSD *)((*fClustersN)[idx])   )->DelCross(clToDelIdx);
    }
  
  
  for (i=index;i<fNclustersP-1;i++)
    {
      (*fClusterPIndexes)[i]=(*fClusterPIndexes)[i+1];
    }
  fNclustersP--; 
  if (fgkDebug) cout<<"Cluster P ("<<index<<") deleted\n";
  
  
  for (i=0;i<fNclustersN;i++)
    {
      if ( (GetNSideCluster(i)->GetCrossNo())==0) DelNCluster(i);
    }
}



/*******************************************************/
void IlcTARGETpackageSSD::DelNCluster(Int_t index)
{
  //it not deletes delete given cluster physically, 
  //but only complytely erase it from package
  //all clusters are deleted automatically when TClonesArray is deleted
  
  Int_t i;
  Int_t idx;
  Int_t clToDelIdx = GetNSideClusterIdx(index); //Index of cluster in TClonesArray 
  IlcTARGETclusterSSD *clToDel = GetNSideCluster(index); //cluster to be deleted
  Int_t ncr = clToDel->GetCrossNo();
  
  for (i =0;i<ncr;i++)
    {
      idx = clToDel->GetCross(i);
      ((IlcTARGETclusterSSD *)((*fClustersP)[idx])   )->DelCross(clToDelIdx);
    }
  
  
  for (i=index;i<fNclustersN-1;i++)
    {
      (*fClusterNIndexes)[i]=(*fClusterNIndexes)[i+1];
    }
  fNclustersN--; 
  if (fgkDebug) cout<<"Cluster N ("<<index<<") deleted\n";
  
  for (i=0;i<fNclustersP;i++)
    {
      if ( (GetPSideCluster(i)->GetCrossNo())==0) DelPCluster(i);
    }
  
}


/*******************************************************/

void IlcTARGETpackageSSD::DelPClusterOI(Int_t index)
{
  //This function looks like this, 
//because probably cut cluster is 
//on the beginning or on the end of package 
 Int_t i;
 if( ((*fClusterPIndexes)[0]) == index) 
  {
    DelPCluster(0);
    return;
  }
 else
  { 
   if( ((*fClusterPIndexes)[fNclustersP-1]) ==index)
    {
      DelPCluster(fNclustersP-1);
      return;
    }
   else
    {
     for (i=1;i<fNclustersP-1;i++)
       {
         if( ((*fClusterPIndexes)[i])==index)
	  {
            DelPCluster(i);
            return;
	  }
       }
    }
  }
 
 cout<<"IlcTARGETpackageSSD - DelPClusterOI: index "<<index<<" not found\n";

}


/*******************************************************/

void IlcTARGETpackageSSD::DelNClusterOI(Int_t index)
{
//This function looks like this, 
//because probably cluster to cut is 
//on the beginning or on the end of package 

 Int_t i;
 if( ((*fClusterNIndexes)[0])==index) 
  {
    DelNCluster(0);
    return;
  }
 else
  { 
   if( ((*fClusterNIndexes)[fNclustersN-1])==index)
    {
      DelNCluster(fNclustersN-1);
      return;
    }
   else
    {
     for (i=1;i<fNclustersN-1;i++)
       {
         if( ((*fClusterNIndexes)[i])==index)
         {
          DelNCluster(i);
          return;
         }
       }
    }
  }
 cout<<"IlcTARGETpackageSSD - DelNClusterOI: index "<<index<<" not found\n";
}


/*******************************************************/


void IlcTARGETpackageSSD::DelClusterOI(Int_t index, Bool_t side)
{
  // delete cluster
 if (side == fgkSIDEP)
  {    
    DelPClusterOI(index);
  } 
  else
  {
    DelNClusterOI(index);
  }

}


/**********************************************/


void  IlcTARGETpackageSSD::GetAllCombinations(Int_t**array,Int_t &num,Int_t sizet)
{
  // get all combinations
  Int_t *takenNcl = new Int_t[fNclustersN];
  
  num=0;
  
  if (fgkDebug) PrintClusters();

  for (Int_t i=0;i<fNclustersP;i++)
   {
     takenNcl[i]=-1;
   }
  //see comment on the beginning of MakeCombin
  if (fgkDebug) cout<<"GetAllCombinations entered";
  MakeCombin (array,num,0,takenNcl,sizet);

  delete []takenNcl; 
}
/**********************************************/


void  IlcTARGETpackageSSD::MakeCombin
   (Int_t**arr,Int_t& nu, Int_t np, Int_t *occup, Int_t sizet)

{
//ATTENTION: anybody watching this function
//IlcTARGETclusterSSD::GetCrossNo() returns index of cluster in main array belonging to IlcTARGETmodulesSSD
//however, we have pointer to that array (TClonesArray)
//we can not use 
//Get?SideCluster because it takes index from local look_up_table
 
 Int_t i,j;
 
 //this cluster
 IlcTARGETclusterSSD *cl=GetPSideCluster(np);

 Int_t nc = cl->GetCrossNo();  //number of crosses for this cluster
 Int_t indcro;                 //index of given cluster on side N that 
                               // this cluster crosses with
   
 if (np == fNclustersP-1) {
   for (i=0;i<nc;i++) {
     indcro=cl->GetCross(i);
     if(IsFree(indcro,np,occup)) {
        occup[np]=indcro;
        for(j=0;j<fNclustersP;j++)  
        {
	   if (nu<sizet) arr[nu][j]=occup[j];
	   else {
	       continue;}
        }
      
        occup[np]=-1;
        if (nu<sizet-1) nu++;
     }
   }
  } else {
    for (i=0;i<nc;i++) {
       indcro=cl->GetCross(i);
       if(IsFree(indcro,np,occup)) {
      	  occup[np]=indcro;
	  if (nu<sizet) MakeCombin(arr,nu,(np+1),occup,sizet);
	  //else printf("MakeComb - exceeding array size!\n");
       }
    }
    occup[np]=-1;
  } 

}

/**********************************************/
Bool_t  IlcTARGETpackageSSD::IsFree(Int_t idx, Int_t nn, const Int_t *lis) const
{
  // 
  for (Int_t i=0;i<nn;i++)
    {
      if (lis[i]==idx) return kFALSE;
    }
  return kTRUE;
}

/**********************************************/
void IlcTARGETpackageSSD::PrintClusters()
{
  // print cluster info
Int_t i,j;
cout<<"SIDE P\n";
for (i=0;i<fNclustersP;i++)
 {
   cout<<i<<".  IO="<<GetPSideClusterIdx(i)<<" NC="<<GetPSideCluster(i)->GetCrossNo()<<"     C. IDXs : ";
   for (j=0;j<GetPSideCluster(i)->GetCrossNo();j++)
    {
      cout<<GetPSideCluster(i)->GetCross(j)<<" ";
    }
 //  if (GetPSideCluster(i)->GetSide()) cout<<" P\n";
 //  else cout<<"BAD SIDE ==N\n";
    cout<<"\n";
   
 }

cout <<"SIDE N\n";
for (i=0;i<fNclustersN;i++)
 {
   cout<<i<<".  IO="<<GetNSideClusterIdx(i)<<" NC="<<GetNSideCluster(i)->GetCrossNo()<<"     C. IDXs : ";
   for (j=0;j<GetNSideCluster(i)->GetCrossNo();j++)
    {
      cout<<GetNSideCluster(i)->GetCross(j)<<" ";
    }
 //  if (GetNSideCluster(i)->GetSide()) cout<<" N\n";
 //  else cout<<"BAD SIDE ==P\n";
    cout<<"\n";   
 }
 
}

/**********************************************/
void IlcTARGETpackageSSD::ConsumeClusters()
{
  // consume cluster
  register Int_t i;
  
  for(i=0;i<fNclustersP;i++)
    {
      GetPSideCluster(i)->Consume();
    }
  
  for(i=0;i<fNclustersN;i++)
    {
      GetNSideCluster(i)->Consume();
    }
  
}

/**********************************************/

Int_t IlcTARGETpackageSSD::GetNextPIdx(Int_t OI) const
{
 //Returns index of next P cluster OI in package; OI == Original Inedx (in TClonesArray)
 //if not egsist return -1;
 for (Int_t i =0; i<fNclustersP-1;i++)
  {
    if(GetPSideClusterIdx(i) == OI)
       return GetPSideClusterIdx(i+1);
  }
 return -1;
}

/**********************************************/
Int_t IlcTARGETpackageSSD::GetPrvPIdx(Int_t OI) const
{
 //Returns index of previous P cluster  OI in package; OI == Original Inedx (in TClonesArray)
 //if not egsist return -1;
 
 for (Int_t i =1; i<fNclustersP;i++)
  {
    if(GetPSideClusterIdx(i) == OI)
       return GetPSideClusterIdx(i-1);
  }
  return -1;
}
/**********************************************/
Int_t IlcTARGETpackageSSD::GetNextNIdx(Int_t OI) const
{
//Returns index of next N cluster OI in package; OI == Original Inedx (in TClonesArray)
 //if not egsist return -1;
 for (Int_t i =0; i<fNclustersN-1;i++)
  {
    if(GetNSideClusterIdx(i) == OI)
       return GetNSideClusterIdx(i+1);
  }
 return -1;

}
/**********************************************/
Int_t  IlcTARGETpackageSSD::GetPrvNIdx(Int_t OI) const
{
 //Returns index of previous N cluster OI in package; OI == Original Inedx (in TClonesArray)
 //if not egsist return -1;
 
 for (Int_t i =1; i<fNclustersN;i++)
  {
    if(GetNSideClusterIdx(i) == OI)
       return GetNSideClusterIdx(i-1);
  }
  return -1;
 
}

void  IlcTARGETpackageSSD::SplitPackage(Int_t pi, Int_t ni, IlcTARGETpackageSSD* pkg)
{
  // split package of clusters
  Int_t p=-1, n=-1;
  Int_t i;
  for (i=0;i<fNclustersN;i++)
   {
     if((*fClusterNIndexes)[i]==ni) 
      {
        n = i;
	break;
      }
    } 
   
  for (i=0;i<fNclustersP;i++)
   {
     if((*fClusterPIndexes)[i]==pi) 
      {
        p = i;
	break;
      }
    }  
  if (fgkDebug) {
    cout<<" p = "<<p<<"  n = "<<n;
  }
  if ((p==-1)||(n==-1)) return;
  
  for (i=p;i<fNclustersP;i++)
   {
     pkg->AddPSideCluster(GetPSideClusterIdx(i));       
   }
  fNclustersP = p;
  
  for (i=n;i<fNclustersN;i++)
   {
     pkg->AddNSideCluster(GetNSideClusterIdx(i));       
   }
  fNclustersN = n;
 
  cout<<"  After split: fNclustersP = "<< fNclustersP<< "fNclustersN = "<< fNclustersN<<"\n";
}
