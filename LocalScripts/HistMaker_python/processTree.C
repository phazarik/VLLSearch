//--------------------------------------------------------------
// This script takes a ROOT file containing custom made tree
// with event level branches, and produces histograms.
//--------------------------------------------------------------

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>

void processTree(const char* inputFilename, const char* outputFilename, const char* campaign, int channelval) {
  // Opening the ROOT file
  TFile *file = TFile::Open(inputFilename);
  if (!file || file->IsZombie()) {
    std::cerr << "Error opening file: " << inputFilename << std::endl;
    return;
  }

  // Getting the TTree
  TTree *tree = (TTree*)file->Get("myEvents");
  if (!tree) {
    std::cerr << "Error getting TTree from file: " << inputFilename << std::endl;
    file->Close();
    return;
  }

  // Setting branch addresses with updated types
  /*
  Int_t channel, nlep, njet, nbjet;
  Float_t lep0_pt, lep0_eta, lep0_phi, lep0_iso, lep0_sip3d, lep0_mt;
  Float_t lep1_pt, lep1_eta, lep1_phi, lep1_iso, lep1_sip3d, lep1_mt;
  Float_t dilep_pt, dilep_eta, dilep_phi, dilep_mass, dilep_mt, dilep_deta, dilep_dphi, dilep_dR, dilep_ptratio;
  Float_t HT, LT, STvis, ST, HTMETllpt, STfrac, metpt, metphi;
  Float_t dphi_metlep0, dphi_metlep1, dphi_metdilep, dphi_metlep_max, dphi_metlep_min;
  Double_t wt_leptonSF, wt_trig, wt_bjet, weight;
  Float_t nnscore1;*/

  Long64_t channel, trigger, nlep, njet, nbjet;
  Double_t lep0_pt, lep0_eta, lep0_phi, lep0_iso, lep0_sip3d, lep0_mt;
  Double_t lep1_pt, lep1_eta, lep1_phi, lep1_iso, lep1_sip3d, lep1_mt;
  Double_t dilep_pt, dilep_eta, dilep_phi, dilep_mass, dilep_mt, dilep_deta, dilep_dphi, dilep_dR, dilep_ptratio;
  Double_t HT, LT, STvis, ST, HTMETllpt, STfrac, metpt, metphi;
  Double_t dphi_metlep0, dphi_metlep1, dphi_metdilep, dphi_metlep_max, dphi_metlep_min;
  Double_t wt_leptonSF, wt_trig, wt_pileup, wt_bjet, weight;
  Double_t nnscore1, nnscore2, nnscore3, nnscore4;

  tree->SetBranchAddress("channel", &channel);
  tree->SetBranchAddress("trigger", &trigger);
  tree->SetBranchAddress("nlep", &nlep);
  tree->SetBranchAddress("njet", &njet);
  tree->SetBranchAddress("nbjet", &nbjet);
  tree->SetBranchAddress("lep0_pt", &lep0_pt);
  tree->SetBranchAddress("lep0_eta", &lep0_eta);
  tree->SetBranchAddress("lep0_phi", &lep0_phi);
  tree->SetBranchAddress("lep0_iso", &lep0_iso);
  tree->SetBranchAddress("lep0_sip3d", &lep0_sip3d);
  tree->SetBranchAddress("lep0_mt", &lep0_mt);
  tree->SetBranchAddress("lep1_pt", &lep1_pt);
  tree->SetBranchAddress("lep1_eta", &lep1_eta);
  tree->SetBranchAddress("lep1_phi", &lep1_phi);
  tree->SetBranchAddress("lep1_iso", &lep1_iso);
  tree->SetBranchAddress("lep1_sip3d", &lep1_sip3d);
  tree->SetBranchAddress("lep1_mt", &lep1_mt);
  tree->SetBranchAddress("dilep_pt", &dilep_pt);
  tree->SetBranchAddress("dilep_eta", &dilep_eta);
  tree->SetBranchAddress("dilep_phi", &dilep_phi);
  tree->SetBranchAddress("dilep_mass", &dilep_mass);
  tree->SetBranchAddress("dilep_mt", &dilep_mt);
  tree->SetBranchAddress("dilep_deta", &dilep_deta);
  tree->SetBranchAddress("dilep_dphi", &dilep_dphi);
  tree->SetBranchAddress("dilep_dR", &dilep_dR);
  tree->SetBranchAddress("dilep_ptratio", &dilep_ptratio);
  tree->SetBranchAddress("HT", &HT);
  tree->SetBranchAddress("LT", &LT);
  tree->SetBranchAddress("STvis", &STvis);
  tree->SetBranchAddress("ST", &ST);
  tree->SetBranchAddress("HT", &HT);
  tree->SetBranchAddress("HTMETllpt", &HTMETllpt);
  tree->SetBranchAddress("STfrac", &STfrac);
  tree->SetBranchAddress("metpt", &metpt);
  tree->SetBranchAddress("metphi", &metphi);
  tree->SetBranchAddress("dphi_metlep0", &dphi_metlep0);
  tree->SetBranchAddress("dphi_metlep1", &dphi_metlep1);
  tree->SetBranchAddress("dphi_metdilep", &dphi_metdilep);
  tree->SetBranchAddress("dphi_metlep_max", &dphi_metlep_max);
  tree->SetBranchAddress("dphi_metlep_min", &dphi_metlep_min);
  tree->SetBranchAddress("wt_leptonSF", &wt_leptonSF);
  tree->SetBranchAddress("wt_trig", &wt_trig);
  tree->SetBranchAddress("wt_pileup", &wt_pileup);
  tree->SetBranchAddress("wt_bjet", &wt_bjet);
  tree->SetBranchAddress("weight", &weight);
  tree->SetBranchAddress("nnscore_qcd_vlldele_100",     &nnscore1);
  tree->SetBranchAddress("nnscore_qcd_vlldele_200_800", &nnscore2);
  tree->SetBranchAddress("nnscore_qcd_vlldmu_100",      &nnscore3);
  tree->SetBranchAddress("nnscore_qcd_vlldmu_200_800",  &nnscore4);

  // Defining histograms with their respective binning
  vector<float> mybins; //for custom binning:
  mybins = {0, 25, 50, 100, 200, 300, 400, 500};
  
  TH1F *hist_channel = new TH1F("channel", "channel", 10, 0, 10); hist_channel->Sumw2();
  TH1F *hist_trigger = new TH1F("trigger", "trigger", 10, 0, 10); hist_trigger->Sumw2();
  TH1F *hist_nlep = new TH1F("nlep", "nlep",          10, 0, 10); hist_nlep->Sumw2();
  TH1F *hist_njet = new TH1F("njet", "njet",          10, 0, 10); hist_njet->Sumw2();
  TH1F *hist_nbjet = new TH1F("nbjet", "nbjet",       10, 0, 10); hist_nbjet->Sumw2();

  TH1F *hist_lep0_pt =    new TH1F("lep0_pt",    "lep0_pt",     50, 0, 500); hist_lep0_pt->Sumw2();
  TH1F *hist_lep0_eta =   new TH1F("lep0_eta",   "lep0_eta",   100, -4, 4);  hist_lep0_eta->Sumw2();
  TH1F *hist_lep0_phi =   new TH1F("lep0_phi",   "lep0_phi",   100, -4, 4);  hist_lep0_phi->Sumw2();
  TH1F *hist_lep0_iso =   new TH1F("lep0_iso",   "lep0_iso",   200, 0, 0.2); hist_lep0_iso->Sumw2();
  TH1F *hist_lep0_sip3d = new TH1F("lep0_sip3d", "lep0_sip3d", 200, 0, 50);  hist_lep0_sip3d->Sumw2();
  TH1F *hist_lep0_mt =    new TH1F("lep0_mt",    "lep0_mt",     50, 0, 500); hist_lep0_mt->Sumw2();

  TH1F *hist_lep1_pt =    new TH1F("lep1_pt",    "lep1_pt",     50, 0, 500); hist_lep1_pt->Sumw2();
  TH1F *hist_lep1_eta =   new TH1F("lep1_eta",   "lep1_eta",   100, -4, 4);  hist_lep1_eta->Sumw2();
  TH1F *hist_lep1_phi =   new TH1F("lep1_phi",   "lep1_phi",   100, -4, 4);  hist_lep1_phi->Sumw2();
  TH1F *hist_lep1_iso =   new TH1F("lep1_iso",   "lep1_iso",   200, 0, 0.2); hist_lep1_iso->Sumw2();
  TH1F *hist_lep1_sip3d = new TH1F("lep1_sip3d", "lep1_sip3d", 200, 0, 50);  hist_lep1_sip3d->Sumw2();
  TH1F *hist_lep1_mt =    new TH1F("lep1_mt",    "lep1_mt",     50, 0, 500); hist_lep1_mt->Sumw2();

  TH1F *hist_dilep_pt = new TH1F("dilep_pt", "dilep_pt", (int)mybins.size()-1, &mybins[0]); hist_dilep_pt->Sumw2();
  TH1F *hist_dilep_eta = new TH1F("dilep_eta", "dilep_eta", 100, -10, 10);   hist_dilep_eta->Sumw2();
  TH1F *hist_dilep_phi = new TH1F("dilep_phi", "dilep_phi", 100, -4, 4);     hist_dilep_phi->Sumw2();
  TH1F *hist_dilep_mass = new TH1F("dilep_mass", "dilep_mass", 50, 0, 500);  hist_dilep_mass->Sumw2();
  TH1F *hist_dilep_mt = new TH1F("dilep_mt", "dilep_mt", (int)mybins.size()-1, &mybins[0]); hist_dilep_mt->Sumw2();
  TH1F *hist_dilep_deta = new TH1F("dilep_deta", "dilep_deta", 100, 0, 6);   hist_dilep_deta->Sumw2();
  TH1F *hist_dilep_dphi = new TH1F("dilep_dphi", "dilep_dphi", 100, 0, 6);   hist_dilep_dphi->Sumw2();
  TH1F *hist_dilep_dR = new TH1F("dilep_dR", "dilep_dR", 100, 0, 6);         hist_dilep_dR->Sumw2();
  TH1F *hist_dilep_ptratio = new TH1F("dilep_ptratio", "dilep_ptratio", 100, 0, 1); hist_dilep_ptratio->Sumw2();

  TH1F *hist_HT = new TH1F("HT", "HT", (int)mybins.size()-1, &mybins[0]);          hist_HT->Sumw2();
  TH1F *hist_LT = new TH1F("LT", "LT", (int)mybins.size()-1, &mybins[0]);          hist_LT->Sumw2();
  TH1F *hist_STvis = new TH1F("STvis", "STvis", (int)mybins.size()-1, &mybins[0]); hist_STvis->Sumw2();
  TH1F *hist_ST = new TH1F("ST", "ST", (int)mybins.size()-1, &mybins[0]);          hist_ST->Sumw2();
  TH1F *hist_STfrac = new TH1F("STfrac", "STfrac", 100, 0, 1.1);                   hist_STfrac->Sumw2();
  TH1F *hist_HTMETllpt = new TH1F("HTMETllpt", "HTMETllpt", (int)mybins.size()-1, &mybins[0]); hist_HTMETllpt->Sumw2();
  TH1F *hist_metpt = new TH1F("metpt", "metpt",(int)mybins.size()-1, &mybins[0]);  hist_metpt->Sumw2();
  TH1F *hist_metphi = new TH1F("metphi", "metphi", 100, -4, 4);                    hist_metphi->Sumw2();

  TH1F *hist_dphi_metlep0 = new TH1F("dphi_metlep0", "dphi_metlep0", 100, 0, 4);          hist_dphi_metlep0->Sumw2();
  TH1F *hist_dphi_metlep1 = new TH1F("dphi_metlep1", "dphi_metlep1", 100, 0, 4);          hist_dphi_metlep1->Sumw2();
  TH1F *hist_dphi_metdilep = new TH1F("dphi_metdilep", "dphi_metdilep", 100, 0, 4);       hist_dphi_metdilep->Sumw2();
  TH1F *hist_dphi_metlep_max = new TH1F("dphi_metlep_max", "dphi_metlep_max", 100, 0, 4); hist_dphi_metlep_max->Sumw2();
  TH1F *hist_dphi_metlep_min = new TH1F("dphi_metlep_min", "dphi_metlep_min", 100, 0, 4); hist_dphi_metlep_min->Sumw2();

  TH1F *hist_wt_leptonSF = new TH1F("2LSS_wt_leptonSF", "wt_leptonSF", 200, 0, 2); hist_wt_leptonSF->Sumw2();
  TH1F *hist_wt_trig     = new TH1F("2LSS_wt_trig",     "wt_trig",     200, 0, 2); hist_wt_trig->Sumw2();
  TH1F *hist_wt_pileup   = new TH1F("2LSS_wt_pileup",   "wt_pileup",   200, 0, 2); hist_wt_pileup->Sumw2();
  TH1F *hist_wt_bjet     = new TH1F("2LSS_wt_bjet",     "wt_bjet",     200, 0, 2); hist_wt_bjet->Sumw2();
  TH1F *hist_weight      = new TH1F("2LSS_wt_evt",      "weight",      200, 0, 2); hist_weight->Sumw2();

  TH1F *hist_nnscore1 = new TH1F("nnscore_qcd_vlldele_100",     "nnscore_qcd_vlldele_100",     200, 0, 1); hist_nnscore1->Sumw2();
  TH1F *hist_nnscore2 = new TH1F("nnscore_qcd_vlldele_200_800", "nnscore_qcd_vlldele_200_800", 200, 0, 1); hist_nnscore2->Sumw2();
  TH1F *hist_nnscore3 = new TH1F("nnscore_qcd_vlldmu_100",      "nnscore_ttbar_vlldmu_100",    200, 0, 1); hist_nnscore3->Sumw2();
  TH1F *hist_nnscore4 = new TH1F("nnscore_qcd_vlldmu_200_800",  "nnscore_qcd_vlldmu_200_800",  200, 0, 1); hist_nnscore4->Sumw2();


  //Flagging specific samples:
  std::string filename(inputFilename);
  std::string baseFilename = filename.substr(filename.find_last_of("/\\") + 1);
  bool correctDY    = baseFilename.find("DYJetsToLL") != std::string::npos;
  bool correctTTBar = baseFilename.find("TTBar") != std::string::npos;
  if(correctDY)    cout<<"Correcting DY events in HT bins (ee)"<<endl;
  if(correctTTBar) cout<<"Correcting TTBar events in LT bins (all channels)"<<endl;
  
  // Event loop:
  Long64_t nentries = tree->GetEntries();
  for (Long64_t i = 0; i < nentries; i++) {
    tree->GetEntry(i);

    //-----------------------------------------------
    // Based on the event-level variables available,
    // Put event selections here.
    bool event_selection = false;
    bool channel_selection = channel == channelval; //This decides the channel
    Double_t wt = wt_leptonSF*wt_trig*wt_pileup*wt_bjet;
    //Double_t wt = wt_leptonSF*wt_trig*wt_pileup;

    //Corrections for individual backgrounds in bins:
    if (correctDY) {
      if(channelval == 3){//For ee channel only
	float Zpt = dilep_pt;
	if(string(campaign) == "2018_UL"){
	  Double_t HT_wt = 1.0;
	  if(HT < 25)                   HT_wt = 1.5243;
	  else if(25 < HT  && HT < 50)  HT_wt = 1.0233;
	  else if(50 < HT  && HT < 100) HT_wt = 0.8203;
	  else if(100 < HT && HT < 200) HT_wt = 0.7536;
	  else if(200 < HT && HT < 300) HT_wt = 0.7366;
	  else if(300 < HT && HT < 400) HT_wt = 0.7684;
	  else                          HT_wt = 0.7809;
	  wt = wt * HT_wt;
	}
	else if (string(campaign)== "2016preVFP_UL"){
	  Double_t HT_wt = 1.0;
	  if(HT < 25)                     HT_wt = 0.9708;
	  else if(25 <= HT  && HT < 50)   HT_wt = 0.6535;
	  else if(50 <= HT  && HT < 100)  HT_wt = 0.5180;
	  else if(100 <= HT && HT < 200)  HT_wt = 0.4765;
	  else if(200 <= HT && HT < 300)  HT_wt = 0.3906;
	  else if(300 <= HT && HT < 400)  HT_wt = 0.6887;
	  else                            HT_wt = 0.3395; //Including overlay
	  wt = wt * HT_wt;
	}
      }// CorrectDY
    }// ee channel

    if(correctTTBar){
      Double_t metpt_wt = 1.0;
      Double_t LT_wt = 1.0;
      //2018_UL
      if(string(campaign) == "2018_UL"){
	if(channelval == 0){
	  if(metpt < 25)                        metpt_wt = 0.9484;
	  else if(25 <= metpt  && metpt < 50)   metpt_wt = 1.3137;
	  else if(50 <= metpt  && metpt < 100)  metpt_wt = 1.1517;
	  else if(100 <= metpt && metpt < 200)  metpt_wt = 1.0123;
	  else if(200 <= metpt && metpt < 300)  metpt_wt = 0.5368;
	  else if(300 <= metpt && metpt < 400)  metpt_wt = 1.4167;
	  else                                  metpt_wt = 1.3333;	  
	  if(LT < 25)                     LT_wt = 1.0;
	  else if(25 <= LT  && LT < 50)   LT_wt = 0.9849;
	  else if(50 <= LT  && LT < 100)  LT_wt = 1.0499;
	  else if(100 <= LT && LT < 200)  LT_wt = 0.9357;
	  else if(200 <= LT && LT < 300)  LT_wt = 0.5187;
	  else if(300 <= LT && LT < 400)  LT_wt = 3.5517;
	  else                            LT_wt = 3.6000;
	}
	if(channelval == 1){
	  if(metpt < 25)                        metpt_wt = 2.0000;
	  else if(25 <= metpt  && metpt < 50)   metpt_wt = 1.3069;
	  else if(50 <= metpt  && metpt < 100)  metpt_wt = 1.1179;
	  else if(100 <= metpt && metpt < 200)  metpt_wt = 0.8601;
	  else if(200 <= metpt && metpt < 300)  metpt_wt = 1.5763;
	  else if(300 <= metpt && metpt < 400)  metpt_wt = 4.2857;
	  else                                  metpt_wt = 1.0000;
	  if(LT < 25)                     LT_wt = 1.0;
	  else if(25 <= LT  && LT < 50)   LT_wt = 1.0654;
	  else if(50 <= LT  && LT < 100)  LT_wt = 0.9621;
	  else if(100 <= LT && LT < 200)  LT_wt = 0.9581;
	  else if(200 <= LT && LT < 300)  LT_wt = 1.2041;
	  else if(300 <= LT && LT < 400)  LT_wt = 2.6957;
	  else                            LT_wt = 3.9333;
	}
	if(channelval == 2){
	  if(metpt < 25)                        metpt_wt = 0.5616;
	  else if(25 <= metpt  && metpt < 50)   metpt_wt = 0.9085;
	  else if(50 <= metpt  && metpt < 100)  metpt_wt = 1.1194;
	  else if(100 <= metpt && metpt < 200)  metpt_wt = 1.1936;
	  else if(200 <= metpt && metpt < 300)  metpt_wt = 0.3019;
	  else if(300 <= metpt && metpt < 400)  metpt_wt = 2.6000;
	  else                                  metpt_wt = 0.1111;
	  if(LT < 25)                     LT_wt = 1.0;
	  else if(25 <= LT  && LT < 50)   LT_wt = 1.0000;
	  else if(50 <= LT  && LT < 100)  LT_wt = 1.0206;
	  else if(100 <= LT && LT < 200)  LT_wt = 0.9510;
	  else if(200 <= LT && LT < 300)  LT_wt = 1.219; //combined
	  else if(300 <= LT && LT < 400)  LT_wt = 1.219; //combined
	  else                            LT_wt = 1.219; //combined
	}
	if(channelval == 3){
	  if(metpt < 25)                        metpt_wt = 6.2500;
	  else if(25 <= metpt  && metpt < 50)   metpt_wt = 1.7385;
	  else if(50 <= metpt  && metpt < 100)  metpt_wt = 1.3454;
	  else if(100 <= metpt && metpt < 200)  metpt_wt = 1.1655;
	  else if(200 <= metpt && metpt < 300)  metpt_wt = 0.3091;
	  else if(300 <= metpt && metpt < 400)  metpt_wt = 1.5; //combined
	  else                                  metpt_wt = 1.5; //combined
	  if(LT < 25)                     LT_wt = 1.0;
	  else if(25 <= LT  && LT < 50)   LT_wt = 3.5000;
	  else if(50 <= LT  && LT < 100)  LT_wt = 0.8952;
	  else if(100 <= LT && LT < 200)  LT_wt = 0.8986;
	  else if(200 <= LT && LT < 300)  LT_wt = 1.8962;
	  else if(300 <= LT && LT < 400)  LT_wt = 1.8462;
	  else                            LT_wt = 1.5000;
	}
      }//end 2018
      wt = wt * metpt_wt * LT_wt;
    }
    //nnscoreCombined = nnscore1*nnscore2*nnscore3;

    event_selection = channel_selection;
    //-----------------------------------------------
    
    if(event_selection){
      hist_channel->Fill(channel, 1.0);
      hist_trigger->Fill(trigger, 1.0);
      hist_nlep->Fill(nlep, wt);
      hist_njet->Fill(njet, wt);
      hist_nbjet->Fill(nbjet, wt);
      hist_lep0_pt->Fill(lep0_pt, wt);
      hist_lep0_eta->Fill(lep0_eta, wt);
      hist_lep0_phi->Fill(lep0_phi, wt);
      hist_lep0_iso->Fill(lep0_iso, wt);
      hist_lep0_sip3d->Fill(lep0_sip3d, wt);
      hist_lep0_mt->Fill(lep0_mt, wt);
      hist_lep1_pt->Fill(lep1_pt, wt);
      hist_lep1_eta->Fill(lep1_eta, wt);
      hist_lep1_phi->Fill(lep1_phi, wt);
      hist_lep1_iso->Fill(lep1_iso, wt);
      hist_lep1_sip3d->Fill(lep1_sip3d, wt);
      hist_lep1_mt->Fill(lep1_mt, wt);
      hist_dilep_pt->Fill(dilep_pt, wt);
      hist_dilep_eta->Fill(dilep_eta, wt);
      hist_dilep_phi->Fill(dilep_phi, wt);
      hist_dilep_mass->Fill(dilep_mass, wt);
      hist_dilep_mt->Fill(dilep_mt, wt);
      hist_dilep_deta->Fill(dilep_deta, wt);
      hist_dilep_dphi->Fill(dilep_dphi, wt);
      hist_dilep_dR->Fill(dilep_dR, wt);
      hist_dilep_ptratio->Fill(dilep_ptratio, wt);
      hist_HT->Fill(HT, wt);
      hist_LT->Fill(LT, wt);
      hist_STvis->Fill(STvis, wt);
      hist_ST->Fill(ST, wt);
      hist_STfrac->Fill(STfrac, wt);
      hist_HTMETllpt->Fill(HTMETllpt, wt);
      hist_metpt->Fill(metpt, wt);
      hist_metphi->Fill(metphi, wt);
      hist_dphi_metlep0->Fill(dphi_metlep0, wt);
      hist_dphi_metlep1->Fill(dphi_metlep1, wt);
      hist_dphi_metdilep->Fill(dphi_metdilep, wt);
      hist_dphi_metlep_max->Fill(dphi_metlep_max, wt);
      hist_dphi_metlep_min->Fill(dphi_metlep_min, wt);
      hist_wt_leptonSF->Fill(wt_leptonSF, 1.0);
      hist_wt_trig->Fill(wt_trig, 1.0);
      hist_wt_pileup->Fill(wt_pileup, 1.0);
      hist_wt_bjet->Fill(wt_bjet, 1.0);
      hist_weight->Fill(wt, 1.0);
      hist_nnscore1->Fill(nnscore1, wt);
      hist_nnscore2->Fill(nnscore2, wt);
      hist_nnscore3->Fill(nnscore3, wt);
      hist_nnscore4->Fill(nnscore4, wt);
    }
  }

  // Save histograms to a new ROOT file
  TFile *outputFile = new TFile(outputFilename, "RECREATE");
  hist_channel->Write();
  hist_trigger->Write();
  hist_nlep->Write();
  hist_njet->Write();
  hist_nbjet->Write();
  hist_lep0_pt->Write();
  hist_lep0_eta->Write();
  hist_lep0_phi->Write();
  hist_lep0_iso->Write();
  hist_lep0_sip3d->Write();
  hist_lep0_mt->Write();
  hist_lep1_pt->Write();
  hist_lep1_eta->Write();
  hist_lep1_phi->Write();
  hist_lep1_iso->Write();
  hist_lep1_sip3d->Write();
  hist_lep1_mt->Write();
  hist_dilep_pt->Write();
  hist_dilep_eta->Write();
  hist_dilep_phi->Write();
  hist_dilep_mass->Write();
  hist_dilep_mt->Write();
  hist_dilep_deta->Write();
  hist_dilep_dphi->Write();
  hist_dilep_dR->Write();
  hist_dilep_ptratio->Write();
  hist_HT->Write();
  hist_LT->Write();
  hist_STvis->Write();
  hist_ST->Write();
  hist_STfrac->Write();
  hist_HTMETllpt->Write();
  hist_metpt->Write();
  hist_metphi->Write();
  hist_dphi_metlep0->Write();
  hist_dphi_metlep1->Write();
  hist_dphi_metdilep->Write();
  hist_dphi_metlep_max->Write();
  hist_dphi_metlep_min->Write();
  hist_wt_leptonSF->Write();
  hist_wt_trig->Write();
  hist_wt_pileup->Write();
  hist_wt_bjet->Write();
  hist_weight->Write();
  hist_nnscore1->Write();
  hist_nnscore2->Write();
  hist_nnscore3->Write();
  hist_nnscore4->Write();

  // Close files
  outputFile->Close();
  file->Close();

  std::cout << "Histograms have been saved to " << outputFilename << std::endl;
}
