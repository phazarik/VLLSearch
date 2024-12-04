## Analysis codes
- **HistMaker:** Makes histograms from nanoAOD files.
- **Skimmer:** Skims nanoAOD files and writes them in a smaller tree.
- **TreeMaker:** Makes custom flat ntuples containing event level branches from nanoAOD.

#### How to run each setup
Compile the codes and produce a library in ROOT prompt.
```
[].L AnaScript.C+
```
Locally test the setup:<br>
Test on MC:
```
.x run_local.C("/home/work/alaha1/public/RunII_ULSamples/2018/DYJetsToLL/M50/VLL_DYJetsToLL_M50_1.root", 0)
```
Test on Data:
```
.x run_local.C("/home/work/alaha1/public/RunII_ULSamples/2018/UL2018Data/SingleMuon/SingleMuon_A/SingleMuon_2018A_1.root", 1)
```