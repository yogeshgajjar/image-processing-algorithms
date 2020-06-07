#!/bin/bash

dir=$(pwd)
echo "The working directory is:" $dir

./output_sobel ${PWD%%/}/images/Dogs.raw ${PWD%%/}/dog_gra_x.raw ${PWD%%/}/dog_gra_y.raw ${PWD%%/}/dog_gra_map.raw ${PWD%%/}/dog_normalized.raw 3 0.9
# ./output_canny ${PWD%%/}/images/Dogs.raw 20 70  

make clean 

echo "Check the output in respective project folder"