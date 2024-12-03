//The scale factors are loaded globally in the SlaveBegin() funtion, before running the Process().

//---------------------------------------
// READ TXT FILES AND LOAD CORRECTIONS
//---------------------------------------
void AnaScript::LoadCorrectionsFromPOG(){

  //Global variables to fill:
  //muonIDSF, muonIsoSF;
  //electronIDSF;
  
  //Accessing correction files:
  TString basedir = "/home/work/phazarik1/work/VLLSearch/Corrections/SFfromPOG/";

  //Muon ID SF:  
  TString muon_id_sf = basedir + "muon_id_sf.txt";
  ifstream infile_muonidsf(muon_id_sf.Data());
  sftxt line_muonidsf;
  while (infile_muonidsf >> line_muonidsf.campaign
	 >> line_muonidsf.etalow >> line_muonidsf.etahigh
	 >> line_muonidsf.ptlow  >> line_muonidsf.pthigh
	 >> line_muonidsf.sfdown >> line_muonidsf.sf >> line_muonidsf.sfup) {
    muonIDSF.push_back(line_muonidsf);
  }
  infile_muonidsf.close();
  cout<<"ScaleFactors loaded: muon (ID)"<<endl;

  //Muon Iso SF:
  TString muon_iso_sf = basedir + "muon_iso_sf.txt";
  ifstream infile_muonisosf(muon_iso_sf.Data());
  sftxt line_muonisosf;
  while (infile_muonisosf >> line_muonisosf.campaign
	 >> line_muonisosf.etalow >> line_muonisosf.etahigh
	 >> line_muonisosf.ptlow  >> line_muonisosf.pthigh
	 >> line_muonisosf.sfdown >> line_muonisosf.sf >> line_muonisosf.sfup) {
    muonIsoSF.push_back(line_muonisosf);
  }
  infile_muonisosf.close();
  cout<<"ScaleFactors loaded: muon (Iso)"<<endl;

  //Electron ID SF:
  TString electron_id_sf = basedir + "electron_id_sf.txt";
  ifstream infile_eleidsf(electron_id_sf.Data());
  sftxt line_eleidsf;
  while (infile_eleidsf >> line_eleidsf.campaign
	 >> line_eleidsf.etalow >> line_eleidsf.etahigh
	 >> line_eleidsf.ptlow  >> line_eleidsf.pthigh
	 >> line_eleidsf.sfdown >> line_eleidsf.sf >> line_eleidsf.sfup) {
    electronIDSF.push_back(line_eleidsf);
  }
  infile_eleidsf.close();
  cout<<"ScaleFactors loaded: electron (ID)"<<endl;

  //Jet JEC SF:
  TString jet_jec_sf = basedir + "jet_jec_sf.txt";
  ifstream infile_jecsf(jet_jec_sf.Data());
  sftxt line_jecsf;
  while (infile_jecsf >> line_jecsf.campaign
	 >> line_jecsf.etalow >> line_jecsf.etahigh
	 >> line_jecsf.ptlow  >> line_jecsf.pthigh
	 >> line_jecsf.sfdown >> line_jecsf.sf >> line_jecsf.sfup) {
    jetJECSF.push_back(line_jecsf);
  }
  infile_jecsf.close();
  cout<<"ScaleFactors loaded: jet (JEC)"<<endl;
  
  //Jet JER SF:
  TString jet_jer_sf = basedir + "jet_jer_sf.txt";
  ifstream infile_jersf(jet_jer_sf.Data());
  sftxt line_jersf;
  while (infile_jersf >> line_jersf.campaign
	 >> line_jersf.etalow >> line_jersf.etahigh
	 >> line_jersf.sfdown >> line_jersf.sf >> line_jersf.sfup) {
    jetJERSF.push_back(line_jersf);
  }
  infile_jersf.close();
  cout<<"ScaleFactors loaded: jet (JER)"<<endl;

  //Jet pTres SF:
  TString jet_ptres_sf = basedir + "jet_ptres_sf.txt";
  ifstream infile_ptressf(jet_ptres_sf.Data());
  sftxt line_ptressf;
  while (infile_ptressf >> line_ptressf.campaign
	 >> line_ptressf.etalow >> line_ptressf.etahigh
	 >> line_ptressf.ptlow >> line_ptressf.pthigh
	 >> line_ptressf.rholow >> line_ptressf.rhohigh
	 >> line_ptressf.sf) {
    jetPtResSF.push_back(line_ptressf);
  }
  infile_ptressf.close();
  cout<<"ScaleFactors loaded: jet (pTres)"<<endl;

  //b-tagging efficiencies (POG):
  TString bjet_eff = basedir + "bjet_mujets_and_incl_eff.txt";
  ifstream infile_bjeteff(bjet_eff.Data());
  sftxt line_bjeteff;
  while (infile_bjeteff >> line_bjeteff.campaign
	 >> line_bjeteff.etalow >> line_bjeteff.etahigh
	 >> line_bjeteff.ptlow  >> line_bjeteff.pthigh
	 >> line_bjeteff.flavor
	 >> line_bjeteff.sfdown >> line_bjeteff.sf >> line_bjeteff.sfup) {
    bJetEffPOG.push_back(line_bjeteff);
  }
  infile_bjeteff.close();
  cout<<"POG efficiencies loaded (b-tagging)."<<endl;
}

