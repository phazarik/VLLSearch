#define AnaScript_cxx
#include "AnaScript.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
using namespace std;

//Common headers:
#include "/home/work/phazarik1/work/VLLSearch/Setup/CustomFunctions.h"
#include "/home/work/phazarik1/work/VLLSearch/Setup/EventSelection.h"
#include "/home/work/phazarik1/work/VLLSearch/Setup/ProduceGenCollection.h"
#include "/home/work/phazarik1/work/VLLSearch/Setup/ProduceRecoCollection.h"
#include "/home/work/phazarik1/work/VLLSearch/Setup/nlohmann/json_setup.h"

//Headers used by this particular setup:
#include "/home/work/phazarik1/work/VLLSearch/AnaScript/Skimmer/skimmerHelper.h"

void AnaScript::Begin(TTree * /*tree*/)
{
  TString option = GetOption();
}

void AnaScript::SlaveBegin(TTree *tree)
{
  TString option = GetOption();
  //---------------------------------------------------
  //Set Campaign manually : (important for corrections)
  //_campaign = "2018_UL";
  //_campaign = "2017_UL";
  //_campaign = "2016preVFP_UL";
  //_campaign = "2016postVFP_UL";
  //---------------------------------------------------

  time(&start);
  
  //Setting year:
  if     (_campaign == "2018_UL") _year = 2018;
  else if(_campaign == "2017_UL") _year = 2017;
  else if((_campaign == "2016preVFP_UL") || (_campaign == "2016postVFP_UL")) _year = 2016;
  else if(_campaign == "Summer22") _year = 2022;
  else cout<<"main: Provide correct campaign name"<<endl;
  cout<<"Year set to: "<<_year<<endl;

  //Loading offline data (json, text):
  jsondata = loadJson();

  //Initializing counters:
  nEvtTotal = 0; nEvtRan = 0;  nEvtTrigger=0;
  nEvtPass=0;    nEvtBad=0;    nThrown=0;

  evt_wt = 1.0;
  bad_event = false;
  evt_trigger = false;

  //For skimmer
  tree->SetBranchStatus("*",0);
  ActivateBranch(tree);

  cout<<"\nn-events \t time_taken (sec)"<<endl;
  
}

void AnaScript::SlaveTerminate()
{
  //For skimmer
  cout<<"Writing Skim File....";
  skimTree->Write();
  _SkimFile->Close();
  cout<<"   Done!  "<<endl;
  
  float goodevtfrac = ((float)nEvtRan)/((float)nEvtTotal);
  float trigevtfrac = ((float)nEvtTrigger)/((float)nEvtTotal);
  float passevtfrac = ((float)nEvtPass)/((float)nEvtTotal);
  float badevtfrac  = ((float)nEvtBad)/((float)nEvtTotal);//for VLLD
  float notgoldenevtfrac  = ((float)nThrown)/((float)nEvtTotal); //for data

  cout<<"---------------------------------------------"<<endl;
  cout<<"Summary:"<<endl;
  cout<<"nEvtTotal = "<<nEvtTotal<<endl;
  cout<<"nEvtRan = "<<nEvtRan<<" ("<<goodevtfrac*100<<" %)"<<endl;
  cout<<"nEvtTrigger = "<<nEvtTrigger<<" ("<<trigevtfrac*100<<" %)"<<endl;
  cout<<"nEvtPass = "<<nEvtPass<<" ("<<passevtfrac*100<<" %)"<<endl;
  cout<<"nEvtBad = "<<nEvtBad<<" ("<<badevtfrac*100<<" %)"<<endl;
  if(_data!=0) cout<<"nEvents not in golden json = "<<nThrown<<" ("<<notgoldenevtfrac*100<<" %)"<<endl;
  cout<<"---------------------------------------------"<<endl;

  time(&end);
  double time_taken = double(end-start);
  cout<<"\nTime taken by the programe is= "<<fixed<<time_taken<<setprecision(5);
  cout<<" sec \n"<<endl;

}

void AnaScript::Terminate()
{

}

