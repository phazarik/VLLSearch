## Analysis codes
- **HistMaker:** Makes histograms from nanoAOD files.
- **Skimmer:** Skims nanoAOD files and writes them in a smaller tree.
- **TreeMaker:** Makes custom flat ntuples containing event level branches from nanoAOD.

### How to run each setup

#### Compile the codes and produce a library in ROOT prompt.
```
[].L AnaScript.C+
```
#### Locally test the setup:<br>
- Test on MC:
```
.x run_local.C("/home/work/alaha1/public/RunII_ULSamples/2018/DYJetsToLL/M50/VLL_DYJetsToLL_M50_1.root", 0)
```
- Test on Data:
```
.x run_local.C("/home/work/alaha1/public/RunII_ULSamples/2018/UL2018Data/SingleMuon/SingleMuon_A/SingleMuon_2018A_1.root", 1)
```
#### Test the driver script for condor:
```
.x run_condor.C("/home/work/alaha1/public/RunII_ULSamples/2018/DYJetsToLL/M50/VLL_DYJetsToLL_M50_1.root", "test_outputs/test_anaCond.root", "0", "2018_UL", "dy", "DYJetsToLL_M50")
```
#### More example files to test  with:
```
/home/work/alaha1/public/RunII_ULSamples/2017/DYJetsToLL/M50/VLL_DYJetsToLL_M50_1.root
/home/work/alaha1/public/RunII_ULSamples/2017/UL2017Data/SingleMuon/SingleMuon_B/SingleMuonRun2017B_tree_1.root
/home/work/alaha1/public/RunII_ULSamples/2016/DYJetsToLL/preVFP/M50/VLL_DYJetsToLLM50_preVFP_1.root
/home/work/alaha1/public/RunII_ULSamples/2016/UL2016Data/SingleMuon/preVFP/SingleMuon_B/SingleMuon_2016BpreVFP_ver2_1.root
/home/work/alaha1/public/RunII_ULSamples/2016/DYJetsToLL/postVFP/M50/VLL_DYJetsToLLM50_postVFP_1.root
/home/work/alaha1/public/RunII_ULSamples/2016/UL2016Data/SingleMuon/postVFP/SingleMuon_F/SingleMuon_2016FpostVFP_1.root
```