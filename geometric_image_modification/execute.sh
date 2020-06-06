#!/bin/bash

dir=$(pwd)
echo "The working directory is:" $dir

./output_warping ${PWD%%/}/image_warping/images/hedwig.raw ${PWD%%/}/image_warping/images/bb8.raw ${PWD%%/}/image_warping/images/raccoon.raw ${PWD%%/}/image_warping/warped_hedwig.raw ${PWD%%/}/image_warping/warped_bb8.raw ${PWD%%/}/image_warping/warped_raccoon.raw ${PWD%%/}/image_warping/ori_hedwig.raw ${PWD%%/}/image_warping/ori_bb8.raw ${PWD%%/}/image_warping/ori_raccoon.raw 512 512 3
./output_stitching ${PWD%%/}/image_stitching/images/left.raw ${PWD%%/}/image_stitching/images/middle.raw ${PWD%%/}/image_stitching/images/right.raw ${PWD%%/}/image_stitching/stitch.raw 720 480 3 
make clean

echo "Check the output in respective project folder"

