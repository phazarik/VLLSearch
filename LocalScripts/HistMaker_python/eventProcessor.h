#ifndef EVENTPROCESSOR_H
#define EVENTPROCESSOR_H

#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>
#include <vector>
#include <string>
#include "setBranchesAndHistograms.h"
using namespace std;

extern Long64_t channel, trigger, nlep, njet, nbjet;
extern Double_t lep0_pt, lep0_eta, lep0_phi, lep0_iso, lep0_sip3d, lep0_mt;
extern Double_t lep1_pt, lep1_eta, lep1_phi, lep1_iso, lep1_sip3d, lep1_mt;
extern Double_t dilep_pt, dilep_eta, dilep_phi, dilep_mass, dilep_mt, dilep_deta, dilep_dphi, dilep_dR, dilep_ptratio;
extern Double_t HT, LT, STvis, ST, HTMETllpt, STfrac, metpt, metphi;
extern Double_t dphi_metlep0, dphi_metlep1, dphi_metdilep, dphi_metlep_max, dphi_metlep_min;
extern Double_t wt_leptonSF, wt_trig, wt_pileup, wt_bjet, weight;
extern Double_t nnscore1, nnscore2, nnscore3, nnscore4;

struct hists{
  TString name;
  TString title;
  Int_t nbins;
  Double_t xmin;
  Double_t xmax;
  std::vector<float> binning;  // For custom binning
};

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
  // Opening the ROOT file
  TFile *file = TFile::Open(inputFilename);
  if (!file || file->IsZombie()) {
    cerr << "Error opening file: " << inputFilename << endl;
    return;
  }

  // Getting the TTree
  TTree *tree = (TTree*)file->Get("myEvents");
  if (!tree) {
    cerr << "Error getting TTree from file: " << inputFilename << endl;
    file->Close();
    return;
  }

  setBranches(tree);
  vector<TH1F*> hst_collection;

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
    // nnscores:
    {"nnscore_qcd_vlldele_100",     "nnscore_qcd_vlldele_100",     200, 0, 1, {}},
    {"nnscore_qcd_vlldele_200_800", "nnscore_qcd_vlldele_200_800", 200, 0, 1, {}},
    {"nnscore_qcd_vlldmu_100",      "nnscore_ttbar_vlldmu_100",    200, 0, 1, {}},
    {"nnscore_qcd_vlldmu_200_800",  "nnscore_qcd_vlldmu_200_800",  200, 0, 1, {}},
    // weights:
    {"2LSS_wt_leptonSF", "wt_leptonSF", 200, 0, 2, {}},
    {"2LSS_wt_trig", "wt_trig", 200, 0, 2, {}},
    {"2LSS_wt_pileup", "wt_pileup", 200, 0, 2, {}},
    {"2LSS_wt_bjet", "wt_bjet", 200, 0, 2, {}},
    {"2LSS_wt_evt", "weight", 200, 0, 2, {}},
  };
    
  //Booking histograms:
  for (int i=0; i<(int)hdef.size(); i++) {
    TH1F* hist;
    //cout<<hdef[i].name <<"\t"<< hdef[i].title <<"\t"<< hdef[i].nbins <<"\t"<< hdef[i].xmin <<"\t"<< hdef[i].xmax<<endl;
    if ((int)hdef[i].binning.size()!=0) hist = new TH1F(hdef[i].name, hdef[i].title, hdef[i].binning.size() - 1, &hdef[i].binning[0]);
    else                                hist = new TH1F(hdef[i].name, hdef[i].title, hdef[i].nbins, hdef[i].xmin, hdef[i].xmax);
    hist->Sumw2();
    hst_collection.push_back(hist);
  }
  cout<<"hst_collection size = "<<(int)hst_collection.size()<<"\033[0m"<<endl;

  //Flagging specific samples:
  string filename(inputFilename);
  string baseFilename = filename.substr(filename.find_last_of("/\\") + 1);
  bool correctDY    = baseFilename.find("DYJetsToLL") != string::npos;
  bool correctTTBar = baseFilename.find("TTBar") != string::npos;
  if(correctDY)    cout<<"Correcting DY events in HT bins (ee)"<<endl;
  if(correctTTBar) cout<<"Correcting TTBar events in LT bins (all channels)"<<endl;

  //________________________________________________________________________________________________
  //
  // Event loop:
  //________________________________________________________________________________________________

  Long64_t nentries = tree->GetEntries();
  for (Long64_t i = 0; i < nentries; i++) {

    tree->GetEntry(i);

    // Based on the event-level variables available,
    // Put event selections here.
    bool event_selection = false;
    bool channel_selection = channel == channelval;

    Double_t wt = 1.0;

    //--------------------------------
    // Corrections to the histograms:
    //--------------------------------

    wt = wt*wt_leptonSF*wt_trig*wt_pileup; //Object corrections
    wt = wt*wt_bjet;             //Adding b-tagging corrections

    event_selection = channel_selection;
    
    //--------------------------------
    // Filling up the histograms:
    //--------------------------------

    // Caution: Careful with the order of the variables and size of hist_collection!
    if(event_selection){
      // integers:
      hst_collection[0] ->Fill(channel, 1.0);
      hst_collection[1] ->Fill(trigger, 1.0);
      hst_collection[2] ->Fill(nlep, wt);
      hst_collection[3] ->Fill(njet, wt);
      hst_collection[4] ->Fill(nbjet, wt);
      // leading lepton:
      hst_collection[5] ->Fill(lep0_pt, wt);
      hst_collection[6] ->Fill(lep0_eta, wt);
      hst_collection[7] ->Fill(lep0_phi, wt);
      hst_collection[8] ->Fill(lep0_iso, wt);
      hst_collection[9] ->Fill(lep0_sip3d, wt);
      hst_collection[10]->Fill(lep0_mt, wt);
      // subleading lepton:
      hst_collection[11]->Fill(lep1_pt, wt);
      hst_collection[12]->Fill(lep1_eta, wt);
      hst_collection[13]->Fill(lep1_phi, wt);
      hst_collection[14]->Fill(lep1_iso, wt);
      hst_collection[15]->Fill(lep1_sip3d, wt);
      hst_collection[16]->Fill(lep1_mt, wt);
      // dilepton system:
      hst_collection[17]->Fill(dilep_pt, wt);
      hst_collection[18]->Fill(dilep_eta, wt);
      hst_collection[19]->Fill(dilep_phi, wt);
      hst_collection[20]->Fill(dilep_mass, wt);
      hst_collection[21]->Fill(dilep_mt, wt);
      hst_collection[22]->Fill(dilep_deta, wt);
      hst_collection[23]->Fill(dilep_dphi, wt);
      hst_collection[24]->Fill(dilep_dR, wt);
      hst_collection[25]->Fill(dilep_ptratio, wt);
      // event level variables:
      hst_collection[26]->Fill(HT, wt);
      hst_collection[27]->Fill(LT, wt);
      hst_collection[28]->Fill(STvis, wt);
      hst_collection[29]->Fill(ST, wt);
      hst_collection[30]->Fill(HTMETllpt, wt);
      hst_collection[31]->Fill(STfrac, wt);
      hst_collection[32]->Fill(metpt, wt);
      hst_collection[33]->Fill(metphi, wt);
      // dphis:
      hst_collection[34]->Fill(dphi_metlep0, wt);
      hst_collection[35]->Fill(dphi_metlep1, wt);
      hst_collection[36]->Fill(dphi_metdilep, wt);
      hst_collection[37]->Fill(dphi_metlep_max, wt);
      hst_collection[38]->Fill(dphi_metlep_min, wt);
      // nnscores:
      hst_collection[39]->Fill(nnscore1, wt);
      hst_collection[40]->Fill(nnscore2, wt);
      hst_collection[41]->Fill(nnscore3, wt);
      hst_collection[42]->Fill(nnscore4, wt);
      // weights:
      hst_collection[43]->Fill(wt_leptonSF, 1.0);
      hst_collection[44]->Fill(wt_trig, 1.0);
      hst_collection[45]->Fill(wt_pileup, 1.0);
      hst_collection[46]->Fill(wt_bjet, 1.0);
      hst_collection[47]->Fill(wt, 1.0);
    }
  }
  // Event loop ends
  //________________________________________________________________________________________________
  
  //Luminosity scaling.
  for(int i=0; i<(int)hst_collection.size(); i++){
    SetLastBinAsOverflow(hst_collection[i]);
    hst_collection[i]->Scale(lumisf);
  }
  
  // Save histograms to a new ROOT file
  TFile *outputFile = new TFile(outputFilename, "RECREATE");
  for(int i=0; i<(int)hst_collection.size(); i++) hst_collection[i]->Write();

  // Close files
  outputFile->Close();
  file->Close();

  cout << "Histograms have been saved to " << outputFilename << endl;
}

#endif //EVENTPROCESSOR_H