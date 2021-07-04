#!/bin/bash

#!!! adjust the time quota !!!

#SBATCH --time=22:00:00
#SBATCH --partition=gelifes
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --mem=10GB
#SBATCH --cpus-per-task=16

module load GCC --latest

datadir="/data/$(whoami)"
./bin/evorole --force param=2021-07-05-pre_0.001_male2_5e7.json rep=10 outdir=$datadir/2021-07-05-pre_0.001_male2_5e7
