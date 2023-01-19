# Waffle++ code generator

The full documentation is to be written yet, but there is a prospect (in Russian): https://habr.com/ru/post/710744/

# How to build

(Sorry, without Dockerfile yet...)
```
# install libclang
sudo apt-get install libclang-dev

# build and test
git clone https://github.com/Izaron/WafflePlusPlus.git
cd WafflePlusPlus && mkdir build && cd build
cmake ../src/
cmake --build . -j10
ctest
```
