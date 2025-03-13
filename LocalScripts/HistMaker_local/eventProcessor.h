#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>
#include <vector>
#include <string>
#include "setBranchesAndHistograms.h"
#include <nlohmann/json.hpp>
using json = nlohmann::json;
using namespace std;
/*
extern Long64_t channel, trigger, nlep, njet, nbjet;
extern Double_t lep0_pt, lep0_eta, lep0_phi, lep0_iso, lep0_sip3d, lep0_mt;
extern Double_t lep1_pt, lep1_eta, lep1_phi, lep1_iso, lep1_sip3d, lep1_mt;
extern Double_t dilep_pt, dilep_eta, dilep_phi, dilep_mass, dilep_mt, dilep_deta, dilep_dphi, dilep_dR, dilep_ptratio;
extern Double_t HT, LT, STvis, ST, HTMETllpt, STfrac, metpt, metphi;
extern Double_t dphi_metlep0, dphi_metlep1, dphi_metdilep, dphi_metlep_max, dphi_metlep_min;
extern Double_t wt_leptonSF, wt_trig, wt_pileup, wt_bjet, weight;
extern Double_t nnscore1, nnscore2, nnscore3, nnscore4, nnscore5;*/
extern Int_t channel, trigger, nlep, njet, nbjet;
extern Float_t lep0_pt, lep0_eta, lep0_phi, lep0_iso, lep0_sip3d, lep0_mt;
extern Float_t lep1_pt, lep1_eta, lep1_phi, lep1_iso, lep1_sip3d, lep1_mt;
extern Float_t dilep_pt, dilep_eta, dilep_phi, dilep_mass, dilep_mt, dilep_deta, dilep_dphi, dilep_dR, dilep_ptratio;
extern Float_t HT, LT, STvis, ST, HTMETllpt, STfrac, metpt, metphi;
extern Float_t dphi_metlep0, dphi_metlep1, dphi_metdilep, dphi_metlep_max, dphi_metlep_min;
extern Float_t nnscore1, nnscore2, nnscore3, nnscore4, nnscore5;
extern Double_t wt_leptonSF, wt_trig, wt_pileup, wt_bjet, weight;

struct hists{
  TString name;
  TString title;
  Int_t nbins;
  Double_t xmin;
  Double_t xmax;
  std::vector<float> binning;  // For custom binning
};

double getScaleFactorInBins(const char* campaign_cstr, int channelval, double var, const json& scale_factors) {
  std::string campaign(campaign_cstr);
  std::string channel_key = std::to_string(channelval);
  if (scale_factors.contains(campaign) && scale_factors[campaign].contains(channel_key)) {
    const auto& ranges = scale_factors[campaign][channel_key];
    for (const auto& range : ranges) {
      double low = range["low"];
      double high = 0.0;
      if (range["high"].is_string() && range["high"] == "inf") high = std::numeric_limits<double>::infinity();
      else high = range["high"];
      double scale = range["scale"];
      if (var >= low && var < high) return scale;
    }
  }
  return 1.0;
}

double getScaleFactorGlobal(const char* campaign_cstr, int channelval, const json& scale_factors) {
  std::string campaign(campaign_cstr);
  std::string channel_key = std::to_string(channelval);
  if (scale_factors.contains(campaign) && scale_factors[campaign].contains(channel_key)) {
    return scale_factors[campaign][channel_key];
  }
  return 1.0;
}

void fillHistogram(TH1F* hist, float value, float weight = 1.0) {
    if      (value < hist->GetXaxis()->GetXmin()) hist->Fill(hist->GetXaxis()->GetXmin(), weight);
    else if (value > hist->GetXaxis()->GetXmax()) hist->Fill(hist->GetXaxis()->GetXmax(), weight);
    else     hist->Fill(value, weight);
}

//________________________________________________________________________________________________________________
//
// Process one file:
//________________________________________________________________________________________________________________


