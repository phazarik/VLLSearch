//----------------------------
// Test-run on a single file
//----------------------------

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
void run_local(TString filepath, int data)
{
  const char *treefilename; 
  TChain *chain = new TChain("Events");
  AnaScript m_selec;//declared an instance of our class.
  
  chain->Add(filepath);
  treefilename = "test_outputs/tree_test.root";
  m_selec.SetData(data); //0=MC, 1=Data
  m_selec.SetCampaign("2018_UL");
  m_selec.SetFlag("");
  m_selec.SetSampleName("test");
  
  std::cout<<"\nOutput : "<<treefilename<<std::endl;
  m_selec.SetTreeFileName(treefilename);
  m_selec.SetVerbose(0);
  
  chain->Process(&m_selec);
    
}
