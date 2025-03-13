#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>

void run_inROOT(
	    TString infile = "../input_files/trees_modified/tree_ee_Zwindow_2018UL_Mar11/tree_DYJetsToLL_M50.root",
	    TString outfile = "test_inROOT.root",
	    TString campaign = "2018_UL",
	    TString channel = "ee",
	    //Double_t lumisf = 1.736657,
	    TString lumisf_str = "1.736657",
	    TString sample = "DYJetsToLL_M50",
	    TString flag = "ScaleDY"
	    )
{
  TString arguments   = "\""+infile+"\"" + ",\""+outfile+"\"" + ",\""+campaign+"\""+",\""+channel+"\""+",\""+lumisf_str+"\""+",\""+sample+"\""+",\""+flag+"\"";
  TString processline = ".x run_once.C(" + arguments + ")";
  //cout<<processline<<endl;
  gSystem->Load("myHist_C.so");
  gROOT->ProcessLine(processline);
}
