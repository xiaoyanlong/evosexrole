# evorole

## Quick start
The following commands will create the binary in evorole/bin.
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
### Both platforms
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


