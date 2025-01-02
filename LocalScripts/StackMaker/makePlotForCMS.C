#include <iostream>
#include <fstream>
#include <vector>
#include <utility> // for std::pair
#include <algorithm>
#include <map>
#include <iomanip>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TH1F.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TMathText.h"
#include "TLine.h"
using namespace std;

//Global parameters used by the other headers:
TString input_path;
int     nbins, rebin;
float   xmin, xmax;
float   globalSbyB, globalObsbyExp, globalObsbyExpErr;
double  QCDscale, TOPscale, DYscale;
bool    toLog, toZoom, toSave, toOverlayData;
TString campaign, campaign_name;
TString channel;
TString tag1, tag2, tag3, info;

//My header files:
#include "common_headers/settings.h"
#include "common_headers/decorations.h"
#include "common_headers/setglobalparameters.h"

//Function declaration:
void plot(TString var, TString name);

//main:
void makePlotForCMS(TString _var = "HT", TString _name = "HT (GeV)", int _nbins = 200, float _xmin = 0.0, float _xmax = 1000, int _rebin = 1){
//void makePlotForCMS(TString _var = "njet", TString _name = "nJet", int _nbins = 10, float _xmin = 0.0, float _xmax = 10, int _rebin = 1){
  
  //Set global parameters:
  channel = "mm";
  campaign = "2018_UL";
  toSave = false;
  toOverlayData = true;

  TString jobname = "hist_test_"+channel;
  input_path = "../input_files/hists/"+jobname;
  tag1 = campaign+"_topcr_"+channel; //folder name
  info = "t#bar{t} CR"; //Event selection
  tag3 = ""; //Additional info
  rebin = _rebin;
  
  InitializeValues();

  cout<<"Making plot for "<<_name<<endl;
  plot(_var, _name);
  cout<<"Done!"<<endl;
  
}//End macro

//________________________________________________________________________________________________________________
//
// Plotmaker for one histogram:
//________________________________________________________________________________________________________________

