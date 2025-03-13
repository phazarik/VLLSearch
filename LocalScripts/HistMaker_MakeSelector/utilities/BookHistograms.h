void myHist::BookHistograms(){

  h.hist.clear();
  h.hist.resize(49);
  std::vector<float> bins = {0, 25, 50, 100, 200, 300, 400, 500};
  std::vector<float> bins2 = {0, 10, 30, 70, 100, 500, 1000, 2000};
  std::vector<float> bins3 = {0, 25, 50, 100, 200, 300, 400, 500};

  // Histograms for variables
  h.hist[0]  = new TH1D("nlep", "nlep", 10, 0, 10);
  h.hist[1]  = new TH1D("njet", "njet", 10, 0, 10);
  h.hist[2]  = new TH1D("nbjet", "nbjet", 10, 0, 10);

  // Leading lepton histograms
  h.hist[3]  = new TH1D("lep0_pt",  "lep0_pt",  50, 0, 500);
  h.hist[4]  = new TH1D("lep0_eta", "lep0_eta", 100, -4, 4);
  h.hist[5]  = new TH1D("lep0_phi", "lep0_phi", 100, -4, 4);
  h.hist[6]  = new TH1D("lep0_iso", "lep0_iso", 100, 0, 0.2);
  h.hist[7]  = new TH1D("lep0_sip3d", "lep0_sip3d", 500, 0, 50);
  h.hist[8]  = new TH1D("lep0_mt",  "lep0_mt",  50, 0, 500);
  h.hist[9]  = new TH1D("lep0_charge", "lep0_charge", 4, -2, 2);

  // Subleading lepton histograms
  h.hist[10] = new TH1D("lep1_pt",  "lep1_pt",  50, 0, 500);
  h.hist[11] = new TH1D("lep1_eta", "lep1_eta", 100, -4, 4);
  h.hist[12] = new TH1D("lep1_phi", "lep1_phi", 100, -4, 4);
  h.hist[13] = new TH1D("lep1_iso", "lep1_iso", 100, 0, 0.2);
  h.hist[14] = new TH1D("lep1_sip3d", "lep1_sip3d", 500, 0, 50);
  h.hist[15] = new TH1D("lep1_mt",  "lep1_mt",  50, 0, 500);
  h.hist[16] = new TH1D("lep1_charge", "lep1_charge", 4, -2, 2);

  // Dilepton system histograms with variable binning
  h.hist[17] = new TH1D("dilep_pt",  "dilep_pt", bins.size()-1, &bins[0]);
  h.hist[18] = new TH1D("dilep_eta", "dilep_eta", 100, -10, 10);
  h.hist[19] = new TH1D("dilep_phi", "dilep_phi", 100, -4, 4);
  h.hist[20] = new TH1D("dilep_mass", "dilep_mass", 50, 0, 500);
  h.hist[21] = new TH1D("dilep_mt",  "dilep_mt", bins.size()-1, &bins[0]);
  h.hist[22] = new TH1D("dilep_deta", "dilep_deta", 100, 0, 6);
  h.hist[23] = new TH1D("dilep_dphi", "dilep_dphi", 100, 0, 6);
  h.hist[24] = new TH1D("dilep_dR",   "dilep_dR",   100, 0, 6);
  h.hist[25] = new TH1D("dilep_ptratio", "dilep_ptratio", 100, 0, 1);

  // Event level variables histograms with variable binning
  h.hist[26] = new TH1D("HT", "HT", bins.size()-1, &bins[0]);
  h.hist[27] = new TH1D("LT", "LT", bins.size()-1, &bins[0]);
  h.hist[28] = new TH1D("STvis", "STvis", bins.size()-1, &bins[0]);
  h.hist[29] = new TH1D("ST", "ST", bins.size()-1, &bins[0]);
  h.hist[30] = new TH1D("STfrac", "STfrac", 100, 0, 1.1);
  h.hist[31] = new TH1D("HTMETllpt", "HTMETllpt", bins.size()-1, &bins[0]);
  h.hist[32] = new TH1D("metpt", "MET", bins.size()-1, &bins[0]);
  h.hist[33] = new TH1D("metphi", "MET_phi", 100, -4, 4);

  // dphi histograms
  h.hist[34] = new TH1D("dphi_metlep0",    "dilep_metlep0",    100, 0, 4);
  h.hist[35] = new TH1D("dphi_metlep1",    "dilep_metlep1",    100, 0, 4);
  h.hist[36] = new TH1D("dphi_metdilep",   "dilep_metdilep",   100, 0, 4);
  h.hist[37] = new TH1D("dphi_metlep_max", "dilep_metlep_max", 100, 0, 4);
  h.hist[38] = new TH1D("dphi_metlep_min", "dilep_metlep_min", 100, 0, 4);

  // Weight histograms
  h.hist[39] = new TH1D("wt_leptonSF",  "Reco-ID-Iso Scale Factor", 200, 0, 2);
  h.hist[40] = new TH1D("wt_trig", "Trigger Scale Factor", 200, 0, 2);
  h.hist[41] = new TH1D("wt_pileup", "bJet weight", 200, 0, 2);
  h.hist[42] = new TH1D("wt_bjet", "bJet weight", 200, 0, 2);
  h.hist[43] = new TH1D("wt_evt", "Event weight", 200, 0, 2);

  h.hist[44] = new TH1D("nnscore_qcd_vlld_2016preVFP",  "nnscore_qcd_vlld_2016preVFP", 200, 0, 1);
  h.hist[45] = new TH1D("nnscore_qcd_vlld_2016postVFP", "nnscore_qcd_vlld_2016postVFP", 200, 0, 1);
  h.hist[46] = new TH1D("nnscore_qcd_vlld_2017", "nnscore_qcd_vlld_2017", 200, 0, 1);
  h.hist[47] = new TH1D("nnscore_qcd_vlld_2018", "nnscore_qcd_vlld_2018", 200, 0, 1);
  h.hist[48] = new TH1D("nnscore_ttbar_vlld", "nnscore_ttbar_vlld", 200, 0, 1);
  
  for(int i=0; i<(int)h.hist.size(); i++){
    h.hist[i]->Reset();
    h.hist[i]->Sumw2();
  }
}