//------------------------------------------------
// Load Correction factors for individual objects:
//------------------------------------------------

double AnaScript::returnMuonIDSF(Particle muon, TString mode){
  float pt  = muon.v.Pt(); 
  float eta = fabs(muon.v.Eta());
  for(const auto& entry : muonIDSF) {
    bool campaign_match = entry.campaign == _campaign;
    bool in_pteta_range = (entry.etalow <= eta && eta < entry.etahigh) && 
      (entry.ptlow  <= pt  && pt  < entry.pthigh);
    if(campaign_match){
      if(in_pteta_range){
        if (mode == "nom")           return entry.sf;
        else if (mode == "systup")   return entry.sfup;
        else if (mode == "systdown") return entry.sfdown;
        else {
          cout << "Error: Unknown mode: " << mode <<endl;
          return 1.0;
        }
      }
      else continue;
    }
  }
  // If no campaign matches or no range matches after the loop, return -1.0
  cout << "\033[031mError: No mathing SF (muon ID), pt = " << pt << ", eta=" << eta << ", campaign=" << _campaign << "\033[0m"<< endl;
  return -1.0;
}

double AnaScript::returnMuonIsoSF(Particle muon, TString mode){
  float pt  = muon.v.Pt();
  float eta = fabs(muon.v.Eta());
  for(const auto& entry : muonIsoSF) {
    bool campaign_match = entry.campaign == _campaign;
    bool in_pteta_range = (entry.etalow <= eta && eta < entry.etahigh) && 
                          (entry.ptlow  <= pt  && pt  < entry.pthigh);
    if(campaign_match){
      if(in_pteta_range){
        if (mode == "nom")           return entry.sf;
        else if (mode == "systup")   return entry.sfup;
        else if (mode == "systdown") return entry.sfdown;
        else {
          cout << "Error: Unknown mode: " << mode <<endl;
          return 1.0;
        }
      }
      else continue;
    }
  }
  // If no campaign matches or no range matches after the loop, return -1.0
  cout << "\033[031mError: No mathing SF (muon Iso), pt = " << pt << ", eta=" << eta << ", campaign=" << _campaign <<"\033[0m"<<endl;
  return -1.0;
}

double AnaScript::returnElectronIDSF(Particle ele, TString mode){
  float pt  = ele.v.Pt();
  float eta = ele.v.Eta();
  for(const auto& entry : electronIDSF) {
    bool campaign_match = entry.campaign == _campaign;
    bool in_pteta_range = (entry.etalow <= eta && eta < entry.etahigh) &&
                          (entry.ptlow  <= pt  && pt  < entry.pthigh);
    if(campaign_match){
      if(in_pteta_range){
        if (mode == "nom")           return entry.sf;
        else if (mode == "systup")   return entry.sfup;
        else if (mode == "systdown") return entry.sfdown;
        else {
          cout << "Error: Unknown mode: " << mode <<endl;
          return 1.0;
        }
      }
      else continue;
    }
  }
  // If no campaign matches or no range matches after the loop, return -1.0
  cout << "\033[033mError: No mathing SF (ele ID), pt = " << pt << ", eta=" << eta << ", campaign=" << _campaign <<"\033[0m"<<endl;
  return -1.0;
}