//---------------------------------------------------------------------------------------------------------------
// EVENT LOOP
//---------------------------------------------------------------------------------------------------------------
Bool_t AnaScript::Process(Long64_t entry)
{
  nEvtTotal++;
  //------------------------------------------------------
  //Initializing fReaders:
  fReader.SetLocalEntry(entry);
  if(_run3)  fReader_Run3.SetLocalEntry(entry);
  else       fReader_Run2.SetLocalEntry(entry);
  if(_data == 0){
    fReader_MC.SetLocalEntry(entry);
    if(!_run3){
      fReader_Run2_MC.SetLocalEntry(entry);
      if(_flag != "qcd") fReader_Run2_MC_nonQCD.SetLocalEntry(entry);
    }
    else {
      fReader_Run3_MC.SetLocalEntry(entry);
    }
  } 
  //-------------------------------------------------------

  ReadBranch(); //for skimmer

  //Setting verbosity:
  time(&buffer);
  double time_taken_so_far = double(buffer-start);
  if(_verbosity==0 && nEvtTotal%10000==0)     cout<<nEvtTotal<<" \t "<<time_taken_so_far<<endl;
  else if(_verbosity>0 && nEvtTotal%10000==0) cout<<nEvtTotal<<" \t "<<time_taken_so_far<<endl;
  
  GoodEvt2018 = (_year==2018 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && *Flag_BadPFMuonFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  GoodEvt2017 = (_year==2017 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && *Flag_BadPFMuonFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  GoodEvt2016 = (_year==2016 ? *Flag_goodVertices && *Flag_globalSuperTightHalo2016Filter && *Flag_HBHENoiseFilter && *Flag_HBHENoiseIsoFilter && *Flag_EcalDeadCellTriggerPrimitiveFilter && *Flag_BadPFMuonFilter && (_data ? *Flag_eeBadScFilter : 1) : 1);
  
  GoodEvt = GoodEvt2016 && GoodEvt2017 && GoodEvt2018;
  if(_campaign=="Summer22") GoodEvt = true;

  if(GoodEvt){
    nEvtRan++;
    
    //------------------------------------------------------------------------------------------------------------
    // TRIGGER
    //------------------------------------------------------------------------------------------------------------
    triggerRes         = true; //default, always true for MC
    muon_trigger       = false;
    electron_trigger   = false;
    overlapping_events = false;

    muon_trigger =     (*HLT_SingleMuon==1);
    electron_trigger = (*HLT_SingleEle==1);
    overlapping_events = muon_trigger && electron_trigger;

    if(_data !=0){

      // Not applying trigger while skimming. That is done while running TreeMaker.

      //Throw awaying bad data that are not included in the GoldenJson:
      int runno = (int)*run;
      int lumisection = (int)*luminosityBlock;
      bool golden_event = checkJson(true, runno, lumisection);
      if(!golden_event){
	nThrown++;
	triggerRes = false;
      }
      
    }//Applying trigger on data


    if(triggerRes){
      nEvtTrigger++;

      //----------------------------------------------------------------------------------------------------------
      // OBJECT DEFINITIONS
      //----------------------------------------------------------------------------------------------------------

      //----------------
      // Gen-Particles
      //----------------
      genMuon.clear();  genElectron.clear();  genLightLepton.clear();
      genJet.clear();   vllep.clear();        vlnu.clear();

      //Handling bad events from signal:
      bad_event = false;

      if(_data==0){
	createGenLightLeptons();
	createGenJets();
        SortGenObjects();
	SortPt(genMuon);
	SortPt(genElectron);
	SortPt(genLightLepton);
	createSignalArrays();
	SortVLL();

	//Correcting the Doublet model (flagging out the invalid decays)
	if(_flag=="doublet"){ //for VLLD files
	  bad_event = false;
	  //a) The neutral particle cannot decay to H,nu or Z,nu.
	  // I am flagging out the events with Higgs(25) or the Z(23) as daughetrs of N
	  //cout<<"----"<<endl;
	  for(int i=0; i<(int)vlnu.size(); i++){
	    for(int j=0; j<(int)vlnu.at(i).dauid.size(); j++){
	      if(fabs(vlnu.at(i).dauid[j]) == 25)      bad_event = true;
	      else if(fabs(vlnu.at(i).dauid[j]) == 23) bad_event = true;
	      //cout<<fabs(vlnu.at(i).dauid[j])<<" ";
	    }
	    //cout<<""<<endl;
	  }
	  //if(bad_event) cout<<"bad"<<endl;
	  //else cout<<"good"<<endl;
	  //cout<<"----"<<endl;
	  
	  //b) The lepton cannot decay to a W,nu of the corresponding flavor (ele/mu):
	  // I am flagging out the events with W(24) as daughetrs of L
	  for(int i=0; i<(int)vllep.size(); i++){
	    for(int j=0; j<(int)vllep.at(i).dauid.size(); j++){
	      if(fabs(vllep.at(i).dauid[j]) == 24)     bad_event = true;
	    }
	  }
	}
	//Make gen-level plots here.
      }
      //Counting bad events:
      if(bad_event) nEvtBad++;


      //----------------
      // Reco-Particles
      //----------------
      
      metpt = *PuppiMET_pt;
      metphi = *PuppiMET_phi;
      
      Muon.clear();    Electron.clear(); LightLepton.clear();
      Photon.clear();  Tau.clear();      Jet.clear();
      bJet.clear();    MediumbJet.clear();
      LooseLepton.clear(); LooseElectron.clear(); LooseMuon.clear();
      
      createLightLeptons();
      createTaus();
      createJets();

      SortRecoObjects();

      //----------------------------------------------------------------------------------------------------------
      // Skimming
      //----------------------------------------------------------------------------------------------------------

      /*
      //Forcing TTreeReader to read some of the MC branches:
      if(_data==0){
	float Pileup_nTrueInt = (float)Pileup_nTrueInt;
	float LHEwt=0; float LHEreweight=0; float LHEscaleweight=0;
	if(_flag!="qcd"){
	for(unsigned int i=0; i<(unsigned int)**ptr_nLHEPdfWeight; i++)
	  LHEwt          = (float)(*ptr_LHEPdfWeight)[i];
	for(unsigned int i=0; i<(unsigned int)**ptr_nLHEReweightingWeight; i++ )
	  LHEreweight    = (float)(*ptr_LHEReweightingWeight)[i];
	for(unsigned int i=0; i<(unsigned int)**ptr_nLHEScaleWeight; i++ )
	  LHEscaleweight = (float)(*ptr_LHEScaleWeight)[i];
	}
	if(*Pileup_nTrueInt > 0){
	  cout<<"Forcing TTreeReader to read some MC branches"<<"\t";
	  cout<<*Pileup_nTrueInt<<endl;
	  //cout<<LHEwt<<"\t"<<LHEreweight<<"\t"<<LHEscaleweight<<endl;
	}
	}*/


      bool keep_this_event = false;
      
      //-----------
      //2LSS skim:
      //----------
      float ptcut_mu  = 26; if(_year==2017) ptcut_mu  = 29;
      float ptcut_ele = 35; if(_year==2017) ptcut_ele = 37; if(_year==2016) ptcut_ele = 30;

      if((int)LightLepton.size()==2){
	//Condition 1: SS
	bool samesign = LightLepton.at(0).charge == LightLepton.at(1).charge;
	//Condition 2: finding a triggerable object
	bool trigger = false;
	for(int i=0; i<(int)LightLepton.size(); i++){
	  int lepton_id = fabs(LightLepton.at(i).id);
	  float lepton_pt = LightLepton.at(i).v.Pt();
	  if(lepton_id == 11 && lepton_pt > ptcut_ele) trigger = true;
	  if(lepton_id == 13 && lepton_pt > ptcut_mu)  trigger = true;
	}
	//Condition 3: low-res veto
	bool reject_low_resonances = (LightLepton.at(0).v + LightLepton.at(1).v).M() > 15;
	
	if(trigger && reject_low_resonances && samesign) keep_this_event = true;
      }

      //--------------
      // For Shalini
      //--------------
      
      /*
      if((int)LooseLepton.size()>=2){
	
	// 1) Object selection:
	//Loose muons/electron, pT>15, abs(eta)<2.4
	//Muon_looseID or Electron_cutBased[i]>1
	//prompt, reliso03 < 1.0;
	
        // 2) dilepton mass requirement:
	float dilep_mass = (LooseLepton.at(0).v + LooseLepton.at(1).v).M();
	bool reject_low_mass = dilep_mass > 50;

	bool trigger = false;
	for(int i=0; i<(int)LooseLepton.size(); i++){
	  int lepton_id = fabs(LooseLepton.at(i).id);
	  float lepton_pt = LooseLepton.at(i).v.Pt();
	  if(lepton_id == 11 && lepton_pt > 35) trigger = true;
	  if(lepton_id == 13 && lepton_pt > 26) trigger = true;
	}
	
	if(reject_low_mass) keep_this_event = true;
	
	}*/
      
      if(bad_event) keep_this_event = false;
      
      if(keep_this_event){
	nEvtPass++;
	skimTree->Fill();
      }
      
      
    }//Trigger
  }//GoodEvt
  
  return kTRUE;
}

