#!/bin/bash

#!!! adjust the time quota !!!

#SBATCH --time=01:15:00
#SBATCH --partition=gelifes
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --mem=15GB
#SBATCH --cpus-per-task=16

module load GCC --latest

datadir="/data/$(whoami)"
./bin/evorole --force param=pre_male_0.json rep=50 outdir=$datadir/2021-07-02-pre_male_0
