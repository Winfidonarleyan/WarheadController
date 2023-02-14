# WarheadController

## Build requirements

1. Ubuntu

```sh
sudo apt-get install git clang cmake make gcc g++
```

2. Manjaro Linux

```sh
sudo pacman -S git clang cmake make
```

## Build process

```sh
git clone https://github.com/Winfidonarleyan/WarheadController
cd WarheadController
mkdir build
cd build
cmake ../ -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_INSTALL_PREFIX=../../wc
make -j 4 && make install
cd ../../wc
```

## Run app
1. Move to `.../wc/bin`
2. `./WarheadController`
