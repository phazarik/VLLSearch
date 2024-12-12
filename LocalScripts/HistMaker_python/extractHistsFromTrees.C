#include <TFile.h>
#include <TTree.h>
#include <TH1F.h>
#include <iostream>
#include <vector>
#include "setBranchesAndHistograms.h"
using namespace std;

Long64_t channel, trigger, nlep, njet, nbjet;
Double_t lep0_pt, lep0_eta, lep0_phi, lep0_iso, lep0_sip3d, lep0_mt;
Double_t lep1_pt, lep1_eta, lep1_phi, lep1_iso, lep1_sip3d, lep1_mt;
Double_t dilep_pt, dilep_eta, dilep_phi, dilep_mass, dilep_mt, dilep_deta, dilep_dphi, dilep_dR, dilep_ptratio;
Double_t HT, LT, STvis, ST, HTMETllpt, STfrac, metpt, metphi;
Double_t dphi_metlep0, dphi_metlep1, dphi_metdilep, dphi_metlep_max, dphi_metlep_min;
Double_t nnscore1, nnscore2, nnscore3, nnscore4;
Double_t wt_leptonSF, wt_trig, wt_pileup, wt_bjet, weight;

struct hists{
  TString name;
  TString title;
  Int_t nbins;
  Double_t xmin;
  Double_t xmax;
  std::vector<float> binning;  // For custom binning
};

//External functions:
void setBranches(TTree *tree);
void SetLastBinAsOverflow(TH1F *hst);
void processTree(
		 const char* inputFilename,
		 const char* outputFilename,
		 const char* campaign,
		 int channelval,
		 float lumisf);

//________________________________________________________________________________________________________________
//
// Process all files for a  given jobname:
//________________________________________________________________________________________________________________

#include <fstream>
#include <string>
#include <map>
#include <filesystem>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <nlohmann/json.hpp>
namespace fs = std::filesystem;
using json = nlohmann::json;

// Function to load JSON
map<string, map<string, float>> loadJson(const string &filename) {
    ifstream file(filename);
    json j;
    file >> j;
    return j.get<map<string, map<string, float>>>();
}

void extractHistsFromTrees(
			   const std::string& jobname  = "tree_2018UL_baseline_Dec12",
			   const std::string& dump     = "test_onlyC_mm",
			   const std::string& campaign = "2018_UL",
			   const std::string& channel  = "mm",
			   bool test   = false,
			   bool dryrun = false)
{
  auto start_time = chrono::high_resolution_clock::now();
  
  //Load luminosity file:
  string lumifile;
  float datalumi = 0.0;
  if (campaign.find("2016preVFP") != string::npos){
    lumifile = "../../Metadata/jsondata/lumidata_2016preVFP_UL.json";
    datalumi = 19692;
  }
  else if (campaign.find("2016postVFP") != string::npos) {
    lumifile = "../../Metadata/jsondata/lumidata_2016postVFP_UL.json";
    datalumi = 16227;
  }
  else if (campaign.find("2017") != string::npos) {
    lumifile = "../../Metadata/jsondata/lumidata_2017_UL.json";
    datalumi = 41480;
  }
  else if (campaign.find("2018") != string::npos) {
    lumifile = "../../Metadata/jsondata/lumidata_2018_UL.json";
    datalumi = 59830;
  }

  // Load luminosity data
  auto samplelist = loadJson(lumifile);

  // Determine channel value
  int chval = -1;
  if (channel == "mm") chval = 0;
  else if (channel == "me") chval = 1;
  else if (channel == "em") chval = 2;
  else if (channel == "ee") chval = 3;
  else {
    cerr << "Pick one: ee, em, me or mm.\n";
    return;
  }

  // Input and output directories
  string indir = "../input_files/trees_modified/" + jobname;
  string outdir = "../input_files/hists/" + dump;
  fs::create_directories(outdir);

  //---------------------------------------
  // Loop over files in the input directory
  //---------------------------------------
  auto prev_time = start_time;
  vector<string> list_processed, list_failed;

  for (const auto &entry : fs::directory_iterator(indir)) {

    string treefile = entry.path().filename().string();
    if (treefile.find(".root") == string::npos) continue;
    cout << "\n\033[033mProcessing " << treefile << "\033[0m" <<endl;

    string histfile = treefile;
    histfile.replace(histfile.find("tree"), 4, "hst");

    string infilepath = indir + "/" + treefile;
    string outfilepath = outdir + "/" + histfile;

    // Extract luminosity
    float lumi = 0.0;
    for (const auto &[sample, subs] : samplelist) {
      if (treefile.find(sample) == string::npos) continue;

      for (const auto &[subsample, val] : subs) {
	if (treefile.find(subsample) != string::npos) {
	  lumi = val;
	  break;
	}
      }
    }

    if (lumi > 0.0) list_processed.push_back(treefile);
    else list_failed.push_back(treefile);

    float lumisf = datalumi / lumi;
    if (treefile.find("SingleMuon") != string::npos || treefile.find("EGamma") != string::npos) {
      lumisf = 1.0;
    }
    cout << "Luminosity = " << lumi << ", scalefactor = " << fixed << setprecision(6) << lumisf << endl;

    if(dryrun) cout<<"Arguments: "<<infilepath <<"\t"<< outfilepath <<"\t"<< campaign <<"\t"<< chval <<"\t"<< lumisf <<endl;
    else  processTree(infilepath.c_str(), outfilepath.c_str(), campaign.c_str(), chval, lumisf);

    auto current_time = chrono::high_resolution_clock::now();
    cout<<"Done! time taken = \033[033m"<<chrono::duration_cast<chrono::seconds>(current_time - prev_time).count()<<" second(s)\033[0m"<<endl;
    prev_time = current_time;

    if(test) break;
    
  }//File loop ends.
    
  auto end_time = chrono::high_resolution_clock::now();
  auto time_taken = chrono::duration_cast<chrono::seconds>(end_time - start_time).count();

  cout << "\033[093m\nSummary:\033[0m" <<endl;
  cout << "Files processed = "<<endl;
  for (const auto &file : list_processed) cout << file << ", ";
  cout << "\033[093m\nOutput directory: " << outdir << endl;
  cout << "Total time taken: " << time_taken << " seconds.\n\033[0m"<<endl;

  if (!list_failed.empty()) {
    cerr << "\033[031m\nWarning: The following files were skipped :\n\033[0m"<<endl;
    for (const auto &file : list_failed) cerr << file << ", ";
    cerr << endl;
  }
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
    {"STfrac", "STfrac", 100, 0, 1.1, {}},
    {"HTMETllpt", "HTMETllpt", 0, 0, 0, {0, 25, 50, 100, 200, 300, 400, 500}}, //variable binning
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

    //wt = wt*wt_leptonSF*wt_trig; //Object corrections
    //wt = wt*wt_bjet;             //Adding b-tagging corrections

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