void plot(TString var, TString name){

  Double_t ymin = 0.1;
  Double_t ymax = 10E6;

  TString date_stamp  = todays_date();
  TString dump_folder = "plots/"+date_stamp+"_"+tag1;
  TString filename = dump_folder+"/"+tag1+"_"+var;

  //------------------
  //Reading from file:
  //------------------
  vector<TH1F *> QCD = {       
    get_hist(var, "QCD_MuEnriched", "20to30"),
    get_hist(var, "QCD_MuEnriched", "30to50"),
    get_hist(var, "QCD_MuEnriched", "50to80"),
    get_hist(var, "QCD_MuEnriched", "80to120"),
    get_hist(var, "QCD_MuEnriched", "120to170"),
    get_hist(var, "QCD_MuEnriched", "170to300"),
    get_hist(var, "QCD_MuEnriched", "300to470"),
    get_hist(var, "QCD_MuEnriched", "470to600"),
    get_hist(var, "QCD_MuEnriched", "600to800"),
    get_hist(var, "QCD_MuEnriched", "800to1000"),
    get_hist(var, "QCD_EMEnriched", "15to20"),
    get_hist(var, "QCD_EMEnriched", "20to30"),
    get_hist(var, "QCD_EMEnriched", "30to50"),
    get_hist(var, "QCD_EMEnriched", "50to80"),
    get_hist(var, "QCD_EMEnriched", "80to120"),
    get_hist(var, "QCD_EMEnriched", "120to170"),
    get_hist(var, "QCD_EMEnriched", "170to300"),
    get_hist(var, "QCD_EMEnriched", "300toInf"),
  };
  //Bakcgrounds with single-top:
  vector<TH1F *>ST = {
    get_hist(var, "SingleTop", "s-channel_LeptonDecays"),
    get_hist(var, "SingleTop", "t-channel_AntiTop_InclusiveDecays"),
    get_hist(var, "SingleTop", "t-channel_Top_InclusiveDecays"),
    get_hist(var, "SingleTop", "tW_AntiTop_InclusiveDecays"),
    get_hist(var, "SingleTop", "tW_Top_InclusiveDecays")
  };
  vector<TH1F *>THX = {
    get_hist(var, "Rare", "THQ"),
    get_hist(var, "Rare", "THW"),
  };
  vector<TH1F *>TZq = {
    get_hist(var, "Rare", "tZq_ll"),
  };
  //Backgrounds with multiple tops:
  vector<TH1F *>TTBar={
    get_hist(var, "TTBar", "TTTo2L2Nu"),
    get_hist(var, "TTBar", "TTToSemiLeptonic"),
  };
  vector<TH1F *>TTW ={
    get_hist(var, "TTW", "TTWToLNu")
  };
  vector<TH1F *>TTZ ={
    get_hist(var, "TTZ", "TTZToLL")
  };
  vector<TH1F *>TTX={
    get_hist(var, "Rare", "TTHH"),
    get_hist(var, "Rare", "TTTJ"),
    get_hist(var, "Rare", "TTTT"),
    get_hist(var, "Rare", "TTTW"),
    get_hist(var, "Rare", "TTWH"),
    get_hist(var, "Rare", "TTWW"),
    get_hist(var, "Rare", "TTWZ"),
    get_hist(var, "Rare", "TTZH"),
    get_hist(var, "Rare", "TTZZ")
  };
  //Backgrounds with single V:
  vector<TH1F *>WJets = {
    get_hist(var, "HTbinnedWJets", "70to100"),
    get_hist(var, "HTbinnedWJets", "100to200"),
    get_hist(var, "HTbinnedWJets", "200to400"),
    get_hist(var, "HTbinnedWJets", "400to600"),
    get_hist(var, "HTbinnedWJets", "600to800"),
    get_hist(var, "HTbinnedWJets", "800to1200"),
    get_hist(var, "HTbinnedWJets", "1200to2500"),
    get_hist(var, "HTbinnedWJets", "2500toInf"),
  };
  vector<TH1F *>WGamma={
    get_hist(var, "WGamma", "WGToLNuG_01J")
  };
  vector<TH1F *> DY = {
    get_hist(var, "DYJetsToLL", "M10to50"),
    get_hist(var, "DYJetsToLL", "M50")
  };
  vector<TH1F *>ZGamma={
    get_hist(var, "ZGamma", "ZGToLLG_01J")
  }; 
  //Backgrounds with VV:
  vector<TH1F *>VV={
    get_hist(var, "WW", "WWTo1L1Nu2Q"),
    get_hist(var, "WW", "WWTo2L2Nu"),
    get_hist(var, "WW", "WWTo4Q"),
    get_hist(var, "WZ", "WZTo1L1Nu2Q"),
    get_hist(var, "WZ", "WZTo2L2Q"),
    get_hist(var, "WZ", "WZTo3LNu"),
    get_hist(var, "ZZ", "ZZTo2L2Nu"),
    get_hist(var, "ZZ", "ZZTo2Q2L"),
    get_hist(var, "ZZ", "ZZTo2Q2Nu"),
    get_hist(var, "ZZ", "ZZTo4L"),
  };
  vector<TH1F *>WpWp={
    get_hist(var, "WpWp", "WpWpJJEWK"),
    get_hist(var, "WpWp", "WpWpJJQCD"),
  };
  //Backgrounds with VVV:
  vector<TH1F *>VVV={
    get_hist(var, "WWW", "Inclusive"),
    get_hist(var, "WWZ", "Inclusive"),
    get_hist(var, "WZZ", "Inclusive"),
    get_hist(var, "ZZZ", "Inclusive"),
  };
  //Higgs processes:
  vector<TH1F *>Higgs={
    get_hist(var, "Higgs", "bbH_HToZZTo4L"),
    get_hist(var, "Higgs", "GluGluHToZZTo4L"),
    get_hist(var, "Higgs", "GluGluToZH_HToZZTo4L"),
    get_hist(var, "Higgs", "GluGluZH_HToWW_ZTo2L"),
    get_hist(var, "Higgs", "ttHToNonbb"),
    get_hist(var, "Higgs", "VBF_HToZZTo4L"),
    get_hist(var, "Higgs", "VHToNonbb")
  };
  //Data:
  vector<TH1F *>SingleMuon={
    get_hist(var, "SingleMuon", "SingleMuon_A"),
    get_hist(var, "SingleMuon", "SingleMuon_B"),
    get_hist(var, "SingleMuon", "SingleMuon_C"),
    get_hist(var, "SingleMuon", "SingleMuon_D"),
    get_hist(var, "SingleMuon", "SingleMuon_E"),
    get_hist(var, "SingleMuon", "SingleMuon_F"),
    get_hist(var, "SingleMuon", "SingleMuon_G"),
    get_hist(var, "SingleMuon", "SingleMuon_H"),
  };
  vector<TH1F *>EGamma={
    get_hist(var, "EGamma", "EGamma_A"),
    get_hist(var, "EGamma", "EGamma_B"),
    get_hist(var, "EGamma", "EGamma_C"),
    get_hist(var, "EGamma", "EGamma_D"),
    get_hist(var, "EGamma", "EGamma_E"),
    get_hist(var, "EGamma", "EGamma_F"),
    get_hist(var, "EGamma", "EGamma_G"),
    get_hist(var, "EGamma", "EGamma_H"),
  };
  cout<<"Histograms loaded."<<endl;

  //______________________________________________________________

  //           BACKGROUNDS: Adding and stacking
  //______________________________________________________________

  vector<TH1F *> bkg; bkg.clear();
  bkg = {
    merge_and_decorate(QCD,   "QCD",       kYellow),
    //Top backgrounds: more top = darker blue
    merge_and_decorate(ST,    "Single t",  kCyan-7),
    merge_and_decorate(TZq,   "tZq",       kCyan-2),
    merge_and_decorate(THX,   "tH+X",      kViolet+1),
    merge_and_decorate(TTBar, "t#bar{t}",  kAzure+1),
    merge_and_decorate(TTW,   "t#bar{t}W", kAzure+2),
    merge_and_decorate(TTZ,   "t#bar{t}Z", kBlue-3),
    merge_and_decorate(TTX,   "t#bar{t}+X",kBlue+1),
    //Singe V backgrounds: red for Z, gray for W
    merge_and_decorate(DY,    "DY",        kRed-7),
    merge_and_decorate(WJets, "W+jets",    kGray+2),
    merge_and_decorate(ZGamma,"Z#gamma",   kRed-9),
    merge_and_decorate(WGamma,"W#gamma",   kGray+1),
    //VV and VVV: more V: darker green
    merge_and_decorate(VV,    "VV",        kGreen+1),
    merge_and_decorate(WpWp,  "WpWp",      kGreen-5),
    merge_and_decorate(VVV,   "VVV",       kGreen+3),
    //Higgs:
    merge_and_decorate(Higgs, "Higgs",     kMagenta),
  };
  std::vector<TH1F*> nulls_removed;
  for(size_t i = 0; i < bkg.size(); ++i){
    if   (bkg[i] != nullptr) nulls_removed.push_back(bkg[i]);
    else DisplayText("Remove null hist: "+(TString)bkg[i]->GetName(), 31);
  }
  bkg = nulls_removed;

  //Summing over the backgrounds:
  TH1F *allbkg = (TH1F *)bkg[0]->Clone(); allbkg->Reset();
  for(int i=0; i<(int)bkg.size(); i++) allbkg->Add(bkg[i]);
  allbkg->SetFillStyle(3013);
  allbkg->SetFillColor(kGray+3);
  allbkg->SetMarkerSize(0); //No marker
  allbkg->SetName("All bkg");

  //Stacking:
  std::sort(bkg.begin(), bkg.end(), compareHists);
  THStack *bkgstack = new THStack("Stacked",var+";"+var+";Events");
  for(int i=0; i<(int)bkg.size(); i++) bkgstack->Add(bkg[i]);
  //bkgstack->Add(hst_qcd);
  SetFillColorFromLineColor(bkgstack);
  bkgstack->SetTitle("");

  cout<<"Backgrounds ready!"<<endl;

  
  //______________________________________________________________
  
  //                    SIGNAL COLLECTION
  //______________________________________________________________

  TH1F *sig1, *sig2, *sig3;
  
  if(channel == "ee"){
    sig1 = get_hist(var, "VLLD_ele", "M400"); if(sig1) {SetHistoStyle(sig1, kRed+0); sig1->SetTitle("VLLD e_{400}");}
    sig2 = get_hist(var, "VLLD_ele", "M200"); if(sig2) {SetHistoStyle(sig2, kRed+2); sig2->SetTitle("VLLD e_{200}");}
  }
  else if (channel == "em"){
    sig1 = get_hist(var, "VLLD_ele", "M400"); if(sig1) {SetHistoStyle(sig1, kRed+0); sig1->SetTitle("VLLD e_{400}");}
    sig2 = get_hist(var, "VLLD_mu", "M400");  if(sig2) {SetHistoStyle(sig2, kRed+2); sig2->SetTitle("VLLD #mu_{400}");}
  }
  else if (channel == "me"){
    sig1 = get_hist(var, "VLLD_mu", "M400");  if(sig1) {SetHistoStyle(sig1, kRed+0); sig1->SetTitle("VLLD #mu_{400}");}
    sig2 = get_hist(var, "VLLD_ele", "M400"); if(sig2) {SetHistoStyle(sig2, kRed+2); sig2->SetTitle("VLLD e_{400}");}
  }
  else if (channel == "mm"){
    sig1 = get_hist(var, "VLLD_mu", "M400");  if(sig1) {SetHistoStyle(sig1, kRed+0); sig1->SetTitle("VLLD #mu_{400}");}
    sig2 = get_hist(var, "VLLD_mu", "M200");  if(sig2) {SetHistoStyle(sig2, kRed+2); sig2->SetTitle("VLLD #mu_{200}");}
  }
  sig3 = nullptr;

  vector<TH1F*> sigvec = {sig1, sig2, sig3};
  cout<<"Signal ready!"<<endl;
  
  //______________________________________________________________
  
  //                           DATA
  //______________________________________________________________

  TH1F *hst_smuon = merge_and_decorate(SingleMuon, "SingleMuon", kBlack);
  TH1F *hst_egamma= merge_and_decorate(EGamma,     "EGamma",     kBlack);

  TH1F *hst_data = (TH1F *)hst_smuon->Clone();
  if(hst_egamma) hst_data->Add(hst_egamma);
  hst_data->SetName("Data");

  cout<<"Data ready!"<<endl;

  //______________________________________________________________
  
  //                     ON-SCREEN DISPLAYS
  //______________________________________________________________


  // Yield for each background:
  if(var=="HT"){
    DisplayText("\nYields with uncertainty for each sample:", 33);
    cout<<fixed<<setprecision(2);
    Double_t sum_bkg = 0; Double_t sum_bkg_sqerr = 0;
    for(int i=(int)bkg.size()-1; i>=0; i--){
      cout<<setw(15)<<left<<bkg[i]->GetTitle()<<"\t"<<bkg[i]->Integral()<<"\\pm"<<GetStatUncertainty(bkg[i])<<endl;
      sum_bkg += bkg[i]->Integral();
      sum_bkg_sqerr += pow(GetStatUncertainty(bkg[i]), 2);
    }
    cout<<"-------------------------------------------"<<endl;
    cout<<setw(15)<<left<<"Total bkg\t"<<sum_bkg<<"\\pm"<<sqrt(sum_bkg_sqerr)<<endl;
    if(sig1) cout<<setw(15)<<left<<sig1->GetTitle()<<"\t"<<sig1->Integral()<<"\\pm"<<GetStatUncertainty(sig1)<<endl;
    if(sig2) cout<<setw(15)<<left<<sig2->GetTitle()<<"\t"<<sig2->Integral()<<"\\pm"<<GetStatUncertainty(sig2)<<endl;
    if(sig3) cout<<setw(15)<<left<<sig3->GetTitle()<<"\t"<<sig3->Integral()<<"\\pm"<<GetStatUncertainty(sig3)<<endl;
    if(toOverlayData) cout<<setw(15)<<left<<"\nData\t"<<right<<hst_data->Integral()<<"\\pm"<<GetStatUncertainty(hst_data)<<endl;
    cout<<defaultfloat<<endl;

    //QCD scale-factor:
    Double_t qcd_yield = 0;
    Double_t other_yield = 0;
    for(int i=(int)bkg.size()-1; i>=0; i--){
      if((TString)bkg[i]->GetName()=="QCD") qcd_yield    = bkg[i]->Integral();
      else                                  other_yield += bkg[i]->Integral();
    }
    Double_t data_yield = hst_data->Integral();
    Double_t QCD_sf =  (data_yield-other_yield)/qcd_yield;
    cout<<fixed<<setprecision(2);
    DisplayText("QCD scale factor calculation:", 33);
    cout<<"Data      = "<<data_yield<<endl;
    cout<<"QCD       = "<<qcd_yield <<endl;
    cout<<"Other bkg = "<<other_yield <<endl;
    cout<<fixed<<setprecision(8);
    cout<<"QCD scale factor (data-others/QCD) = \033[33m"<<QCD_sf<<"\033[0m"<<endl;
    cout<<defaultfloat<<endl;

    GetBinwiseSF(var, hst_data, bkg, "t#bar{t}");
  }

  //______________________________________________________________
  
  //                     PLOTTING ON CANVAS
  //______________________________________________________________


  TCanvas *canvas = create_canvas(var, filename, 600, 600);
  TPad *mainPad   = create_mainPad(0.0, 0.3, 1.0, 1.0);   // Full width, 70% height, standard CMS margins
  TPad *ratioPad  = create_ratioPad(0.0, 0.0, 1.0, 0.3); // Full width, 30% height, standard CMS margins
  mainPad->Draw(); ratioPad->Draw();

  //-------------------------------------------------------------
  //                     Drawing on mainpad
  //-------------------------------------------------------------

  mainPad->cd();
  
  // 1) Setting up a dummy hist to enforce display style in the plot. It should always be drawn first.
  TH1F *dummy = (TH1F *)bkg[0]->Clone(); dummy->Reset();
  dummy->SetTitle("");
  dummy->GetYaxis()->SetTitle("Events");
  dummy->GetYaxis()->SetTitleSize(0.07);
  dummy->GetYaxis()->SetTitleOffset(1.00);
  dummy->GetYaxis()->SetLabelSize(0.05);
  dummy->GetYaxis()->SetTickSize(0.01);
  dummy->GetYaxis()->SetRangeUser(ymin, ymax); //10E8 for baseline, #10E5 for topCR
  dummy->GetXaxis()->SetTickSize(0.01);
  if(toZoom) dummy->GetXaxis()->SetRangeUser(xmin, xmax);
  dummy->SetStats(0);
  dummy->Draw("hist");

  // 2) Drawing the rest:
  bkgstack->Draw("hist same"); //Stacked (total) background
  allbkg->Draw("E2 same");     //Uncertainty on the total background
  TH1F *allbkg2 = (TH1F*) allbkg->Clone();
  allbkg2->SetFillColor(0);
  allbkg2->SetLineColor(kGray+3);
  allbkg2->Draw("hist same");  //Black hist over the stack (to outline the total background)
  if(sig1) sig1->Draw("HIST same");
  if(sig2) sig2->Draw("HIST same");
  if(sig3) sig3->Draw("HIST same");
  if(toOverlayData) hst_data->Draw("ep same");
  dummy->Draw("sameaxis"); //Drawing axis ticks on top

  // 3) Done. Update the canvas.
  canvas->Update();

  //-------------------------------------------------------------
  //                     Drawing on ratiopad
  //-------------------------------------------------------------

  ratioPad->cd();

  // 1) Calculation of S/sqrtB and plotting:
  globalSbyB = 0;
  TH1F *sbyrb = nullptr;
  if     (sig1) sbyrb = GetSbyRootB(sig1, bkg);
  else if(sig2) sbyrb = GetSbyRootB(sig2, bkg);
  else if(sig3) sbyrb = GetSbyRootB(sig3, bkg);
  if(sbyrb){
    SetRatioStyle(sbyrb, name);
    sbyrb->GetYaxis()->SetTitle("S/\\sqrt{B}");
    sbyrb->GetYaxis()->SetTitleSize(0.15);
    sbyrb->GetYaxis()->SetTitleOffset(0.43);
    sbyrb->GetYaxis()->SetLabelSize(0.13);
    if(toZoom) sbyrb->GetXaxis()->SetRangeUser(xmin, xmax);
    if(!toOverlayData) sbyrb->Draw("ep");
    canvas->Update();
  }
  else DisplayText("Warning: SbyRootB is null!");
  
  // 2) Calculation of Obs/Exp and plotting (in case data is overlaid):
  TH1F *ratiohist = nullptr;
  globalObsbyExp =0;
  if(toOverlayData){
    ratiohist = GetRatio(hst_data, bkg);
  
    //Setting up a horizontal line on the ratiopad:
    float xlow  = ratiohist->GetXaxis()->GetBinLowEdge(1);
    float xhigh = ratiohist->GetXaxis()->GetBinUpEdge(ratiohist->GetNbinsX());
    TLine *line = new TLine(xlow, 1, xhigh, 1);
    line->SetLineColor(kRed);
    line->SetLineWidth(2);
    
    //Calculating the uncertainty on the background in each bin:
    TGraphErrors *err = GetUncertainty(allbkg);
    err->GetYaxis()->SetNdivisions(5, kTRUE);
    err->SetStats(0);
    if(toZoom) err->GetXaxis()->SetRangeUser(xmin, xmax);

    //-----------------------------
    // Special corrections in bins:
    //-----------------------------
    
    if(var == "LT"){
      TH1F* data_minus_bkg = (TH1F *)hst_data->Clone();
      for(int i=0; i<(int)bkg.size(); i++){
	if(TString (bkg[i]->GetName()) != "t#bar{t}"){
	  data_minus_bkg->Add(bkg[i], -1);
	}
      }
      ratiohist->Reset();
      for(int i=0; i<(int)bkg.size(); i++){
	if(TString (bkg[i]->GetName()) == "t#bar{t}"){
	  data_minus_bkg->Divide(bkg[i]);
	  ratiohist = data_minus_bkg;
	}
      }
    }//Special
    
    //Drawing everything in the proper order:
    SetRatioStyle(ratiohist, name);
    ratiohist->GetYaxis()->SetTitle("obs/exp");
    if(var == "LT") ratiohist->GetYaxis()->SetTitle("SF");
    ratiohist->GetYaxis()->SetTitleSize(0.15);
    ratiohist->GetYaxis()->SetTitleOffset(0.43);
    ratiohist->GetYaxis()->SetLabelSize(0.13);
    ratiohist->GetYaxis()->SetRangeUser(0, 2.2);
    if(toZoom) ratiohist->GetXaxis()->SetRangeUser(xmin, xmax);

    ratiohist->Draw("ep"); //Inheriting the settings from the ratio hist.
    err->Draw("SAME P E2");
    line->Draw("same");
    ratiohist->Draw("ep same"); //I want the ratio to be on top.
    canvas->Update();
  }

  // 3) Drawing veto region for Drell-Yan:
  if(channel=="ee" && var=="dilep_mass") draw_veto_region(ratioPad, 76, 106);
  
  //-------------------------------------------------------------
  //                    Texts and legend
  //-------------------------------------------------------------

  mainPad->cd();

  put_text("CMS", 0.17, 0.83, 62, 0.07);          // Larger, bold CMS label
  put_text("Preliminary", 0.27, 0.83, 52, 0.05);  // Smaller preliminary label
  if(campaign == "2016preVFP_UL")  put_latex_text(campaign_name, 0.62, 0.94, 42, 0.05);
  if(campaign == "2016postVFP_UL") put_latex_text(campaign_name, 0.60, 0.94, 42, 0.05);
  if(campaign == "2017_UL")        put_latex_text(campaign_name, 0.74, 0.94, 42, 0.05);
  if(campaign == "2018_UL")        put_latex_text(campaign_name, 0.74, 0.94, 42, 0.05);
  put_latex_text(tag2, 0.17, 0.78, 42, 0.04);     //Additional information
  put_latex_text(tag3, 0.17, 0.73, 42, 0.04);     //Additional information

  TLegend *lg = create_legend(0.48, 0.52, 0.94, 0.89); lg->SetNColumns(2);
  if(toOverlayData){
    TString data_yield = Form("%d", (int)hst_data->Integral());
    TString text = "Observed ["+data_yield+"]";
    lg->AddEntry(hst_data, text, "f");
    if(hst_smuon)  SetLegendEntry(lg, hst_smuon);
    if(hst_egamma) SetLegendEntry(lg, hst_egamma);
  }

  for(int i=(int)bkg.size()-1; i>=0; i--) SetLegendEntry(lg, bkg[i]);
  if(sig1) SetLegendEntry(lg, sig1);
  if(sig2) SetLegendEntry(lg, sig2);
  if(sig3) SetLegendEntry(lg, sig3);
  TString legendheader = Form("Global S/#sqrt{B} = %.3f", globalSbyB);
  if(toOverlayData){
    TString val = Form("Global obs/exp = %.3f", globalObsbyExp);
    TString err = Form("%.3f", globalObsbyExpErr);
    legendheader = val+" #pm "+err;
    cout << fixed << setprecision(3);
    cout << "Obs/Exp = " << globalObsbyExp << " ± " << globalObsbyExpErr << "\n";
    cout << defaultfloat << endl;
  }
  else cout<<legendheader<<"\n"<<endl; 
  lg->SetHeader(legendheader);
  lg->Draw();
  canvas->Update();
  
  //-------------------------------------------------------------
  //                    Output management
  //-------------------------------------------------------------
  if(toSave){
    createFolder(dump_folder);
    canvas->SaveAs(filename+".png");
  }

  /*
  //Cleanup:
  delete allbkg;
  delete bkgstack;
  delete sig1;
  delete sig2;
  delete sig3;
  delete hst_smuon;
  delete hst_egamma;
  delete hst_data;
  delete dummy;
  if(sbyrb) delete sbyrb;
  if(ratiohist) delete ratiohist;
  for (auto hist : bkg)  delete hist;*/

  /*
  bkg.clear();
  sigvec.clear();
  QCD.clear();
  ST.clear();
  THX.clear();
  TZq.clear();
  TTBar.clear();
  TTW.clear();
  TTZ.clear();
  TTX.clear();
  WJets.clear();
  WGamma.clear();
  DY.clear();
  ZGamma.clear();
  VV.clear();
  WpWp.clear();
  VVV.clear();
  Higgs.clear();
  SingleMuon.clear();
  EGamma.clear();*/

}//End plotmaker
