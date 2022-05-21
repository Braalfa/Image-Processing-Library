g++ main.cpp Base/traditional.cpp Base/morphological.cpp Base/convolutional.cpp  -o main `pkg-config --cflags --libs opencv4`
./main