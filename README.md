# CJson

A tiny library

## Build and Run

requires `make`, `cc` and assumes Linux based system

### Static Library

```bash
mkdir build
make clean
make static
# outputs ./build/cjson.a
```

### Cli


```bash
mkdir build
make clean
make
cat data/minimal.json | ./build/cjson
```
