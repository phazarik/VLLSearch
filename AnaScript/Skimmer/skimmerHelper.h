//#include <string>
using namespace std;

void AnaScript::ActivateBranch(TTree *t){
  
  //Make sure to include all branches which are being used in the codes.
  //DON'T USE THIS FUNCTION if you want to keep the same tree structure in the skimmed file.
  //Keepting the same structure is helpful, because the same analysis script works on it.
  
  for(auto activeBranchName : {"run","luminosityBlock","event","Flag_*","nMuon","Muon_*","nElectron","Electron_*","nTau","Tau_*","nJet","Jet_*","MET_*","PuppiMET_*","nTrigObj","TrigObj_*", "nPhoton", "Photon_*", "*fixed*", "PV*"})
    t->SetBranchStatus(activeBranchName, 1);
  
  for(auto activeBranchName : {"HLT_IsoMu*", "HLT_Ele*"}) t->SetBranchStatus(activeBranchName, 1);
  
  if(_data==0){
    for(auto activeBranchName : {"nGenPart","GenPart_*","nGenJet","GenJet_*","nGenVisTau","GenVisTau_*","GenMET_phi","GenMET_pt", "Pileup*"})  //"*LHE*Weight*" is not avaiable in QCD multijet backgrounds.
      t->SetBranchStatus(activeBranchName, 1);
    if(_flag != "qcd"){
      for(auto activeBranchName : {"*LHE*Weight*"}) t->SetBranchStatus(activeBranchName, 1);
    }
  }

  /*
  //For Shalini:
  for(auto activeBranchName : {"HLT*", "nLowPtElectron", "LowPtElectron_*", "nSubJet", "SubJet_*"}) t->SetBranchStatus(activeBranchName, 1);
  if(_data==0){
  for(auto activeBranchName : {"*PSWeight*", "genWeight", "nGenJetAK8", "GenJetAK8_*", "nSubGenJetAK8", "SubGenJetAK8_*", "nGenDressedLepton", "GenDressedLepton_*", "nLHEPart", "LHEPart_*", "*LHE*Weight*"})
  t->SetBranchStatus(activeBranchName, 1);
  }*/
}