void processTree(
		 const char* inputFilename,
		 const char* outputFilename,
		 const char* campaign,
		 int channelval,
		 float lumisf)
{ 
  // Load corrections from JSON:
  std::ifstream json_ttbar("corrections/TTBar_HTbinned_corrections.json");
  std::ifstream json_qcd("corrections/QCD_global_corrections.json");
  json sf_ttbar, sf_qcd;
  json_ttbar >> sf_ttbar;
  json_qcd >> sf_qcd;
  cout << "Corrections loaded from JSON." << endl;
  
  //setBranches(tree);
  vector<TH1D*> hst_collection;

  vector<hists> hdef = {
    // integers:
    {"channel", "channel", 10, 0, 10, {}},
    {"trigger", "trigger", 10, 0, 10, {}},
    {"nlep", "nlep", 10, 0, 10, {}},
    {"njet", "njet", 10, 0, 10, {}},
    {"nbjet", "nbjet", 10, 0, 10, {}},
    // leading lepton:
    {"lep0_pt", "lep0_pt", 50, 0, 500, {}},
    {"lep0_eta", "lep0_eta", 100, -4, 4, {}},
    {"lep0_phi", "lep0_phi", 100, -4, 4, {}},
    {"lep0_iso", "lep0_iso", 200, 0, 0.2, {}},
    {"lep0_sip3d", "lep0_sip3d", 200, 0, 50, {}},
    {"lep0_mt", "lep0_mt", 50, 0, 500, {}},
    // subleading lepton:
    {"lep1_pt", "lep1_pt", 50, 0, 500, {}},
    {"lep1_eta", "lep1_eta", 100, -4, 4, {}},
    {"lep1_phi", "lep1_phi", 100, -4, 4, {}},
    {"lep1_iso", "lep1_iso", 200, 0, 0.2, {}},
    {"lep1_sip3d", "lep1_sip3d", 200, 0, 50, {}},
    {"lep1_mt", "lep1_mt", 50, 0, 500, {}},
    // dilepton system:
    {"dilep_pt", "dilep_pt", 0, 0, 0, {0, 25, 50, 100, 200, 300, 400, 500}}, //variable binning
    {"dilep_eta", "dilep_eta", 100, -10, 10, {}},
    {"dilep_phi", "dilep_phi", 100, -4, 4, {}},
    {"dilep_mass", "dilep_mass", 50, 0, 500, {}},
    {"dilep_mt", "dilep_mt", 0, 0, 0, {0, 25, 50, 100, 200, 300, 400, 500}}, //variable binning
    {"dilep_deta", "dilep_deta", 100, 0, 6, {}},
    {"dilep_dphi", "dilep_dphi", 100, 0, 6, {}},
    {"dilep_dR", "dilep_dR", 100, 0, 6, {}},
    {"dilep_ptratio", "dilep_ptratio", 100, 0, 1, {}},
    // event level variables:
    {"HT",    "HT",    0, 0, 0, {0, 25, 50, 100, 200, 300, 400, 500}}, //variable binning
    {"LT",    "LT",    0, 0, 0, {0, 25, 50, 100, 200, 300, 400, 500}}, //variable binning
    {"STvis", "STvis", 0, 0, 0, {0, 25, 50, 100, 200, 300, 400, 500}}, //variable binning
    {"ST",    "ST",    0, 0, 0, {0, 25, 50, 100, 200, 300, 400, 500}}, //variable binning
    {"HTMETllpt", "HTMETllpt", 0, 0, 0, {0, 25, 50, 100, 200, 300, 400, 500}}, //variable binning
    {"STfrac", "STfrac", 100, 0, 1.1, {}},
    {"metpt", "metpt", 0, 0, 0, {0, 25, 50, 100, 200, 300, 400, 500}}, //variable binning
    {"metphi", "metphi", 100, -4, 4, {}},
    // dphis:
    {"dphi_metlep0", "dphi_metlep0", 100, 0, 4, {}},
    {"dphi_metlep1", "dphi_metlep1", 100, 0, 4, {}},
    {"dphi_metdilep", "dphi_metdilep", 100, 0, 4, {}},
    {"dphi_metlep_max", "dphi_metlep_max", 100, 0, 4, {}},
    {"dphi_metlep_min", "dphi_metlep_min", 100, 0, 4, {}},
    // weights:
    {"2LSS_wt_leptonSF", "wt_leptonSF", 200, 0, 2, {}},
    {"2LSS_wt_trig", "wt_trig", 200, 0, 2, {}},
    {"2LSS_wt_pileup", "wt_pileup", 200, 0, 2, {}},
    {"2LSS_wt_bjet", "wt_bjet", 200, 0, 2, {}},
    {"2LSS_wt_evt", "weight", 200, 0, 2, {}},
    // nnscores:
    {"nnscore_qcd_vlld_2016preVFP",  "nnscore_qcd_vlld_2016preVFP",   200, 0, 1, {}},
    {"nnscore_qcd_vlld_2016postVFP", "nnscore_qcd_vlld_2016postVFP",  200, 0, 1, {}},
    {"nnscore_qcd_vlld_2017",        "nnscore_qcd_vlld_2017",         200, 0, 1, {}},
    {"nnscore_qcd_vlld_2018",        "nnscore_qcd_vlld_2018",         200, 0, 1, {}},
    {"nnscore_ttbar_vlld",           "nnscore_ttbar_vlld",            200, 0, 1, {}},
  };
    
  //Booking histograms:
  for (int i=0; i<(int)hdef.size(); i++) {
    TH1D* hist;
    //cout<<hdef[i].name <<"\t"<< hdef[i].title <<"\t"<< hdef[i].nbins <<"\t"<< hdef[i].xmin <<"\t"<< hdef[i].xmax<<endl;
    if ((int)hdef[i].binning.size()!=0) hist = new TH1D(hdef[i].name, hdef[i].title, hdef[i].binning.size() - 1, &hdef[i].binning[0]);
    else                                hist = new TH1D(hdef[i].name, hdef[i].title, hdef[i].nbins, hdef[i].xmin, hdef[i].xmax);
    hist->Sumw2();
    hst_collection.push_back(hist);
  }
  cout<<"hst_collection size = "<<(int)hst_collection.size()<<"\033[0m"<<endl;

  //________________________________________________________________________________________________
  //
  // Event loop:
  //________________________________________________________________________________________________


  // Opening the ROOT file
  TFile *file = TFile::Open(inputFilename);
  if (!file || file->IsZombie() || !file->IsOpen()) {
    cerr << "Error opening file: " << inputFilename << endl;
    return;
  }

  TTree* tree = (TTree*)file->Get("myEvents");
  setBranches(tree);
  
  Long64_t nentries = tree->GetEntries();
  for (Long64_t i = 0; i < nentries; i++) {

    tree->GetEntry(i);

    /*
    int verbosity = 100000;
    if(i % verbosity == 0) {
      cout << "Processed " << i << " events ";
      cout << "("<<fixed<<setprecision(2)<<(float(i)/nentries)*100<<"%)";
      cout << defaultfloat <<endl;
    }
    if(i==nentries-1) cout << "Processed " << nentries << " events (100%)"<<endl;*/

    // Based on the event-level variables available,
    // Put event selections here.
    bool event_selection = false;
    bool channel_selection = channel == channelval;
	
    Double_t wt = 1.0;
    
    //if((string)campaign == "2016preVFP_UL" || (string)campaign == "2016postVFP_UL") wt_pileup = 1.0;
    wt = wt*wt_leptonSF*wt_trig*wt_pileup; //Object corrections
    //wt = wt*wt_bjet;             //Adding b-tagging corrections

    //--------------------------------
    // Corrections to the histograms:
    //--------------------------------
    std::string filename(inputFilename);
    std::string baseFilename = filename.substr(filename.find_last_of("/\\") + 1);
    
    // 1) Global QCD scaling:
    bool correct_QCD = (baseFilename.find("QCD") != std::string::npos) && (baseFilename.find("Enriched") != std::string::npos);
    if(correct_QCD){
      //cout<<"Correcting QCD MC"<<endl;
      Double_t scale_qcd = 1.0;
      scale_qcd = (Double_t)getScaleFactorGlobal(campaign, channelval, sf_qcd);
      wt = wt * scale_qcd;
    }
    // 2) TTbar HT binned scaling:
    bool correct_ttbar = baseFilename.find("TTBar") != std::string::npos;
    if(correct_ttbar){
      //cout<<"Correcting TTbar MC"<<endl;
      Double_t scale_ttbar = 1.0;
      //if(correct_ttbar)  scale_ttbar = (Double_t)getScaleFactorInBins(campaign, channelval, HT, sf_ttbar);
      wt = wt * scale_ttbar;
    }
    
    //--------------------------------
    // Filling up the histograms:
    // Caution: Careful with the order of the variables and size of hist_collection!
    //--------------------------------
    
    event_selection = channel_selection;
    if(event_selection){
      Double_t fnwt = wt;
      // integers:
      hst_collection[0] ->Fill(channel, 1.0);
      hst_collection[1] ->Fill(trigger, 1.0);
      hst_collection[2] ->Fill(nlep, fnwt);
      hst_collection[3] ->Fill(njet, fnwt);
      hst_collection[4] ->Fill(nbjet, fnwt);
      // leading lepton:
      hst_collection[5] ->Fill(lep0_pt, fnwt);
      hst_collection[6] ->Fill(lep0_eta, fnwt);
      hst_collection[7] ->Fill(lep0_phi, fnwt);
      hst_collection[8] ->Fill(lep0_iso, fnwt);
      hst_collection[9] ->Fill(lep0_sip3d, fnwt);
      hst_collection[10]->Fill(lep0_mt, fnwt);
      // subleading lepton:
      hst_collection[11]->Fill(lep1_pt, fnwt);
      hst_collection[12]->Fill(lep1_eta, fnwt);
      hst_collection[13]->Fill(lep1_phi, fnwt);
      hst_collection[14]->Fill(lep1_iso, fnwt);
      hst_collection[15]->Fill(lep1_sip3d, fnwt);
      hst_collection[16]->Fill(lep1_mt, fnwt);
      // dilepton system:
      hst_collection[17]->Fill(dilep_pt, fnwt);
      hst_collection[18]->Fill(dilep_eta, fnwt);
      hst_collection[19]->Fill(dilep_phi, fnwt);
      hst_collection[20]->Fill(dilep_mass, fnwt);
      hst_collection[21]->Fill(dilep_mt, fnwt);
      hst_collection[22]->Fill(dilep_deta, fnwt);
      hst_collection[23]->Fill(dilep_dphi, fnwt);
      hst_collection[24]->Fill(dilep_dR, fnwt);
      hst_collection[25]->Fill(dilep_ptratio, fnwt);
      // event level variables:
      hst_collection[26]->Fill(HT, fnwt);
      hst_collection[27]->Fill(LT, fnwt);
      hst_collection[28]->Fill(STvis, fnwt);
      hst_collection[29]->Fill(ST, fnwt);
      hst_collection[30]->Fill(HTMETllpt, fnwt);
      hst_collection[31]->Fill(STfrac, fnwt);
      hst_collection[32]->Fill(metpt, fnwt);
      hst_collection[33]->Fill(metphi, fnwt);
      // dphis:
      hst_collection[34]->Fill(dphi_metlep0, fnwt);
      hst_collection[35]->Fill(dphi_metlep1, fnwt);
      hst_collection[36]->Fill(dphi_metdilep, fnwt);
      hst_collection[37]->Fill(dphi_metlep_max, fnwt);
      hst_collection[38]->Fill(dphi_metlep_min, fnwt);
      // weights:
      hst_collection[39]->Fill(wt_leptonSF, 1.0);
      hst_collection[40]->Fill(wt_trig, 1.0);
      hst_collection[41]->Fill(wt_pileup, 1.0);
      hst_collection[42]->Fill(wt_bjet, 1.0);
      hst_collection[43]->Fill(wt, 1.0);
    }//Event selection
    //if(i>=100000) break;
  }//Event loop
    
  // Event loop ends
  //________________________________________________________________________________________________

  /*
  //Debug:
  cout << fixed << setprecision(3);
  cout << setw(20) << left << "Histogram Name"
       << setw(15) << right << "Entries"
       << setw(20) << "Overflow"
       << setw(20) << "Integral (Before Overflow)"
       << setw(20) << "Integral (After Overflow)"
       << setw(20) << "Integral (After Lumi)" << endl;
  cout << string(115, '-') << endl;

  for(int i = 0; i < (int)hst_collection.size(); i++) {

    if(i == 2 || i == 20 || i == 26) {
      double entries = hst_collection[i]->GetEntries();
      double overflow = hst_collection[i]->GetBinContent(hst_collection[i]->GetNbinsX() + 1);
      double integral_before = hst_collection[i]->Integral();

      // Handle overflow
      SetLastBinAsOverflow(hst_collection[i]);

      double integral_after = hst_collection[i]->Integral();

      // Apply lumi scaling
      hst_collection[i]->Scale(lumisf);
      double integral_after_lumi = hst_collection[i]->Integral();

      cout << setw(20) << left << hst_collection[i]->GetTitle()
	   << setw(15) << right << entries
	   << setw(20) << overflow
	   << setw(20) << integral_before
	   << setw(20) << integral_after
	   << setw(20) << integral_after_lumi << endl;
    }
  }
  */
  
  //Luminosity scaling and overflow handling:
  for(int i=0; i<(int)hst_collection.size(); i++){
    SetLastBinAsOverflow(hst_collection[i]);
    hst_collection[i]->Scale(lumisf);
  }
  
  // Save histograms to a new ROOT file
  TFile *outputFile = new TFile(outputFilename, "RECREATE");
  for(int i=0; i<(int)hst_collection.size(); i++) hst_collection[i]->Write();

  // Close files
  for(int i=0; i<(int)hst_collection.size(); i++) delete hst_collection[i];
  outputFile->Close();
  file->Close();

  cout << "Histograms have been saved to " << outputFilename << endl;
}

#endif //EVENTPROCESSOR_H
