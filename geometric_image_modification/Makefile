all: output_warping output_stitching 

output_warping: main_image_warping.o image_warping.o 
	g++ -o output_warping main_image_warping.o image_warping.o -lm 

main_image_warping.o: image_warping/main_image_warping.cpp 
	g++ -c image_warping/main_image_warping.cpp -lm

image_warping.o: image_warping/image_warping.cpp image_warping/image_warping.h #*.h
	g++ -c image_warping/image_warping.cpp 


output_stitching: main_image_stitching.o image_stitching.o 
	g++ -o output_stitching main_image_stitching.o image_stitching.o -lm 

main_image_stitching.o: image_stitching/main_image_stitching.cpp 
	g++ -c image_stitching/main_image_stitching.cpp -lm 

image_stitching.o: image_stitching/image_stitching.cpp image_stitching/image_stitching.h #*h 
	g++ -c image_stitching/image_stitching.cpp 

clean:
	rm *.o output_warping output_stitching
