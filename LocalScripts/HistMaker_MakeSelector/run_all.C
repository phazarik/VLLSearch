#include <fstream>
#include <filesystem>
#include <string>
#include <map>
#include <chrono>
#include <cstdlib>
#include <iomanip>
#include <TSystem.h>
#include <TString.h>
#include <nlohmann/json.hpp>
namespace fs = std::filesystem;
using json = nlohmann::json;

// Function to load JSON:
map<string, map<string, float>> loadJson(const string &filename) {
    ifstream file(filename); json j;
    file >> j;
    return j.get<map<string, map<string, float>>>();
}

void run_all(
	     TString jobname  = "tree_eeSS_Zwindow_2018UL_Mar13",
	     TString dump     = "hist_eeSS_Zwindow_2018UL_Mar13_ee",
	     TString campaign = "2018_UL",
	     TString channel  = "ee",
	     bool test   = false,
	     bool dryrun = false
  )
{
  // Input and output directories
  string indir = "../input_files/trees_modified/" + string(jobname);
  string outdir = "../input_files/hists/" + string(dump);
  fs::create_directories(outdir);

  auto start = std::chrono::high_resolution_clock::now();
  //------------------------------------------------------------------
  //Load luminosity data from JSON:
  string lumifile;
  float datalumi = 0.0;
  if (string(campaign).find("2016preVFP") != string::npos){
    lumifile = "../../Metadata/jsondata/lumidata_2016preVFP_UL.json";
    datalumi = 19692;
  }
  else if (string(campaign).find("2016postVFP") != string::npos) {
    lumifile = "../../Metadata/jsondata/lumidata_2016postVFP_UL.json";
    datalumi = 16227;
  }
  else if (string(campaign).find("2017") != string::npos) {
    lumifile = "../../Metadata/jsondata/lumidata_2017_UL.json";
    datalumi = 41480;
  }
  else if (string(campaign).find("2018") != string::npos) {
    lumifile = "../../Metadata/jsondata/lumidata_2018_UL.json";
    datalumi = 59830;
  }
  auto samplelist = loadJson(lumifile);
  //cout<<"JSON data loaded."<<endl;
  //------------------------------------------------------------------

   //---------------------------------------
  // Loop over files in the input directory
  //---------------------------------------
  vector<string> list_processed, list_failed;

  int count = 0;
  int totalfiles = std::distance(fs::directory_iterator(indir), fs::directory_iterator());
  
  for (const auto &entry : fs::directory_iterator(indir)) {

    //INFILE
    string treefile = entry.path().filename().string();
    if (treefile.find(".root") == string::npos) continue;
    string infilepath = indir + "/" + treefile;
    
    //OUTFILE
    string histfile = treefile;
    histfile.replace(histfile.find("tree"), 4, "hst");
    string outfilepath = outdir + "/" + histfile;

    //LUMISF and SAMPLENAME
    float lumi = 0.0; TString samplename = "";
    for (const auto &[sample, subs] : samplelist) {
      if (treefile.find(sample) == string::npos) continue;
      for (const auto &[subsample, val] : subs) {
	if (treefile.find(subsample) != string::npos) {
	  lumi = val;
	  samplename = (TString)sample+"_"+(TString)subsample;
	  break;
	}
      }
    }
    Double_t lumisf = datalumi/(Double_t)lumi;
    if(
       treefile.find("SingleMuon")     != string::npos ||
       treefile.find("EGamma")         != string::npos ||
       treefile.find("SingleElectron") != string::npos
       ) lumisf = 1.0;
    
    //FLAGS
    TString flag = "";
    if(treefile.find("DYJetsToLL") != string::npos) flag = "ScaleDY";
    if(treefile.find("QCD")        != string::npos) flag = "ScaleQCD";
    if(treefile.find("TTBar")      != string::npos) flag = "ScaleTTBar";

    //-------------------------------------------------------------------
    //MAIN:
    count += 1;
    cout<<"\n\033[1;33m("<< count <<"/"<< totalfiles <<") "<<samplename<<"\033[0m"<<endl;
    TString arguments = "\""+(TString)infilepath+"\"" + ",\""+(TString)outfilepath+"\"" + ",\""+campaign+"\""+",\""+channel+"\""+",\""+lumisf+"\",\""+samplename+"\""+",\""+flag+"\"";
    TString processline = "root -q -b -l 'run_inROOT.C("+arguments+")'";
    if(dryrun)cout<<processline<<endl;
    else gSystem->Exec(processline);  
    //-------------------------------------------------------------------
    if(test) break; //File
  }

  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> duration = end - start;
  std::cout << "\n\033[1;33mTotal time taken: " << duration.count() << " seconds.\033[0m" << std::endl;
}
