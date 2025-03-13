#define myHist_cxx
#include "myHist.h"
#include <TH2.h>
#include <TStyle.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <string>
#include <set>
#include "utilities/BookHistograms.h"
#include "utilities/MCcorrections.h"
using namespace std;


void myHist::Begin(TTree * /*tree*/)
{
   TString option = GetOption();
}
void myHist::SlaveBegin(TTree * /*tree*/)
{
   TString option = GetOption();
   time(&start);

   cout << "Global parameters loaded." <<endl;
   cout <<_campaign<<" "<<_channel<<" "<<_lumi<<" "<<_samplename<<" "<<_flag<<endl;
   if(_flag == "ScaleDY")    cout <<"\033[33mScaling DY MC globally.\033[0m"<<endl;
   if(_flag == "ScaleQCD")   cout <<"\033[33mScaling QCD MC globally.\033[0m"<<endl;
   if(_flag == "ScaleTTBar") cout <<"\033[33mScaling TTBar MC in HT bins.\033[0m"<<endl;
   
   nEvtTotal = 0;
   nEvtPassScaled = 0;
   _HstFile = new TFile(_HstFileName,"recreate");
   BookHistograms();
   cout<<"n-events time(sec)"<<endl;
   
}
void myHist::SlaveTerminate()
{
  //Overflow bins and lumi scaling:
  for(int i=0; i<(int)h.hist.size(); i++){
    SetLastBinAsOverflow(h.hist[i]);
    h.hist[i]->Scale(_lumi);
  }
  
  /*
  //---------------------------------------------------
  //Debugging:
  cout << fixed << setprecision(3);
  cout << setw(20) << left << "Histogram Name"
       << setw(15) << right << "Entries"
       << setw(20) << "Overflow"
       << setw(20) << "Integral (Before Overflow)"
       << setw(20) << "Integral (After Overflow)"
       << setw(20) << "Integral (After Lumi)" << endl;
  cout << string(115, '-') << endl;

  for(int i=0; i<(int)h.hist.size(); i++) {

    if (std::set<int>{0, 1, 2, 3, 4, 5, 20, 21, 26}.count(i))  {
      //if (std::set<int>{39, 40, 41, 42, 43}.count(i))  {
      double entries = h.hist[i]->GetEntries();
      double overflow = h.hist[i]->GetBinContent(h.hist[i]->GetNbinsX() + 1);
      double integral_before = h.hist[i]->Integral();

      // Handle overflow
      SetLastBinAsOverflow(h.hist[i]);
      double integral_after = h.hist[i]->Integral();

      // Apply lumi scaling
      h.hist[i]->Scale(_lumi);
      double integral_after_lumi = h.hist[i]->Integral();

      cout << setw(20) << left << h.hist[i]->GetTitle()
           << setw(15) << right << entries
           << setw(20) << overflow
           << setw(20) << integral_before
           << setw(20) << integral_after
           << setw(20) << integral_after_lumi << endl;
    }
    }

  cout<<"\nAdditional check 1"<<endl;
  for(int bin=0; bin<h.hist[41]->GetNbinsX()+1; bin++){
    cout<<"bin "<<bin<<" = "<<h.hist[41]->GetBinContent(bin)<<endl;
  }
  cout<<"Integral = "<<h.hist[41]->Integral()<<endl;
  cout<<"Yield    = "<<getYield(h.hist[41])<<endl;

  cout<<"\nAdditional check 2"<<endl;
  for(int bin=0; bin<h.hist[42]->GetNbinsX()+1; bin++){
    cout<<"bin "<<bin<<" = "<<h.hist[42]->GetBinContent(bin)<<endl;
  }
  cout<<"Integral = "<<h.hist[42]->Integral()<<endl;
  cout<<"Yield    = "<<getYield(h.hist[42])<<endl;*/
  //---------------------------------------------------
  
  _HstFile->Write();
  _HstFile->Close();

  cout<<"-------------------------------"<<endl;
  cout << setw(12) << left << "nEvtTotal:" << nEvtTotal << endl;
  cout << fixed << setprecision(2);
  cout << setw(12) << left << "Yield:"  << (nEvtPassScaled * _lumi) << endl;
  cout<<"-------------------------------"<<endl;
  
  time(&end);
  double time_taken = double(end - start);
  cout << setw(10) << left << "Time taken: " << (int)time_taken << " sec." << endl;
}

void myHist::Terminate()
{
  
}

