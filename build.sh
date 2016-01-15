#!/bin/bash

#Compile sources
g++ -fPIC src/ptg.cpp src/cPTG.cpp -Iinclude/ -c

#Packing library
ar rvs libptg.a ptg.o cPTG.o

#Removing files
rm ptg.o cPTG.o