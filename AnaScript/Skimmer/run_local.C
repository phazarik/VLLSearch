//----------------------------
// Test-run on a single file
//----------------------------

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
void run_local(TString filepath, int data)
{
  const char *skimfilename; 
  TChain *chain = new TChain("Events");
  AnaScript m_selec;//declared an instance of our class.
  
  chain->Add(filepath);
  skimfilename = "test_outputs/skim_test.root";
  m_selec.SetData(data); //0=MC, 1=Data
  m_selec.SetCampaign("2018_UL");
  m_selec.SetFlag("");
  m_selec.SetSampleName("test");
  
  std::cout<<"\nOutput : "<<skimfilename<<std::endl;
  m_selec.SetSkimFileName(skimfilename);
  m_selec.SetVerbose(0);
  
  chain->Process(&m_selec);
    
}
