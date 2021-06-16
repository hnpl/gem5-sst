# gem5/SST integration (2021)



## Compile gem5
```sh
scons3 build/X86/gem5.opt CPU_MODELS=AtomicSimpleCPU,MinorCPU,O3CPU,TimingSimpleCPU -j256 --without-tcmalloc
```

## Compile libgem5_opt.so (gem5 library)
```sh
scons3 build/X86/libgem5_opt.so -j 256 --without-tcmalloc
```

## Add libgem5_opt.so to LD_LIBRARY_PATH
```sh
echo "export LD_LIBRARY_PATH=absoblute/path/to/gem5/X86/build:$LD_LIBRARY_PATH" > ~/.bashrc
```

## Compile gem5/SST interface
This step must be done after gem5 is compiled.
```sh
make -C ext/sst2
```

## Download SST
Download SST source code from here <http://sst-simulator.org/SSTPages/SSTMainDownloads/>.
```sh
wget https://github.com/sstsimulator/sst-core/releases/download/v10.1.0_Final/sstcore-10.1.0.tar.gz
tar xf sstcore-10.1.0.tar.gz
```

## Compile SST Core
Original documentation is here <http://sst-simulator.org/SSTPages/SSTBuildAndInstall10dot1dot0SeriesDetailedBuildInstructions/>.

Setting up the environment variables,
```sh
export SST_CORE_HOME=$HOME/sst             # where to install SST
export SST_CORE_ROOT=$HOME/sstcore-10.1.0  # the location of SST source code
```

Configure SST-Core,
```sh
./configure --prefix=$SST_CORE_HOME --with-python=/usr/bin/python3-config
            --disable-mpi # to compile without MPI
```

Compile SST,
```sh
make all -j8
make install
```

Update `PATH`,
```sh
export PATH=$SST_CORE_HOME/bin:$PATH
```

Testing the installation,
```sh
which sst
```




## Download  SST Elements
Download SST source code from here <http://sst-simulator.org/SSTPages/SSTMainDownloads/>.
```sh
wget https://github.com/sstsimulator/sst-elements/releases/download/v10.1.0_Final/sstelements-10.1.0.tar.gz
tar xf sstelements-10.1.0.tar.gz
```
# Compile SST Elements
Original documentation is here <http://sst-simulator.org/SSTPages/SSTBuildAndInstall10dot1dot0SeriesDetailedBuildInstructions/>.

Setting up the environment variables,
```sh
export SST_ELEMENTS_HOME=$HOME/sst/elements                 # where to install SST
export SST_ELEMENTS_ROOT=$HOME/sst-elements-library-10.1.0  # the location of SST source code
```

Configure SST-Elements,
```sh
./configure --prefix=$SST_ELEMENTS_HOME --with-sst-core=$SST_CORE_HOME --with-python=/usr/bin/python3-config
```

Compile SST-Element,
```sh
make all -j8
make install
```

Update `PATH`,
```sh
export PATH=$SST_ELEMENTS_HOME/bin:$PATH
```

Testing the installation,
```sh
sst $SST_ELEMENTS_ROOT/src/sst/elements/simpleElementExample/tests/test_simpleRNGComponent_mersenne.py
```

Adding PKG_CONFIG_PATH to .bashrc,
```
export PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$SST_CORE_HOME/lib/pkgconfig/
```

Adding sst to LD_LIBRARY_PATH in .bashrc,
```
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$SST_CORE_HOME
```
