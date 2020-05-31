#!/bin/bash

dir=$(pwd)
echo "The working directory is:" $dir

./output_dithering ${PWD%%/}/dithering/images/LightHouse.raw ${PWD%%/}/dithering/fixed.raw ${PWD%%/}/dithering/random.raw ${PWD%%/}/dithering/I2.raw 500 750 0 
./output_errorDiffusion ${PWD%%/}/error_diffusion/images/LightHouse.raw ${PWD%%/}/error_diffusion/FS.raw ${PWD%%/}/error_diffusion/JJN.raw ${PWD%%/}/error_diffusion/S.raw 500 750 
./output_halftoning ${PWD%%/}/color_halftoning/images/Rose.raw ${PWD%%/}/color_halftoning/output.raw 480 640 3 
make clean

echo "Check the output in respective project folder"