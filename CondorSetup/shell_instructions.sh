#!/bin/bash
source /home/work/phazarik1/.bashrc
source /opt/apps/root-6.18.04-install/bin/thisroot.sh
root -q -b -l /home/work/phazarik1/work/VLLSearch/CondorSetup/runana.C\(\"$1\",\"$2\",\"$3\",\"$4\",\"$5\",\"$6\",\"$7\"\)
