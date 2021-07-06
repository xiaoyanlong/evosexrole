#!/bin/bash

#!!! adjust the time quota !!!

#SBATCH --time=22:00:00
#SBATCH --partition=gelifes
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --mem=10GB
#SBATCH --cpus-per-task=24

module load GCC --latest

datadir="/data/$(whoami)"
./bin/evorole --force param=2021-07-05-pre_0.001_male3_1e8.json rep=5 outdir=$datadir/2021-07-05-pre_0.001_male3_1e8
