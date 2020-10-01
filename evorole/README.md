# evorole

## Quick start
The following commands will create the binary in evorole/bin.

### Window
```
Download Visual Studio 2019
Open 'evorole.sln' from the folder of 'evorole' using Visual Studio and run the code
```
### Linux (debian/apt based)
```
sudo apt install cmake
sudo apt install libtbb-dev
```
### Peregrine cluster
```
module load GCC --latest
module load CMake --latest
```
### Both platforms (Linux and Peregrine cluster)
```
git clone https://github.com/xiaoyanlong/evosexrole.git
cd ~/evosexrole/evorole
mkdir build & cd build
cmake ..
make install
cd ~/evosexrole/evorole/bin
./evorole --help
./evorole param=./param.json outdir='your output directory'
```
## submit job on peregrine
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
The example runs in roughly 10min (20 simulations). --time must be long enough for the simulations to finish.
To be sure, oversubscribe your estimated runtime.

## Simulation parameters
The example runs are the 1) baseline scenario (random mating, only care levels of sexes can evolve) and 2) the introduction of weak parental synergy (σ = 0.05).
 
```
Simulation parameters can be changed in `param.json`.
For instance, run a simulation including sexual selection:
m_p: 0.005, m_tau:0.005, random_mating: false
```

## Simulation output
Simulation output (average values of female care, male care, female preferences, male ornaments...) is written in csv files. 
