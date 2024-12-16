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

//Corrections:
#include "/home/work/phazarik1/work/VLLSearch/Corrections/calculateCorrections.h"
#include "/home/work/phazarik1/work/VLLSearch/Corrections/TriggerEfficiency.h"

//Headers used by this particular setup:
#include "/home/work/phazarik1/work/VLLSearch/AnaScript/TreeMaker/treeMakerHelper.h"

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
  if(_flag=="doublet") cout<<"Removing invalid VLLD decay modes ..."<<endl;

  //Loading offline data (json, text):
  jsondata = loadJson();
  LoadCorrectionsFromPOG();

  //Initializing counters:
  nEvtTotal = 0; nEvtRan = 0;  nEvtTrigger=0;
  nEvtPass=0;    nEvtBad=0;    nThrown=0;

  evt_wt = 1.0;
  bad_event = false;
  evt_trigger = false;

  cout<<"\nn-events \t time_taken (sec)"<<endl;
  
}

void AnaScript::SlaveTerminate()
{
  //For treeMaker:
  mytree->Write();
  _TreeFile->Close();
  
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
    if(!_run3) {
      fReader_Run2_MC.SetLocalEntry(entry);
      if(_flag != "qcd") fReader_Run2_MC_nonQCD.SetLocalEntry(entry);
    }
    else       fReader_Run3_MC.SetLocalEntry(entry);
  } 
  //------------------------------------------------------

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

      //Applying trigger on data:
      if(_flag != "egamma") triggerRes = muon_trigger; //For the SingleMuon dataset, including overlap.
      if(_flag == "egamma") triggerRes = electron_trigger && !muon_trigger; //For the EGamma dataset

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
      // Writing to tree
      //----------------------------------------------------------------------------------------------------------

      FillTree(mytree);
      
      
    }//Trigger
  }//GoodEvt
  
  return kTRUE;
}

