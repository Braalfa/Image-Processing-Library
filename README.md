# Image Processing Library

### Prerequisites

- Linux Operating System
- OpenMP Library installed
- OpenCV Library installed
- Cpp compiler

### General information

- The folders Base and Optimized contain several image filters. The Base folder contains non-optimized filters, and the Optimized folder contains the same filters optimized with OpenMP.
- The files base-main.cpp and optimized-main.cpp, show how to use each of the main filters available, for each of the libraries (base and optimized).
- The folder InputImages contains the input images, and the output images are saved on the folder OutputImages.
- The available filters are:
   1. Median filter
   2. Average filter
   3. Border enhancement filter
   4. Contrast enhancemet filter
  
### Usage

1. To compile the example main files, run the following commands:

```console
./compile-base.sh
./compile-optimized.sh
```

2. To run the files, execute:

```console
time ./base-main
time ./optimized-main
```

