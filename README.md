# Individual variation in parental care drives divergence of sex roles

**Xiaoyan Long and Franz J. Weissing**

email: x.long@rug.nl and f.j.weissing@rug.nl

---

## Contents

- [Overview](#overview)
- [Software Requirements](#software-requirements)
- [Demo](#demo)
- [Results](#results)
- [License](./LICENSE)
- [Issues](#issues)
- [Citation](#citation)



# Overview

We built an individual-based evolutionary simulation to study the evolution of sex roles.The `evorole` folder contains all codes which are needed for simulations. In addition, analytical model has also been investigated to complement our simulation study. The `Mathematical analysis` folder contains one pdf file indicating the details of mathematical analysis and one Mathematica notebook showing how we get the evolutionary trajectories and pairwise invasibility plots.



# Software Requirements

## Individual-based simulations

To run the individual-based simulations, we advise Windows users to download `Visual Studio Community 2019` and Mac users to download free compiler `XCode`.

## Analytical models

To run the mathematical models, users should download `Wolfram Mathematica 12`.


# Demo

## Individual-based simulations

### Windows

```
1. Download our `Code`;

2. Open 'evorole.sln' from the folder of 'evorole' directly and run the simulations in Visual Studio 2019 (in XCode if you use Mac).
```
It takes around 5min to run 2 simulations (time steps = 5,000,000) 
on the "normal" laptop (RAM: 16 GB; CPU: 4 cores, 2.1 GHz/core).

### Linux (debian/apt based)

```
sudo apt install cmake
sudo apt install libtbb-dev

git clone https://github.com/xiaoyanlong/evosexrole.git
cd ~/evosexrole/evorole
mkdir build & cd build
cmake ..
make install
cd ~/evosexrole/evorole/bin
./evorole --help
./evorole param=../param.json outdir='your output directory'
```
### Peregrine cluster

Since we have to run thousands of simulations, peregrine cluster was used. If you could also use peregrine cluster, you can run the simulations as follow.

```
module load GCC --latest
module load CMake --latest

git clone https://github.com/xiaoyanlong/evosexrole.git
cd ~/evosexrole/evorole
mkdir build & cd build
cmake ..
make install
cd ~/evosexrole/evorole/bin
./evorole --help
./evorole param=../param.json outdir='your output directory'
```

If you prefer to submit job on peregrine, you can use following command (`submit_example.sh` is already offered in the folder of `evorole`):

```
cd ~/evosexrole/evorole
# edit submit_example.sh
nano submit_example.sh
sbatch submit_example.sh
```

You need to adjust time quota! This is given as

```
#SBATCH --time=hh:mm:ss
```
The example runs in roughly 10min (20 simulations in total) in peregrine cluster. --time must be long enough for the simulations to finish. To be sure, oversubscribe your estimated runtime.

# Results

## Parameters

Simulation parameters can be changed in `param.json` (in the folder of `evorole`).

We offered two parameter settings：

1. The baseline scenario where mating is randome and only care strategies can evolve.

2. The scenario considering weak parental synergy (σ = 0.05).

If you want to implement other scenarios，such as sexual selection, please remember to turn off random mating and allow female preferences and male ornaments to evolve (m_p: 0.005, m_tau:0.005, random_mating: false). You can use this code script to produce your own simulation data with the parameters you are interested in.

## Outputs

Three csv files will be produced: 

1. `all_traits`: Average values of female care, male care, female preferences, male ornaments in the population, population size at each state (pre-mating state, mating state, caring state, juvenile state).

2. `deaths`: Genetic information of all dead individuals.

3. `offspring`:  Genetic information of all parents and their offspring.


# Issues

If you have any questions about the code or need any help when run the simulations, please contact us. And if you would like to contribute to this project, please also let us know.

# Citation

If you are going use this code, please cite our paper.
