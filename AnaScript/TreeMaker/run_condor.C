//#########################################################################################################
// This script is sent to the condor job.
// It still requires the library to be loaded by doing .L *.so beforehand.
// It takes input and output filenames and some other parameters as arguments.
// This driver script is run using runana.C, which uses gROOT to run this from outside the root terminal.
//#########################################################################################################

//Execution line :
// .x run_condor.C("/home/work/alaha1/public/RunII_ULSamples/2018/DYJetsToLL/M50/VLL_DYJetsToLL_M50_98.root", "test_outputs/test_anaCond.root", "0", "2018_UL", "dy", "DYJetsToLL_M50")

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <string>

void run_condor( TString ifname , TString ofname, TString data, TString campaign, TString flag, TString samplename)
{
  //Description of the parameters:
  //1. ifname : Input file name with full path.
  //2. ofname : Output file name with full path.
  //3. data   : 0, or 1 depedning on whether 'ifname' is MC or data.
  //4. year   : Which year of data taking. (affects trigger paths and efficiencies)
  //5. lep    : If muon dataset, lep=1, if electron dataset, lep=0
  
  gROOT->Time();
  const char *treefilename;
  TChain *chain = new TChain("Events");
  AnaScript m_selec;

  //Setting parameters from the input arguments:
  TString input = ifname;
  bool manual = input.EndsWith(".root");
  bool single=false;
  if(!manual) input += "/*.root"; //This makes sure that the input filenames always end with .root
  chain->Add(input);
  
  treefilename = ofname;  

  //SetTreeFileName:
  m_selec.SetTreeFileName(treefilename);

  //SetVerbose:
  m_selec.SetVerbose(1);

  //SetData:
  if(data=="0") m_selec.SetData(0);
  if(data=="1") m_selec.SetData(1);
  
  //SetCampaign:
  m_selec.SetCampaign(campaign);
  
  //Set the additional flags:
  m_selec.SetFlag(flag);
  m_selec.SetSampleName(samplename);

  //Run:
  chain->Process(&m_selec);
  gROOT->Time();
}
