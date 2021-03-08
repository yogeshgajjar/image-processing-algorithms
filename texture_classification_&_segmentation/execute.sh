#!/bin/bash
dir=$(pwd)
printf "The working directory is:" $dir

printf "\n ***** IMPORTANT STEP TO FOLLOW BEFORE EXECUTING THE CODE ****** \n"
printf "\n [INFO] Copy all the images from the test_image and train_image folder and paste it in the folder name [texture_classification].\n"

while true; do
    read -p "[INFO] Done? " yn
    case $yn in
        [Yy]* ) cd texture_classification;./output_classification 128 128 train_read; break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no. ";;
    esac
done


printf "\n[INFO] Executing next mode... \n"
cd ${PWD%%/}
./output_classification 128 128 test_read 

while true; do
    read -p "[INFO] Done? " yn
    case $yn in
        [Yy]* ) ./output_classification 128 128 test_PCA; break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no. ";;
    esac
done
 
rm -rf *.raw
rm -rf *.txt

cd ..

printf "\n"
while true; do
    read -p "[INFO] Want to execute texture segmentation? " yn
    case $yn in
        [Yy]* ) cd texture_segmentation;./output_segmentation ${PWD%%/}/images/comp.raw ${PWD%%/}/output.raw 450 600 15; break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no. ";;
    esac
done

cd .. 

printf "\n"
while true; do
    read -p "[INFO] Want to execute advanced texture segmentation? " yn
    case $yn in
        [Yy]* ) cd advanced_texture_segmentation;./output_advance_segmentation ${PWD%%/}/images/comp.raw ${PWD%%/}/output.raw 450 600 15 write; break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no. ";;
    esac
done

while true; do
    read -p "[INFO] Done? " yn
    case $yn in
        [Yy]* ) ./output_advance_segmentation ${PWD%%/}/images/comp.raw ${PWD%%/}/output.raw 450 600 15 read; break;;
        [Nn]* ) exit;;
        * ) echo "Please answer yes or no. ";;
    esac
done

rm -rf *.txt

cd ..
make clean 