Bool_t myHist::Process(Long64_t entry)
{
  nEvtTotal++;
  fReader.SetLocalEntry(entry);

  //Verbosity:
  time(&buffer);
  double time_buff = double(buffer-start);
  if (nEvtTotal % 1000000 == 0) cout << setw(10) << left << nEvtTotal << " " << time_buff << endl;
  
  //Final event weight:
  Double_t fnwt = 1.0;
  fnwt = (*wt_leptonSF) * (*wt_trig) * (*wt_pileup);

  //Put additional corrections here:
  if(_flag == "ScaleQCD")   fnwt = fnwt * getSF_QCD_global();
  if(_flag == "ScaleTTBar") fnwt = fnwt * getSF_TTBar_HTbinned((float)*HT);
  
  //Fill Histograms:
  nEvtPassScaled =  nEvtPassScaled + 1*fnwt;
  
  h.hist[0]->Fill((Double_t)*nlep, fnwt);
  h.hist[1]->Fill((Double_t)*njet, fnwt);
  h.hist[2]->Fill((Double_t)*nbjet, fnwt);

  h.hist[3]->Fill((Double_t)*lep0_pt, fnwt);
  h.hist[4]->Fill((Double_t)*lep0_eta, fnwt);
  h.hist[5]->Fill((Double_t)*lep0_phi, fnwt);
  h.hist[6]->Fill((Double_t)*lep0_iso, fnwt);
  h.hist[7]->Fill((Double_t)*lep0_sip3d, fnwt);
  h.hist[8]->Fill((Double_t)*lep0_mt, fnwt);
  h.hist[9]->Fill((Double_t)*lep0_charge, fnwt);

  h.hist[10]->Fill((Double_t)*lep1_pt, fnwt);
  h.hist[11]->Fill((Double_t)*lep1_eta, fnwt);
  h.hist[12]->Fill((Double_t)*lep1_phi, fnwt);
  h.hist[13]->Fill((Double_t)*lep1_iso, fnwt);
  h.hist[14]->Fill((Double_t)*lep1_sip3d, fnwt);
  h.hist[15]->Fill((Double_t)*lep1_mt, fnwt);
  h.hist[16]->Fill((Double_t)*lep1_charge, fnwt);

  h.hist[17]->Fill((Double_t)*dilep_pt, fnwt);
  h.hist[18]->Fill((Double_t)*dilep_eta, fnwt);
  h.hist[19]->Fill((Double_t)*dilep_phi, fnwt);
  h.hist[20]->Fill((Double_t)*dilep_mass, fnwt);
  h.hist[21]->Fill((Double_t)*dilep_mt, fnwt);
  h.hist[22]->Fill((Double_t)*dilep_deta, fnwt);
  h.hist[23]->Fill((Double_t)*dilep_dphi, fnwt);
  h.hist[24]->Fill((Double_t)*dilep_dR, fnwt);
  h.hist[25]->Fill((Double_t)*dilep_ptratio, fnwt);

  h.hist[26]->Fill((Double_t)*HT, fnwt);
  h.hist[27]->Fill((Double_t)*LT, fnwt);
  h.hist[28]->Fill((Double_t)*STvis, fnwt);
  h.hist[29]->Fill((Double_t)*ST, fnwt);
  h.hist[30]->Fill((Double_t)*STfrac, fnwt);
  h.hist[31]->Fill((Double_t)*HTMETllpt, fnwt);
  h.hist[32]->Fill((Double_t)*metpt, fnwt);
  h.hist[33]->Fill((Double_t)*metphi, fnwt);

  h.hist[34]->Fill((Double_t)*dphi_metlep0, fnwt);
  h.hist[35]->Fill((Double_t)*dphi_metlep1, fnwt);
  h.hist[36]->Fill((Double_t)*dphi_metdilep, fnwt);
  h.hist[37]->Fill((Double_t)*dphi_metlep_max, fnwt);
  h.hist[38]->Fill((Double_t)*dphi_metlep_min, fnwt);

  h.hist[39]->Fill((Double_t)*wt_leptonSF, fnwt);
  h.hist[40]->Fill((Double_t)*wt_trig, fnwt);
  h.hist[41]->Fill((Double_t)*wt_pileup, fnwt);
  h.hist[42]->Fill((Double_t)*wt_bjet, fnwt);
  h.hist[43]->Fill((Double_t)*weight, fnwt);

  h.hist[44]->Fill((Double_t)*nnscore_qcd_vlld_2016preVFP, fnwt);
  h.hist[45]->Fill((Double_t)*nnscore_qcd_vlld_2016postVFP, fnwt);
  h.hist[46]->Fill((Double_t)*nnscore_qcd_vlld_2017, fnwt);
  h.hist[47]->Fill((Double_t)*nnscore_qcd_vlld_2018, fnwt);
  h.hist[48]->Fill((Double_t)*nnscore_ttbar_vlld, fnwt);
  
  return kTRUE;
}

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

//Additional functions:
void myHist::SetLastBinAsOverflow(TH1D *hst){

  //Handle overlow:
  int lastBin = hst->GetNbinsX();
  double content  = hst->GetBinContent(lastBin);
  double error    = hst->GetBinError(lastBin);
  double overflow = hst->GetBinContent(lastBin + 1);
  double overflow_err = hst->GetBinError(lastBin + 1);
  double updated_content = content+overflow;
  double updated_error = std::sqrt(error * error + overflow_err * overflow_err);
  hst->SetBinContent(lastBin, updated_content);
  hst->SetBinError(lastBin, updated_error);

  //Handle underflow:
  double content_first  = hst->GetBinContent(1);
  double error_first    = hst->GetBinError(1);
  double underflow      = hst->GetBinContent(0);
  double underflow_err  = hst->GetBinError(0);
  double updated_content_first = content_first + underflow;
  double updated_error_first = std::sqrt(error_first*error_first + underflow_err*underflow_err);
  hst->SetBinContent(1, updated_content_first);
  hst->SetBinError(1, updated_error_first);

  // Resetting underflow and overflow bins to avoid multiple calling of this function.
  hst->SetBinContent(lastBin + 1, 0);
  hst->SetBinError(lastBin + 1, 0);
  hst->SetBinContent(0, 0);
  hst->SetBinError(0, 0);

}

Double_t myHist::getYield(TH1D *hst) {
  Double_t yield = 0;
  yield += hst->GetBinContent(0); //underflow
  for (int bin = 1; bin <= hst->GetNbinsX(); bin++) {
    yield += hst->GetBinContent(bin);
  }
  yield += hst->GetBinContent(hst->GetNbinsX() + 1); //overflow

  return yield;
}
