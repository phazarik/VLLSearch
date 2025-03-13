#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <string>
using namespace std;

void run_once(
		TString infile = "../input_files/trees_modified/tree_ee_Zwindow_2018UL_Mar11/tree_DYJetsToLL_M50.root",
		TString outfile = "test.root",
		TString campaign = "2018_UL",
		TString channel = "ee",
		TString lumisf_str = "1.736657",
		TString sample = "DYJetsToLL_M50",
		TString flag = "dyscale"
		)
{
  const char *hstfilename;
  TChain *chain = new TChain("myEvents");
  myHist m_selec;
  Double_t lumisf = lumisf_str.Atof();

  //Parameter set:
  cout << "\033[33mInput  : " << infile <<"\033[0m"<< endl;
  cout << "\033[33mOutput : " << outfile <<"\033[0m"<< endl;
  chain->Add(infile);
  hstfilename = outfile;
  m_selec.SetCampaign(campaign);
  m_selec.SetChannel(channel);
  m_selec.SetLumi(lumisf);
  m_selec.SetSampleName(sample);
  m_selec.SetFlag(flag);
  m_selec.SetHstFileName(hstfilename);
  m_selec.SetVerbose(0);

  //Main:
  chain->Process(&m_selec);
  cout<<"Done."<<endl;
}

