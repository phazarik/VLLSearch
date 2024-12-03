#!/bin/bash
source /home/work/phazarik1/.bashrc
source /opt/apps/root-6.18.04-install/bin/thisroot.sh

# Ensure that 7 arguments are passed
if [ "$#" -ne 7 ]; then
    echo ""
    echo -e "\033[31mError: 7 arguments required: codedir, ifname, ofname, data, campaign, flag, samplename\033[0m"
    echo "Example of a test tun:"
    echo -e "\033[93m. testjob.sh /home/work/phazarik1/work/VLLSearch/AnaScript/HistMaker /home/work/alaha1/public/RunII_ULSamples/2018/DYJetsToLL/M50/VLL_DYJetsToLL_M50_98.root demo_test.root 0 2018_UL dy DYJetsToLL_M50\033[0m"
    echo ""
else
    # Pass all 7 arguments to the ROOT macro
    root -q -b -l /home/work/phazarik1/work/VLLSearch/CondorSetup/runana.C\(\"$1\",\"$2\",\"$3\",\"$4\",\"$5\",\"$6\",\"$7\"\)
fi