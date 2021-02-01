# gem5/SST integration (2021)

## Compile gem5
```sh
scons3 build/X86/gem5.opt CPU_MODELS=AtomicSimpleCPU,MinorCPU,O3CPU,TimingSimpleCPU -j256
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
```sh
make -C ext/sst2
```

