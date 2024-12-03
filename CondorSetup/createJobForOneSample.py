#-------------------------------------------------------------------------------
# Creates a condor job for only one sample (eg. DYJetsToLL_M50)                
# It uses runana.C, which is kept in the same directory as the condor scripts. 
#
# Arguments needed:
# 1. Which code to run (HistMaker, StackMaker or TreeMaker)
# 2. Input folder containing nanoAOD files.
# 3. jobname to set the output directory.
# 4. Data or not (1 or 0)
# 5. Campaign name (Options: 2016preVFP_UL, 2016postVFP_UL, 2017_UL, 2018_UL)
# 6. Sample and subsample names (e.g. DYJetsToLL and M50)
# 7. Flag for specific samples
# 8. Input file type (original nanoAOD, or skimmed)
# 9. Additional flags for debugging
#
# Note: All the inputs are strings, because runana.C expects strings.
#-------------------------------------------------------------------------------

import os, sys, argparse
from datetime import datetime

timestamp = datetime.now().strftime("%Y-%m-%d")

parser=argparse.ArgumentParser()
### Necessary arguments:
parser.add_argument('--mode',     type=str, required=True,  help='Mode: hist, skim or tree')
parser.add_argument('--indir',    type=str, required=True,  help='Input  directory')
parser.add_argument('--jobname',  type=str, required=True,  help='Jobname')
parser.add_argument('--data',     type=str, required=True,  help='Data = 1, MC = 0')
parser.add_argument('--campaign', type=str, required=True,  help='Standard campaign name')
parser.add_argument('--flag',     type=str, required=True,  help='Flagging specific sample')
parser.add_argument('--type',     type=str, required=True,  help='File type: normal or skimmed')
parser.add_argument('--sample',   type=str, required=True,  help='Samplen ame. e.g. DYJetsToLL')
parser.add_argument('--subsample',type=str, required=True,  help='Sub-sample name. e.g. M50')
### Optional arguments:
parser.add_argument('--test' , type=bool, required=False, help='Submit only one job',    default=False)
parser.add_argument('--debug', type=bool, required=False, help='Print debug statements', default=False)
args=parser.parse_args()

toTest  = args.test
toDebug = args.debug 
if toDebug: print('\033[093m\nDebug mode: createJobForOneSample.py\033[0m')

### Selecting codes to run:
runana  = 'runana.C'
codedir = None
if args.mode   == 'hist': codedir = '/home/work/phazarik1/work/VLLSearch/AnaScript/HistMaker'
elif args.mode == 'skim': codedir = '/home/work/phazarik1/work/VLLSearch/AnaScript/Skimmer'
elif args.mode == 'tree': codedir = '/home/work/phazarik1/work/VLLSearch/AnaScript/TreeMaker'
else: print('\033[31mChoose a correct code directory!\033[0m')
prefix = args.mode

### Set output directories:
dump = '/home/work/phazarik1/work/CondorDump'
fullsample = args.sample + '_' + args.subsample
output_tag = fullsample+'_'+timestamp
outdir = os.path.join(dump, 'output', args.jobname, output_tag)
logdir = os.path.join(dump, 'log',    args.jobname, output_tag)

if not toDebug:
    print('Creating output folders.')
    os.makedirs(outdir, exist_ok=True)
    os.makedirs(logdir, exist_ok=True)
    os.system(f'chmod -R 777 {outdir}')
    os.system(f'chmod -R 777 {logdir}')
else:
    print(f'outdir = {outdir}')
    print(f'logdir = {logdir}')

### Generate the shell script for submitting jobs:
shell_instructions = r'''#!/bin/bash
source /home/work/phazarik1/.bashrc
source /opt/apps/root-6.18.04-install/bin/thisroot.sh
root -q -b -l /home/work/phazarik1/work/VLLSearch/CondorSetup/runana.C\(\"$1\",\"$2\",\"$3\",\"$4\",\"$5\",\"$6\",\"$7\"\)
'''

with open('shell_instructions.sh', 'w') as script_file: script_file.write(shell_instructions)
os.system('chmod a+x shell_instructions.sh')
if toDebug: print('Shell script written.')

### Write condor instructions to run this this shell script:
condor_info = [
'universe = vanilla',
'Requirements = (Arch == "X86_64")',
'Executable = /home/work/phazarik1/work/VLLSearch/CondorSetup/shell_instructions.sh',
'should_transfer_files = YES',
'Notification=never',
'',
''
]

### Instructions for each file to be appended to the header:
files = os.listdir(args.indir)
if len(files) == 0: print('\033[31mCould not find files in : '+args.indir+' \033[0m')

filecount = 0
for rootfile in files:
    if not rootfile.endswith('root'): continue
    filecount+=1

    infilepath  = os.path.join(args.indir, rootfile)
    outfilename = prefix+'_'+args.sample+'_'+args.subsample+'.root'
    outfilepath = os.path.join(outdir, outfilename)
    arguments = f'{codedir} {infilepath} {outfilepath} {args.data} {args.campaign} {args.flag} {fullsample}'

    if toDebug:
        print(f'\nFile {filecount}:')
        print('Input  file = '+rootfile)
        print('Output file = '+outfilename)
        print('Arguments = '+arguments)

    info_for_each_file = [
        "output = "+logdir+"//$(Cluster)_log_"+output_tag+".out",
        "error = " +logdir+"//$(Cluster)_log_"+output_tag+".err",
        "log = "   +logdir+"//$(Cluster)_log_"+output_tag+".log",
        "arguments = "+arguments,
        "queue",
        ""
        ]

    condor_info += info_for_each_file
    #if toTest: break
    
### Write these instuctions in a condor file:
condorfile = 'submitJobsCondor_'+output_tag+'.condor'
with open(condorfile, 'w') as file:
    for string in condor_info:
        file.write(string + '\n')

os.system("chmod a+x "+condorfile)
if toDebug: print('Condor file written.')

### Submit the condor job:
processline = "condor_submit "+condorfile

print("\n\033[33m______"+fullsample+"______\033[0m")
print("Input directory : "+args.indir)
print("Output directory: "+outdir)
print("Code directory: "  +codedir)

if toDebug : print('processline = '+processline)
else:
    os.system("eval "+processline)
    print('\033[33mSubmit successfull.\033[0m')

### Move the condor file to a different directory:
dumpcondorpath = os.path.join('previous_jobs', timestamp, args.jobname, args.sample)
os.makedirs(dumpcondorpath, exist_ok=True)
os.system("mv "+condorfile+" "+dumpcondorpath+"/.")
print('condor file: '+os.path.join(dumpcondorpath, condorfile)+'\n')
    
    

    




