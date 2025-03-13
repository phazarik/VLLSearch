import os, sys
import numpy as np
import uproot
import pandas as pd
import joblib
from sklearn.discriminant_analysis import LinearDiscriminantAnalysis
from sklearn.preprocessing import MinMaxScaler

#Global parameters:
indir = '../input_files/trees_modified/'
jobname = 'tree_2018UL_searchregion'
outdir = f'../input_files/trees_modified/tree_2018UL_searchregion_withLDA'
os.makedirs(outdir, exist_ok=True)

modeldict = {
    'LDA-ttbar-vlld-feb13': 'ldascore_ttbar_vlld',
}

#-------------------------------------------
# DON'T TOUCH BELOW:

#-------------------------------------------
# Define functions:
#Given a TFile, read its branches into a dataframe.
def read_file_into_df(filepath, truth=None):

    tfile = uproot.open(filepath)
    
    ttree = tfile['myEvents']
    branches = ttree.keys()
    awkarray = ttree.arrays(branches)
    df = pd.DataFrame(awkarray.to_list())

    return df

def ApplyMinMax(X, modelname, fit_var):
    minmax_filename = f'{modelname}/scaler.pkl'
    scaler = joblib.load(minmax_filename)
    X_scaled = scaler.transform(pd.DataFrame(X, columns=fit_var))
    return X_scaled

def write_df_into_file(df, filepath):
    if df.empty:
        data_dict = {col: np.array([], dtype=df[col].dtype) for col in df.columns}
        print(f"\033[0;31mWarning: Writing empty file: {filepath}\033[0m\n")
    else:
        data_dict = df.to_dict('list')
        
    with uproot.recreate(filepath) as file: file['myEvents'] = data_dict

print('Functions loaded.')

#-----------------------------------------
# Load models and scaling parameters once
models = {}
#scaling_params = {}
for modelname, scorename in modeldict.items():
    model_filename = f'{modelname}/lda_model.pkl' 
    models[modelname] = joblib.load(model_filename)
    print(f'Model loaded: {modelname}')

print('Models loaded.')

#----------------------------------------------------------------------------------
print('\nStarting evaluation ... (hold on, this will take a while)\n')
import time
start_time = time.time()

list_success = []
list_failed  = []

list_of_files = os.listdir(os.path.join(indir, jobname))
for f in list_of_files:

    #Step1: Prepare the dataframe
    print(f'\nLoading file: {f}')
    filepath = os.path.join(indir, jobname, f)
    sample = filepath.split("_")[1]
    subsample = "_".join(filepath.split("_")[2:])
    outfile = os.path.join(outdir, f)
    #if os.path.exists(outfile): continue
    df = read_file_into_df(filepath)
    if df.empty :
        print(f'\033[31mSkipping: File is empty: {filepath}\033[0m')
        list_failed.append(f)
        continue
    
    #Step2 Load the model:
    for modelname, scorename in modeldict.items():

        #Step 2.1: pick which variables to read, and turn those into a numpy array: 
        fit_var = [
            'dilep_pt',
            'dilep_mt',
            'dilep_dphi',
            'dilep_ptratio',
            'LT',
            'ST',
            'STfrac',
            'dphi_metlep0',
            'dphi_metlep1',
            'dphi_metlep_min'
        ]

        X= df[fit_var].values

        print(f'Calculating LDA score for model: {modelname}')
        X_scaled = ApplyMinMax(X, modelname, fit_var)
        print(f'X is scaled with min-max values for model: {modelname}')

        y = models[modelname].transform(X_scaled)
        df[scorename] = y

        #break #model

    write_df_into_file(df, os.path.join(outdir, f))
    list_success.append(f)
    
    print(f'\033[1;33mFile written: {outfile}\033[0m')
    #break #file

end_time = time.time()
time_taken = end_time-start_time

print(' \n\033[33mDone!\033[0m\n')
print('------------------------------------------------------------------------------')
print( '\033[33mSummary:\033[0m')
print(f'\033[33mSuccess : {list_success}\033[0m')
print(f'\033[31mEmpty   : {list_failed}\033[0m')
print(f'\033[33mOutput directory: {outdir}\033[0m')
print(f'\033[33mtime taken : {time_taken:.2f} seconds\033[0m')
print('-------------------------------------------------------------------------------\n')

