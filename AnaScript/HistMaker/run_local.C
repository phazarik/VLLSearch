//----------------------------
// Test-run on a single file
//----------------------------

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
void run_local(TString filepath, int data)
{
  const char *hstfilename; 
  TChain *chain = new TChain("Events");
  AnaScript m_selec;//declared an instance of our class.
  
  chain->Add(filepath);
  hstfilename = "test_outputs/hst_test.root";
  m_selec.SetData(data); //0=MC, 1=Data
  m_selec.SetCampaign("2018_UL");
  m_selec.SetFlag("");
  m_selec.SetSampleName("test");
  
  std::cout<<"\nOutput : "<<hstfilename<<std::endl;
  m_selec.SetHstFileName(hstfilename);
  m_selec.SetVerbose(0);
  
  chain->Process(&m_selec);
    
}
