#!/bin/bash

dir=$(pwd)
printf "The working directory is:" $dir


printf "\nCounting stars using connected component labelling\n\n"
./output_count ${PWD%%/}/counting_stars/images/stars.raw ${PWD%%/}/counting_stars/output.raw 480 640 0  
printf "\nCounting stars using morhological processes\n\n"
./output_count ${PWD%%/}/counting_stars/images/stars.raw ${PWD%%/}/counting_stars/output.raw 480 640 1

printf "\n------- Defect Detection in a Geartooth ---------"
./output_defect ${PWD%%/}/defect_detection/images/Geartooth.raw ${PWD%%/}/defect_detection/output.raw 250 250 


./output_shrink ${PWD%%/}/morph_process/images/maze.raw ${PWD%%/}/morph_process/output.raw 558 558  

printf "\n------- PCB Analysis --------\n\n"
./output_pcb ${PWD%%/}/pcb_analysis/images/PCB.raw ${PWD%%/}/pcb_analysis/output.raw 239 372

make clean

echo "Check the output in respective project folder"

  