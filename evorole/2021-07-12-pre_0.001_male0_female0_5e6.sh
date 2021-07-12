#!/bin/bash

#!!! adjust the time quota !!!

#SBATCH --time=10:00:00
#SBATCH --partition=gelifes
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --mem=5GB
#SBATCH --cpus-per-task=24

module load GCC --latest

datadir="/data/$(whoami)"
./bin/evorole --force param=2021-07-12-pre_0.001_male0_female0_5e6.json rep=10 outdir=$datadir/2021-07-12-pre_0.001_male0_female0_5e6
