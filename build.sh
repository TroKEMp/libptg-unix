#!/bin/bash

#Compile sources
g++ -std=c++11 -fPIC src/ptg.cpp -Iinclude/ -c

#Packing library
ar rvs libptg.a ptg.o

#Removing files
rm ptg.o