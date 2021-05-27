#!/bin/bash

#!!! adjust the time quota !!!

#SBATCH --time=00:30:00
#SBATCH --partition=gelifes
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --mem=1G
#SBATCH --cpus-per-task=16

module load GCC --latest

datadir="/data/$(whoami)"
./bin/evorole --force param=param_check_pre.json rep=2 outdir=$datadir/2021-05-27_param_check_pre