void AnaScript::ReadBranch(){
  
  //Only the following branches will be filled.
  //The other branches (containing arrays of numbers) will be empty.
  //IF THE EMPTY BRANCHES ARE TO BE REMOVED, use the ActivateBranch function.
  //But this is risky, because all the branches used in the codes has to be mentioned there.
  //Otherwise it will lead to segmentation violation.
  
  *run;
  *luminosityBlock;
  *event;

  //HLT;
  *HLT_SingleMuon; //Empty pointer, used in the main header.
  *HLT_SingleEle;  //Empty pointer, used in the main header.
  /* 
   *HLT_IsoMu24; //Used in 2018
   *HLT_IsoMu27; //Used in 2017
   *HLT_Ele27_WPTight_Gsf; //Used in 2016
   *HLT_Ele32_WPTight_Gsf; //Not available in 2017
   *HLT_Ele35_WPTight_Gsf; //Used in 2018, not in 2016
   */

  //--------------
  //For Shalini:
  //-------------
  /*
  //HLT paths:
  *HLT_DoubleEle24_eta2p1_WPTight_Gsf;
  *HLT_DoubleEle25_CaloIdL_MW;
  *HLT_DoubleEle27_CaloIdL_MW;
  *HLT_DoubleEle33_CaloIdL_MW;
  *HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_DZ_PFHT350;
  *HLT_DoubleEle8_CaloIdM_TrackIdM_Mass8_PFHT350;
  *HLT_Ele115_CaloIdVT_GsfTrkIdT;
  *HLT_Ele135_CaloIdVT_GsfTrkIdT;
  *HLT_Ele145_CaloIdVT_GsfTrkIdT;
  *HLT_Ele15_Ele8_CaloIdL_TrackIdL_IsoVL;
  *HLT_Ele15_IsoVVVL_PFHT450;
  *HLT_Ele15_IsoVVVL_PFHT450_PFMET50;
  *HLT_Ele15_IsoVVVL_PFHT600;
  *HLT_Ele15_WPLoose_Gsf;
  *HLT_Ele16_Ele12_Ele8_CaloIdL_TrackIdL;
  *HLT_Ele17_CaloIdM_TrackIdM_PFJet30;
  *HLT_Ele17_WPLoose_Gsf;
  *HLT_Ele200_CaloIdVT_GsfTrkIdT;
  *HLT_Ele20_WPLoose_Gsf;
  *HLT_Ele20_WPTight_Gsf;
  *HLT_Ele20_eta2p1_WPLoose_Gsf;
  *HLT_Ele23_CaloIdL_TrackIdL_IsoVL_PFJet30;
  *HLT_Ele23_CaloIdM_TrackIdM_PFJet30;
  *HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL;
  *HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ;
  *HLT_Ele27_Ele37_CaloIdL_MW;
  *HLT_Ele27_WPTight_Gsf;
  *HLT_Ele28_HighEta_SC20_Mass55;
  *HLT_Ele28_eta2p1_WPTight_Gsf_HT150;
  *HLT_Ele300_CaloIdVT_GsfTrkIdT;
  *HLT_Ele30_eta2p1_WPTight_Gsf_CentralPFJet35_EleCleaned;
  *HLT_Ele32_WPTight_Gsf;
  *HLT_Ele32_WPTight_Gsf_L1DoubleEG;
  *HLT_Ele35_WPTight_Gsf;
  *HLT_Ele35_WPTight_Gsf_L1EGMT;
  *HLT_Ele38_WPTight_Gsf;
  *HLT_Ele40_WPTight_Gsf;
  *HLT_IsoMu24;
  *HLT_IsoMu24_eta2p1;
  *HLT_IsoMu27_MET90;
  *HLT_Mu23_Mu12;
  *HLT_Mu23_Mu12_DZ;
  *HLT_Mu23_Mu12_SameSign;
  *HLT_IsoMu24_eta2p1;

  //Gen level branches:
  if(_data == 0){
    *nPSWeight;
    for(unsigned int i=0; i<(unsigned int)*nPSWeight; i++) PSWeight[i];
    *genWeight;
    for(unsigned int i=0; i<(unsigned int)*nGenJet; i++){
      GenJet_partonFlavour[i];
      GenJet_hadronFlavour[i];
    }
    *nGenJetAK8;
    for(unsigned int i=0; i<(unsigned int)*nGenJetAK8; i++){
      GenJetAK8_eta[i];
      GenJetAK8_mass[i];
      GenJetAK8_phi[i];
      GenJetAK8_pt[i];
      GenJetAK8_partonFlavour[i];
      GenJetAK8_hadronFlavour[i];
    }
    *nSubGenJetAK8;
    for(unsigned int i=0; i<(unsigned int)*nSubGenJetAK8; i++){
      SubGenJetAK8_eta[i];
      SubGenJetAK8_mass[i];
      SubGenJetAK8_phi[i];
      SubGenJetAK8_pt[i];
    }
    *GenMET_phi;
    *GenMET_pt;
    for(unsigned int i=0; i<(unsigned int)*nMuon; i++){
      Muon_genPartIdx[i];
    }
    for(unsigned int i=0; i<(unsigned int)*nElectron; i++){
      Electron_genPartIdx[i];
      Electron_genPartFlav[i];
    }
    *nGenDressedLepton;
    for(unsigned int i=0; i<(unsigned int)*nGenDressedLepton; i++){
      GenDressedLepton_eta[i];
      GenDressedLepton_mass[i];
      GenDressedLepton_phi[i];
      GenDressedLepton_pt[i];
      GenDressedLepton_pdgId[i];
      GenDressedLepton_hasTauAnc[i];
    }
    *nLHEPart;
    for(unsigned int i=0; i<(unsigned int)*nLHEPart; i++){
      LHEPart_pt[i];
      LHEPart_eta[i];
      LHEPart_phi[i];
      LHEPart_mass[i];
      LHEPart_incomingpz[i];
      LHEPart_pdgId[i];
      LHEPart_status[i];
      LHEPart_spin[i];
    }
    *LHEWeight_originalXWGTUP;
    *nLHEPdfWeight;
    for(unsigned int i=0; i<(unsigned int)*nLHEPdfWeight; i++) LHEPdfWeight[i];
    *nLHEReweightingWeight;
    for(unsigned int i=0; i<(unsigned int)*nLHEReweightingWeight; i++) LHEReweightingWeight[i];
    *nLHEScaleWeight;
    for(unsigned int i=0; i<(unsigned int)*nLHEScaleWeight; i++) LHEScaleWeight[i];
  }
  //non-gen
  *nSubJet;
  for(unsigned int i=0; i<(unsigned int)*nSubJet; i++){
    SubJet_btagCSVV2[i];
    SubJet_btagDeepB[i];
    SubJet_eta[i];
    SubJet_mass[i];
    SubJet_n2b1[i];
    SubJet_n3b1[i];
    SubJet_phi[i];
    SubJet_pt[i];
    SubJet_rawFactor[i];
    SubJet_tau1[i];
    SubJet_tau2[i];
    SubJet_tau3[i];
    SubJet_tau4[i];
  }
  *nLowPtElectron;
  for(unsigned int i=0; i<(unsigned int)*nLowPtElectron; i++){
    LowPtElectron_ID[i];
    LowPtElectron_convVtxRadius[i];
    LowPtElectron_deltaEtaSC[i];
    LowPtElectron_dxy[i];
    LowPtElectron_dxyErr[i];
    LowPtElectron_dz[i];
    LowPtElectron_dzErr[i];
    LowPtElectron_eInvMinusPInv[i];
    LowPtElectron_embeddedID[i];
    LowPtElectron_energyErr[i];
    LowPtElectron_eta[i];
    LowPtElectron_hoe[i];
    LowPtElectron_mass[i];
    LowPtElectron_miniPFRelIso_all[i];
    LowPtElectron_miniPFRelIso_chg[i];
    LowPtElectron_phi[i];
    LowPtElectron_pt[i];
    LowPtElectron_ptbiased[i];
    LowPtElectron_r9[i];
    LowPtElectron_scEtOverPt[i];
    LowPtElectron_sieie[i];
    LowPtElectron_unbiased[i];
    LowPtElectron_charge[i];
    LowPtElectron_convWP[i];
    LowPtElectron_pdgId[i];
    LowPtElectron_convVeto[i];
    LowPtElectron_lostHits[i];
    }*/

  //TrigObj
  *nTrigObj;
  for(unsigned int i=0; i<(unsigned int)*nTrigObj; i++){
    TrigObj_pt[i];
    TrigObj_eta[i];
    TrigObj_phi[i];
    TrigObj_l1pt[i];
    TrigObj_l1pt_2[i];
    TrigObj_l2pt[i];
    TrigObj_id[i];
    TrigObj_l1iso[i];
    TrigObj_l1charge[i];
    TrigObj_filterBits[i];
  }

  //Flags
  *Flag_HBHENoiseFilter;
  *Flag_HBHENoiseIsoFilter;
  *Flag_CSCTightHaloFilter;
  *Flag_CSCTightHaloTrkMuUnvetoFilter;
  *Flag_CSCTightHalo2015Filter;
  *Flag_globalTightHalo2016Filter;
  *Flag_globalSuperTightHalo2016Filter;
  *Flag_HcalStripHaloFilter;
  *Flag_hcalLaserEventFilter;
  *Flag_EcalDeadCellTriggerPrimitiveFilter;
  *Flag_EcalDeadCellBoundaryEnergyFilter;
  *Flag_ecalBadCalibFilter;
  *Flag_goodVertices;
  *Flag_eeBadScFilter;
  *Flag_ecalLaserCorrFilter;
  *Flag_trkPOGFilters;
  *Flag_chargedHadronTrackResolutionFilter;
  *Flag_muonBadTrackFilter;
  *Flag_BadChargedCandidateFilter;
  *Flag_BadPFMuonFilter;
  //*Flag_BadPFMuonDzFilter;
  //*Flag_hfNoisyHitsFilter;
  *Flag_BadChargedCandidateSummer16Filter;
  *Flag_BadPFMuonSummer16Filter;
  *Flag_trkPOG_manystripclus53X;
  *Flag_trkPOG_toomanystripclus53X;
  *Flag_trkPOG_logErrorTooManyClusters;
  *Flag_METFilters;

  //For Jet corrections:
  **ptr_fixedGridRhoFastjetAll;
  **ptr_fixedGridRhoFastjetCentral;
  **ptr_fixedGridRhoFastjetCentralCalo;
  **ptr_fixedGridRhoFastjetCentralChargedPileUp;
  **ptr_fixedGridRhoFastjetCentralNeutral;
  
  //PFMET
  *MET_MetUnclustEnUpDeltaX;
  *MET_MetUnclustEnUpDeltaY;
  *MET_covXX;
  *MET_covXY;
  *MET_covYY;
  *MET_phi;
  *MET_pt;
  *MET_significance;
  *MET_sumEt;
  *MET_sumPtUnclustered;

  //PuppiMET
  *PuppiMET_phi;
  //*PuppiMET_phiJERDown;
  *PuppiMET_phiJERUp;
  //*PuppiMET_phiJESDown;
  *PuppiMET_phiJESUp;
  //*PuppiMET_phiUnclusteredDown;
  //*PuppiMET_phiUnclusteredUp;
  *PuppiMET_pt;
  //*PuppiMET_ptJERDown;
  *PuppiMET_ptJERUp;
  //*PuppiMET_ptJESDown;
  *PuppiMET_ptJESUp;
  //*PuppiMET_ptUnclusteredDown;
  //*PuppiMET_ptUnclusteredUp;
  *PuppiMET_sumEt;

  //PV
  *PV_npvs;
  *PV_npvsGood;
  *PV_x;
  *PV_y;
  *PV_z;

  if(_data==0){
    *Pileup_nPU;
    *Pileup_sumEOOT;
    *Pileup_sumLOOT;
    *Pileup_nTrueInt; //important for pileup correction
    *Pileup_gpudensity;

    if(_flag != "qcd"){
      **ptr_LHEWeight_originalXWGTUP;
      **ptr_nLHEPdfWeight;         //iterator
      **ptr_nLHEReweightingWeight; //iterator
      **ptr_nLHEScaleWeight;       //iterator
      for(unsigned int i=0; i<(unsigned int)**ptr_nLHEPdfWeight; i++)          (*ptr_LHEPdfWeight)[i];
      for(unsigned int i=0; i<(unsigned int)**ptr_nLHEReweightingWeight; i++ ) (*ptr_LHEReweightingWeight)[i];
      for(unsigned int i=0; i<(unsigned int)**ptr_nLHEScaleWeight; i++ )       (*ptr_LHEScaleWeight)[i];
    }
    
  }
  
  //Electron Branches:
  *nElectron;
  for(unsigned int i=0;i<(unsigned int)*nElectron;i++){
    Electron_dxy[i];
    Electron_dxyErr[i];
    Electron_dz[i];
    Electron_dzErr[i];
    Electron_deltaEtaSC[i];
    Electron_dr03EcalRecHitSumEt[i];
    Electron_dr03HcalDepth1TowerSumEt[i];
    Electron_dr03TkSumPt[i];
    Electron_dr03TkSumPtHEEP[i];
    //Electron_eCorr[i]; //Run2only
    Electron_eInvMinusPInv[i];
    Electron_energyErr[i];
    Electron_eta[i];
    Electron_hoe[i];
    Electron_ip3d[i];
    Electron_jetPtRelv2[i];
    Electron_jetRelIso[i];
    Electron_mass[i];
    Electron_miniPFRelIso_all[i];
    Electron_miniPFRelIso_chg[i];
    //Electron_mvaFall17V2Iso[i]; //Run2only
    //Electron_mvaFall17V2noIso[i]; //Run2only
    Electron_pfRelIso03_all[i];
    Electron_pfRelIso03_chg[i];
    Electron_phi[i];
    Electron_pt[i];
    Electron_r9[i];
    Electron_scEtOverPt[i];
    Electron_sieie[i];
    Electron_sip3d[i];
    Electron_mvaTTH[i];
    Electron_charge[i];
    Electron_cutBased[i];
    Electron_jetIdx[i];
    Electron_pdgId[i];
    Electron_photonIdx[i];
    Electron_tightCharge[i];
    Electron_vidNestedWPBitmap[i];
    Electron_vidNestedWPBitmapHEEP[i];
    Electron_convVeto[i];
    Electron_cutBased_HEEP[i];
    Electron_isPFcand[i];
    //Electron_lostHits[i]; //Run2only
    //Electron_mvaFall17V2Iso_WP80[i]; //Run2only
    //Electron_mvaFall17V2Iso_WP90[i]; //Run2only
    //Electron_mvaFall17V2Iso_WPL[i]; //Run2only
    //Electron_mvaFall17V2noIso_WP80[i]; //Run2only
    //Electron_mvaFall17V2noIso_WP90[i]; //Run2only
    //Electron_mvaFall17V2noIso_WPL[i]; //Run2only
    Electron_seedGain[i];
    // Electron_dEscaleDown[i]; //Run2only
    // Electron_dEscaleUp[i]; //Run2only
    // Electron_dEsigmaDown[i]; //Run2only
    // Electron_dEsigmaUp[i]; //Run2only
    // Electron_jetNDauCharged[i] //Run2only
  }

  //Muon Branches
  *nMuon;
  for(unsigned int i=0;i<(unsigned int)*nMuon;i++){
    Muon_dxy[i];
    Muon_dxyErr[i];
    Muon_dxybs[i];
    Muon_dz[i];
    Muon_dzErr[i];
    Muon_eta[i];
    Muon_ip3d[i];
    Muon_jetPtRelv2[i];
    Muon_jetRelIso[i];
    Muon_mass[i];
    Muon_miniPFRelIso_all[i];
    Muon_miniPFRelIso_chg[i];
    Muon_pfRelIso03_all[i];
    Muon_pfRelIso03_chg[i];
    Muon_pfRelIso04_all[i];
    Muon_phi[i];
    Muon_pt[i];
    Muon_ptErr[i];
    Muon_segmentComp[i];
    Muon_sip3d[i];
    //Muon_softMva[i]; //Run2only
    Muon_tkRelIso[i];
    Muon_tunepRelPt[i];
    Muon_mvaLowPt[i];
    Muon_mvaTTH[i];
    Muon_charge[i];
    Muon_jetIdx[i];
    Muon_nStations[i];
    Muon_nTrackerLayers[i];
    Muon_pdgId[i];
    Muon_tightCharge[i];
    Muon_fsrPhotonIdx[i];
    Muon_highPtId[i];
    Muon_highPurity[i];
    Muon_inTimeMuon[i];
    Muon_isGlobal[i];
    Muon_isPFcand[i];
    //Muon_isStandalone[i]; //Run2only
    Muon_isTracker[i];
    //Muon_jetNDauCharged[i]; //Run2only
    Muon_looseId[i];
    Muon_mediumId[i];
    Muon_mediumPromptId[i];
    Muon_miniIsoId[i];
    Muon_multiIsoId[i];
    //Muon_mvaId[i]; //Run2only
    //Muon_mvaLowPtId[i]; //Run2only
    Muon_pfIsoId[i];
    //Muon_puppiIsoId[i]; //Run2only
    Muon_softId[i];
    Muon_softMvaId[i];
    Muon_tightId[i];
    Muon_tkIsoId[i];
    Muon_triggerIdLoose[i];    
  }
 
  //Jets
  *nJet;
  for(unsigned int i=0; i<(unsigned int)*nJet;i++){
    Jet_area[i];
    //Jet_btagCSVV2[i]; //Run2only
    //Jet_btagDeepB[i]; //Run2only
    //Jet_btagDeepCvB[i]; //Run2only
    //Jet_btagDeepCvL[i]; //Run2only
    Jet_btagDeepFlavB[i];
    //Jet_btagDeepFlavCvB[i]; //Run2only
    //Jet_btagDeepFlavCvL[i]; //Run2only
    //Jet_btagDeepFlavQG[i]; //Run2only
    Jet_chEmEF[i];
    //Jet_chFPV0EF[i]; //Run2only
    Jet_chHEF[i];
    Jet_eta[i];
    //Jet_hfsigmaEtaEta[i]; //Run2only
    //Jet_hfsigmaPhiPhi[i]; //Run2only
    Jet_mass[i];
    Jet_muEF[i];
    Jet_muonSubtrFactor[i];
    Jet_neEmEF[i];
    Jet_neHEF[i];
    Jet_phi[i];
    Jet_pt[i];
    //Jet_puIdDisc[i]; //Run2only
    //Jet_qgl[i]; //Run2only
    //Jet_rawFactor[i]; //Run2only
    //Jet_bRegCorr[i]; //Run2only
    //Jet_bRegRes[i]; //Run2only
    //Jet_cRegCorr[i]; //Run2only
    //Jet_cRegRes[i]; //Run2only
    Jet_electronIdx1[i];
    Jet_electronIdx2[i];
    //Jet_hfadjacentEtaStripsSize[i]; //Run2only
    //Jet_hfcentralEtaStripSize[i]; //Run2only
    Jet_jetId[i];
    Jet_muonIdx1[i];
    Jet_muonIdx2[i];
    Jet_nElectrons[i];
    Jet_nMuons[i];
    //Jet_puId[i]; //Run2only
    //Jet_nConstituents[i]; //Run2only
    if(_data==0) Jet_hadronFlavour[i]; //Important for b-tagging corrections, gen-only. Use it in the main code.
  }

  //Photons:
  *nPhoton;
  for(unsigned int i=0; i<(unsigned int)*nPhoton;i++){
    //Photon_dEscaleDown[i]; //Run2only
    //Photon_dEscaleUp[i]; //Run2only
    //Photon_dEsigmaDown[i]; //Run2only
    //Photon_dEsigmaUp[i]; //Run2only
    //Photon_eCorr[i]; //Run2only
    Photon_energyErr[i];
    Photon_eta[i];
    Photon_hoe[i];
    //Photon_mass[i]; //Run2only
    Photon_mvaID[i];
    //Photon_mvaID_Fall17V1p1[i]; //Run2only
    Photon_pfRelIso03_all[i];
    Photon_pfRelIso03_chg[i];
    Photon_phi[i];
    Photon_pt[i];
    Photon_r9[i];
    Photon_sieie[i];
    //Photon_charge[i]; //Run2only
    Photon_cutBased[i];
    //Photon_cutBased_Fall17V1Bitmap[i];  //Run2only
    Photon_electronIdx[i];
    Photon_jetIdx[i];
    // Photon_pdgId[i]; //Run2only
    Photon_vidNestedWPBitmap[i];
    Photon_electronVeto[i];
    Photon_isScEtaEB[i];
    Photon_isScEtaEE[i];
    Photon_mvaID_WP80[i];
    Photon_mvaID_WP90[i];
    Photon_pixelSeed[i];
    Photon_seedGain[i];
  }
  
}
