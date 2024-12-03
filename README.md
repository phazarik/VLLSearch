#  VLL Search
This repository contains tools for analyzing nanoAOD files. This is my personal work.

## Organizational structure
```.
├── AnaScript
│   └── Analysis Codes folder (HistMaker/Skimmer/TreeMaker)
│       ├── AnaScript.C   -> The main C script
│       ├── AnaScript.h   -> The associated header file containing the class
│       ├── run_condor.C  -> The input-output handler for condor
│       ├── run_local.C   -> For testing codes locally
│       └── cleanup.py    -> Cleaning up the directory
|        + additional header files.
│
├── CondorSetup
│   ├── runana.C -> Loads the ROOT library and runs run_condor.C
│   ├── shell_instructions.sh    -> Runs runana.C in each condor node
│   ├── createJobForOneSample.py -> Submits condor jobs from one directory
│   ├── submitJobsInBulk.py      -> Bulk submission
│   ├── organize_and_hadd.py     -> Adds up outputs from one job (hadd)
│   └── testjob -> For testing the condor setup
|
├── InputJsons   -> Contains information about input samples in json format.
├── LocalScripts -> Everything that happens on a local computer is kept here.
├── Corrections  -> Contains MC corrections as suggested by POGs
└── Setup        -> Contains all the extra header files used by AnaScript.C
```
## Prerequisites
Here is the current setup in the IISER cluster. For the work that happens locally, I am using the updated versions.
| Language / package | Version | Date |
|--|--|--|
|gcc    | 4.8.5   | --- |
|ROOT   | 6.18/04 | Sep 11, 2019|
|python | 3.9.0   | --- |
|[nlohmann/json](https://github.com/nlohmann/json)|---| Oct 15, 2024|

## Instructions
My workflow primarily involves creating custom trees from nanoAOD files (**using TreeMaker**) and keeping event-level branches there (including the correction factors for each event). I read them into pandas dataframes using uproot, put event selections, or train and evaluate neural networks there. After I am done with the analysis part, I create histograms from these branches and make plots. Apart from the tree-making part, the rest of the work happens locally on my laptop.

### Things to check before running
Make sure that the following files contain full paths to the header files.
- `AnaScript/HistMaker/AnaScript.C`
- `AnaScript/Skimmer/AnaScript.C`
- `AnaScript/TreeMaker/AnaScript.C`

The correction factors provided by the POGs are kept as text files in `Corrections/SFfromPOG`. These are extracted from the json files provided by POGs and correctionlib [[documented here](https://github.com/phazarik/correctionlib-to-text)]. The following header file should have a full path to the base directory where these text files are kept.
- `Corrections/loadCorrectionsFromPOG.h`

### Testing the event processor
Before bringing the files locally, I do the following.
- Run **HistMaker** to see whether the setup is well-behaving.
- Run **Skimmer** on all the input files so that I can produce trees iteratively. This has to be done once. I keep only the 2LSS events in the output, which significantly reduces the file size, and its very convenient for me to handle. I use HTcondor to run the skimmer over all files.
- Run **TreeMaker** to produce custom trees that contain event-level variables only. I also add the event weights coming from the MC corrections as separate branches and apply the Jet corrections beforehand. **This has to be done repeatedly for systematic variations**, and hence skimming the files before this step is very handy.

### Condor-job submission
Once the framework works in the master node, HTcondor is used to run the setup on all the data and MC samples in the available job-nodes. The details of the samples used in this analysis (along with cross-section, lumnosity, etc.) are documented in this [spreadsheet](https://docs.google.com/spreadsheets/d/10LLDj0NRvGVtQPhoFLRR2A7NRblJRvnX_U7wvfeYhvw/edit?pli=1&gid=0#gid=0). Details of the job parallelization are documented in the `CondorSetup` section. After the trees are generated, I hadd the files from the same samples using `organize_and_hadd.py` and bring them locally for further analysis.

### Local analysis
The custom trees are read into pandas dataframes. Further event selections, training and evaluation of neural networks etc. are done at this step. Once done with the analysis, the event-level branches are written into histograms for plotting. The following is a summary of packages currently used in my system.
| Language / package | Version | Date |
|--|--|--|
|miniconda3    | 24.9.1   | --- |
|python | 3.10.9   | --- |
|ROOT   | 6.26/10 | Apr 07, 2023|
#### Conda packages
```
awkward                   2.7.0              pyhff2d567_1    conda-forge
correctionlib             2.6.4           py310hca78215_0    conda-forge
ipython                   8.29.0             pyh707e725_0    conda-forge
jupyterlab                4.2.5              pyhd8ed1ab_0    conda-forge
keras                     3.6.0              pyhd8ed1ab_0    conda-forge
notebook                  7.2.2              pyhd8ed1ab_0    conda-forge
numpy                     1.26.4          py310hb13e2d6_0    conda-forge
pandas                    2.2.2           py310hf9f9076_1    conda-forge
scikit-learn              1.5.2           py310h27f47ee_1    conda-forge
scipy                     1.14.1          py310hfcf56fc_1    conda-forge
seaborn                   0.13.2               hd8ed1ab_2    conda-forge
tensorflow                2.17.0      cpu_py310h42475c5_2    conda-forge
uproot                    5.4.1              pyhd8ed1ab_0    conda-forge
```

## Relevant links/tools
- Sample database: [Google spreadsheet](https://docs.google.com/spreadsheets/d/10LLDj0NRvGVtQPhoFLRR2A7NRblJRvnX_U7wvfeYhvw/edit?pli=1&gid=0#gid=0)
- Bringing nanoAOD files from DAS to T2 area: [phazarik/crabSkimSetup](https://github.com/phazarik/crabSkimSetup)
- Simpler analysis template for nanoAOD files: [phazarik/nanoAOD_analyzer](https://github.com/phazarik/nanoAOD_analyzer)
- Extracting POG scale factors in text format: [phazarik/correctionlib-to-text](https://github.com/phazarik/correctionlib-to-text)
