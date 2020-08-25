#!/bin/bash

#!!! adjust the time quota !!!

#SBATCH --time=23:30:00
#SBATCH --partition=gelifes
#SBATCH --ntasks=1
#SBATCH --nodes=1
#SBATCH --mem=1000
#SBATCH --cpus-per-task=16

module load GCC --latest

datadir="/data/$(whoami)/ani_9"
./bin/evorole --force param=param.json rep=20 outdir=$datadir
