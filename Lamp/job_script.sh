#!/bin/bash
# Slurm directives
#SBATCH --job-name=root_c_program
#SBATCH --output=output.log
#SBATCH --error=error.log
#SBATCH --ntasks=1
#SBATCH --time=0:15:00 # Request 15 min of runtime
#SBATCH --mem=8G # Request 8GM of memory
#SBATCH --account=rrg-jmammei
#SBATCH --job-name=SpectrumAnalyze
#SBATCH --output=job.out
#SBATCH --err=err.out

# Load any necessary modules or set environment variables here
soruce ~/projects/rrg-jmammei/REMOLL/enviroment/cedar_env_2.0.sh
# module load some_module

# Run the ROOT C++ program using root.exe
root -b -l -q readInSpec.C

echo "C++ program executed using ROOT"
