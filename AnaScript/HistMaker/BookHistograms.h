void AnaScript::BookHistograms(){
  
  h.evt2LSS[0]  = new TH1F("nlep", "nlep", 10, 0, 10);
  h.evt2LSS[1]  = new TH1F("njet", "njet", 10, 0, 10);
  h.evt2LSS[2]  = new TH1F("nbjet", "nbjet", 10, 0, 10);

  h.evt2LSS[3]  = new TH1F("lep0_pt",  "lep0_pt",  50, 0, 500);
  h.evt2LSS[4]  = new TH1F("lep0_eta", "lep0_eta", 100, -4, 4);
  h.evt2LSS[5]  = new TH1F("lep0_phi", "lep0_phi", 100, -4, 4);
  h.evt2LSS[6]  = new TH1F("lep0_iso", "lep0_iso", 100, 0, 0.2);
  h.evt2LSS[7]  = new TH1F("lep0_sip3d", "lep0_sip3d", 500, 0, 50);
  h.evt2LSS[8]  = new TH1F("lep0_mt",  "lep0_mt",  50, 0, 500);
  h.evt2LSS[9] = new TH1F("lep0_deepjet", "lep0_deepjet", 300, -1, 2);
  
  h.evt2LSS[10]  = new TH1F("lep1_pt",  "lep1_pt",  50, 0, 500);
  h.evt2LSS[11]  = new TH1F("lep1_eta", "lep1_eta", 100, -4, 4);
  h.evt2LSS[12] = new TH1F("lep1_phi", "lep1_phi", 100, -4, 4);
  h.evt2LSS[13] = new TH1F("lep1_iso", "lep1_iso", 100, 0, 0.2);
  h.evt2LSS[14]  = new TH1F("lep1_sip3d", "lep1_sip3d", 500, 0, 50);
  h.evt2LSS[15] = new TH1F("lep1_mt",  "lep1_mt",  50, 0, 500);
  h.evt2LSS[16] = new TH1F("lep1_deepjet", "lep1_deepjet", 300, -1, 2);
  
  h.evt2LSS[17] = new TH1F("dilep_pt",  "dilep_pt",  50, 0, 500);
  h.evt2LSS[18] = new TH1F("dilep_eta", "dilep_eta", 100, -10, 10);
  h.evt2LSS[19] = new TH1F("dilep_phi", "dilep_phi", 100, -4, 4);
  h.evt2LSS[20] = new TH1F("dilep_mass","dilep_mass",50, 0, 500);
  h.evt2LSS[21] = new TH1F("dilep_mt",  "dilep_mt",  50, 0, 500);
  h.evt2LSS[22] = new TH1F("dilep_deta", "dilep_deta", 100, 0, 6);
  h.evt2LSS[23] = new TH1F("dilep_dphi", "dilep_dphi", 100, 0, 6);
  h.evt2LSS[24] = new TH1F("dilep_dR",   "dilep_dR",   100, 0, 6);
  h.evt2LSS[25] = new TH1F("dilep_ptratio","dilep_ptratio",100, 0, 1);

  h.evt2LSS[26] = new TH1F("HT", "HT", 20, 0, 500);
  h.evt2LSS[27] = new TH1F("LT", "LT", 20, 0, 500);
  h.evt2LSS[28] = new TH1F("STvis", "STvis", 50, 0, 500);
  h.evt2LSS[29] = new TH1F("ST", "ST", 50, 0, 500);
  h.evt2LSS[30] = new TH1F("STfrac", "STfrac", 100, 0, 1.1); //For nJet-==0, STfrac=1
  h.evt2LSS[31] = new TH1F("HTMETllpt", "HTMETllpt", 50, 0, 500);
  h.evt2LSS[32] = new TH1F("metpt", "MET", 50, 0, 500);
  h.evt2LSS[33] = new TH1F("metphi", "MET_phi", 100, -4, 4);
  
  h.evt2LSS[34] = new TH1F("dphi_metlep0",    "dilep_metlep0",    100, 0, 4);
  h.evt2LSS[35] = new TH1F("dphi_metlep1",    "dilep_metlep1",    100, 0, 4);
  h.evt2LSS[36] = new TH1F("dphi_metdilep",   "dilep_metdilep",   100, 0, 4);
  h.evt2LSS[37] = new TH1F("dphi_metlep_max", "dilep_metlep_max", 100, 0, 4);
  h.evt2LSS[38] = new TH1F("dphi_metlep_min", "dilep_metlep_min", 100, 0, 4);

  h.evt2LSS[39] = new TH1F("2LSS_wt_leptonSF",  "Reco-ID-Iso Scale Factor",200,0,2);
  h.evt2LSS[40] = new TH1F("2LSS_wt_trig","Trigger Scale Factor",200,0,2);
  h.evt2LSS[41] = new TH1F("2LSS_wt_bjet","bJet weight",200,0,2);
  h.evt2LSS[42] = new TH1F("2LSS_wt_evt", "Event weight",200,0,2);
  
  for(int i=0; i<43; i++) h.evt2LSS[i]->Sumw2();

  h.correction[0] = new TH1F("sf_jec",  "JEC",        200, 0, 2);
  h.correction[1] = new TH1F("sf_jer",  "JER",        200, 0, 2);
  h.correction[2] = new TH1F("wt_lep",  "lep wt",     200, 0, 2);
  h.correction[3] = new TH1F("wt_trig", "teigger wt", 200, 0, 2);
  h.correction[4] = new TH1F("wt_pu",   "pileup wt",  200, 0, 2);
  h.correction[5] = new TH1F("wt_bjet", "bjet wt",    200, 0, 2);
  h.correction[6] = new TH1F("wt_evt",  "event wt",   200, 0, 2);
  for(int i=0; i<7; i++) h.correction[i]->Sumw2();
}
