# CJson

A tiny library

## Build and Run

requires `make`, `cc` and assumes Linux based system

### Static Library

```
mkdir build
make clean
make static
```

### Cli


```
mkdir build
make clean
make
cat data/minimal.json | ./build/cjson
```