double AnaScript::returnJetJECSF(Particle jet, TString mode){
  float pt  = jet.v.Pt();
  float eta = jet.v.Eta();
  for(const auto& entry : jetJECSF) {
    bool campaign_match = entry.campaign == _campaign;
    bool in_pteta_range = (entry.etalow <= eta && eta < entry.etahigh) &&
                          (entry.ptlow  <= pt  && pt  < entry.pthigh);
    if(campaign_match){
      if(in_pteta_range){
        if (mode == "nom")           return entry.sf;
        else if (mode == "systup")   return entry.sfup;
        else if (mode == "systdown") return entry.sfdown;
        else {
          cout << "Error: Unknown mode: " << mode <<endl;
          return 1.0;
        }
      }
      else continue;
    }
  }
  // If no campaign matches or no range matches after the loop, return -1.0
  cout << "\033[033mError: No mathing SF (jet JEC), pt = " << pt << ", eta=" << eta << ", campaign=" << _campaign <<"\033[0m"<<endl;
  return -1.0;
}

double AnaScript::returnJetJERSF(Particle jet, TString mode){
  float eta = jet.v.Eta();
  for(const auto& entry : jetJERSF) {
    bool campaign_match = entry.campaign == _campaign;
    bool in_eta_range = (entry.etalow <= eta && eta < entry.etahigh);
    if(campaign_match){
      if(in_eta_range){
        if (mode == "nom")           return entry.sf;
        else if (mode == "systup")   return entry.sfup;
        else if (mode == "systdown") return entry.sfdown;
        else {
          cout << "Error: Unknown mode: " << mode <<endl;
          return 1.0;
        }
      }
      else continue;
    }
  }
  // If no campaign matches or no range matches after the loop, return -1.0
  cout << "\033[033mError: No mathing SF (jet JER), eta=" << eta << ", campaign=" << _campaign <<"\033[0m"<<endl;
  return -1.0;
}

double AnaScript::returnJetPtResSF(Particle jet){
  float pt  = jet.v.Pt();
  float eta = jet.v.Eta();
  float rho = 0.0;
  if(ptr_fixedGridRhoFastjetAll != nullptr) rho = **ptr_fixedGridRhoFastjetAll;

  for(const auto& entry : jetPtResSF) {
    bool campaign_match = entry.campaign == _campaign;
    bool in_ptetarho_range = (entry.etalow <= eta && eta < entry.etahigh) &&
                          (entry.ptlow  <= pt  && pt  < entry.pthigh) &&
                          (entry.rholow <= rho && rho < entry.rhohigh);

    if(campaign_match){
      if(in_ptetarho_range){
        return entry.sf;
      }
      else continue;
    }
  }
  // If no campaign matches or no range matches after the loop, return -1.0
  cout << "\033[033mError: No mathing SF (jet pTres), pt = " << pt << ", eta = " << eta <<", rho = "<<rho<<", campaign=" << _campaign <<"\033[0m"<<endl;
  return -1.0;
}

double AnaScript::returnbJetTaggingSFPOG(Particle jet, TString mode){
  float pt  = jet.v.Pt();
  float eta = fabs(jet.v.Eta());
  int flavor = jet.hadronflavor;
  for(const auto& entry : bJetEffPOG) {
    bool campaign_match = entry.campaign == _campaign;
    bool in_pteta_range = (entry.etalow <= eta && eta < entry.etahigh) &&
                          (entry.ptlow  <= pt  && pt  < entry.pthigh);
    bool flavor_match = int(entry.flavor) == flavor; 
    if(campaign_match){
      if(in_pteta_range && flavor_match){
        if (mode == "nom")           return entry.sf;
        else if (mode == "systup")   return entry.sfup;
        else if (mode == "systdown") return entry.sfdown;
        else {
          cout << "Error: Unknown mode: " << mode <<endl;
          return 1.0;
        }
      }
      else continue;
    }
  }
  // If no campaign matches or no range matches after the loop, return -1.0
  cout << "\033[033mError: No mathing eff (btagging), pt = " << pt << ", eta=" << eta << ", flavor = "<< flavor << ", campaign=" << _campaign <<"\033[0m"<<endl;
  return -1.0;
}

