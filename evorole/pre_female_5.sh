#!/bin/bash

#!!! adjust the time quota !!!

#SBATCH --time=05:15:00
#SBATCH --partition=gelifes
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --mem=10GB
#SBATCH --cpus-per-task=16

module load GCC --latest

datadir="/data/$(whoami)"
./bin/evorole --force param=pre_female_5.json rep=20 outdir=$datadir/2021-06-23-pre_female_5
