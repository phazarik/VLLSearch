//##############################################################################################
// This picks the appropriate libraries automatically and runs the anaCond.C script using gROOT.
// The inputs to runana can be given from the condor submission scripts.
// While anaCond.C is used for only one root file (locally), runana.C is used to run on loops,
// without having to generate the libabary each time by doing .L *so.
// While anaCond.C sits at the directory where the library is, runana.C can sit anywhere.
//##############################################################################################

//.x runana.C("/home/work/phazarik1/work/minimal-analysis-setup/AnaScript/HistMaker", "/home/work/alaha1/public/RunII_ULSamples/2018/DYJetsToLL/M50/VLL_DYJetsToLL_M50_98.root", "/home/work/phazarik1/work/VLLSearch/AnaScript/HistMaker/test_outputs/test_runana.root", "0", "2018_UL", "dy", "DYJetsToLL_M50")

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>

void runana(TString codedir, TString ifname, TString ofname, TString data, TString campaign, TString flag, TString samplename){

  //#######################
  // DO NOT TOUCH ANYTHING
  //#######################
  
  TString path_to_anaCond = codedir;
  TString path_to_library =path_to_anaCond;
  TString anastring = ".x "+path_to_anaCond+"/run_condor.C(\""+ifname+"\",\""+ofname+"\",\""+data+"\",\""+campaign+"\",\""+flag+"\",\""+samplename+"\")";

  gSystem->Load(path_to_library+"/AnaScript_C.so");
  gROOT->ProcessLine(anastring);

  std::cout<<"Input file  = "<<ifname<<endl;
  std::cout<<"Output file = "<<ofname<<"\n"<<endl;

}
