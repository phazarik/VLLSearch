//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar 12 11:56:01 2025 by ROOT version 6.26/10
// from TTree myEvents/
// found on file: tree_DYJetsToLL_M50.root
//////////////////////////////////////////////////////////

#ifndef myHist_h
#define myHist_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTreeReader.h>
#include <TTreeReaderValue.h>
#include <TTreeReaderArray.h>

// Headers needed by this particular selector
#include <vector>
#include <TH1.h>
#include <TH2.h>
#include <TMath.h>
#include "TLorentzVector.h"
#include <fstream>
#include <iostream>
#include "TString.h"
#include <bitset>
#include <time.h>

class myHist : public TSelector {
public :
  TTreeReader     fReader;  //!the tree reader
  TTree          *fChain = 0;   //!pointer to the analyzed TTree or TChain

  // Readers to access the data (delete the ones you do not need).
  TTreeReaderValue<Int_t> channel = {fReader, "channel"};
  TTreeReaderValue<Int_t> trigger = {fReader, "trigger"};
  TTreeReaderValue<Int_t> nlep = {fReader, "nlep"};
  TTreeReaderValue<Int_t> njet = {fReader, "njet"};
  TTreeReaderValue<Int_t> nbjet = {fReader, "nbjet"};
  TTreeReaderValue<Float_t> lep0_pt = {fReader, "lep0_pt"};
  TTreeReaderValue<Float_t> lep0_eta = {fReader, "lep0_eta"};
  TTreeReaderValue<Float_t> lep0_phi = {fReader, "lep0_phi"};
  TTreeReaderValue<Float_t> lep0_iso = {fReader, "lep0_iso"};
  TTreeReaderValue<Float_t> lep0_sip3d = {fReader, "lep0_sip3d"};
  TTreeReaderValue<Float_t> lep0_mt = {fReader, "lep0_mt"};
  TTreeReaderValue<Int_t> lep0_charge = {fReader, "lep0_charge"};
  TTreeReaderValue<Float_t> lep1_pt = {fReader, "lep1_pt"};
  TTreeReaderValue<Float_t> lep1_eta = {fReader, "lep1_eta"};
  TTreeReaderValue<Float_t> lep1_phi = {fReader, "lep1_phi"};
  TTreeReaderValue<Float_t> lep1_iso = {fReader, "lep1_iso"};
  TTreeReaderValue<Float_t> lep1_sip3d = {fReader, "lep1_sip3d"};
  TTreeReaderValue<Float_t> lep1_mt = {fReader, "lep1_mt"};
  TTreeReaderValue<Int_t> lep1_charge = {fReader, "lep1_charge"};
  TTreeReaderValue<Float_t> dilep_pt = {fReader, "dilep_pt"};
  TTreeReaderValue<Float_t> dilep_eta = {fReader, "dilep_eta"};
  TTreeReaderValue<Float_t> dilep_phi = {fReader, "dilep_phi"};
  TTreeReaderValue<Float_t> dilep_mass = {fReader, "dilep_mass"};
  TTreeReaderValue<Float_t> dilep_mt = {fReader, "dilep_mt"};
  TTreeReaderValue<Float_t> dilep_deta = {fReader, "dilep_deta"};
  TTreeReaderValue<Float_t> dilep_dphi = {fReader, "dilep_dphi"};
  TTreeReaderValue<Float_t> dilep_dR = {fReader, "dilep_dR"};
  TTreeReaderValue<Float_t> dilep_ptratio = {fReader, "dilep_ptratio"};
  TTreeReaderValue<Float_t> HT = {fReader, "HT"};
  TTreeReaderValue<Float_t> LT = {fReader, "LT"};
  TTreeReaderValue<Float_t> STvis = {fReader, "STvis"};
  TTreeReaderValue<Float_t> ST = {fReader, "ST"};
  TTreeReaderValue<Float_t> HTMETllpt = {fReader, "HTMETllpt"};
  TTreeReaderValue<Float_t> STfrac = {fReader, "STfrac"};
  TTreeReaderValue<Float_t> metpt = {fReader, "metpt"};
  TTreeReaderValue<Float_t> metphi = {fReader, "metphi"};
  TTreeReaderValue<Float_t> dphi_metlep0 = {fReader, "dphi_metlep0"};
  TTreeReaderValue<Float_t> dphi_metlep1 = {fReader, "dphi_metlep1"};
  TTreeReaderValue<Float_t> dphi_metdilep = {fReader, "dphi_metdilep"};
  TTreeReaderValue<Float_t> dphi_metlep_max = {fReader, "dphi_metlep_max"};
  TTreeReaderValue<Float_t> dphi_metlep_min = {fReader, "dphi_metlep_min"};
  TTreeReaderValue<Float_t> jec = {fReader, "jec"};
  TTreeReaderValue<Float_t> jer = {fReader, "jer"};
  TTreeReaderValue<Double_t> wt_leptonSF = {fReader, "wt_leptonSF"};
  TTreeReaderValue<Double_t> wt_trig = {fReader, "wt_trig"};
  TTreeReaderValue<Double_t> wt_pileup = {fReader, "wt_pileup"};
  TTreeReaderValue<Double_t> wt_bjet = {fReader, "wt_bjet"};
  TTreeReaderValue<Double_t> weight = {fReader, "weight"};
  TTreeReaderValue<Float_t> nnscore_qcd_vlld_2016postVFP = {fReader, "nnscore_qcd_vlld_2016postVFP"};
  TTreeReaderValue<Float_t> nnscore_qcd_vlld_2016preVFP = {fReader, "nnscore_qcd_vlld_2016preVFP"};
  TTreeReaderValue<Float_t> nnscore_qcd_vlld_2017 = {fReader, "nnscore_qcd_vlld_2017"};
  TTreeReaderValue<Float_t> nnscore_qcd_vlld_2018 = {fReader, "nnscore_qcd_vlld_2018"};
  TTreeReaderValue<Float_t> nnscore_ttbar_vlld = {fReader, "nnscore_ttbar_vlld"};

  myHist(TTree * /*tree*/ =0) { }
  virtual ~myHist() { }
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();

  //User defined functions:
  void SetHstFileName(const char *HstFileName){ _HstFileName = HstFileName;}
  void SetVerbose(int verbose){ _verbosity = verbose; }
  //void SetData(int data){_data=data;}
  void SetCampaign(TString campaign){_campaign = campaign;}
  void SetChannel(TString ch){_channel=ch;}
  void SetLumi(double lumi){_lumi=lumi;}
  void SetSampleName(TString samplename){_samplename=samplename;}
  void SetFlag(TString flag){_flag=flag;}
  void BookHistograms();
  void SetLastBinAsOverflow(TH1D *hst);
  Double_t getYield(TH1D *hst);

  //Corrections:
  Double_t getSF_QCD_global();
  Double_t getSF_TTBar_HTbinned(float ht);

public:
  struct Hists {
    vector<TH1D *>hist;
  };

protected:
  Hists h;

private:
  TFile *_HstFile;
  const char *_HstFileName;

  //Global variables:
  int _verbosity;
  TString _flag, _samplename, _campaign, _channel;
  double _lumi;
  int nEvtTotal; Double_t nEvtPassScaled;
  time_t start, end, buffer;
  
  ClassDef(myHist,0);

};

#endif

#ifdef myHist_cxx
void myHist::Init(TTree *tree)
{
   fReader.SetTree(tree);
}

Bool_t myHist::Notify()
{
   return kTRUE;
}

#endif // #ifdef myHist_cxx
