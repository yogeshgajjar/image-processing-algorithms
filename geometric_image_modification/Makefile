all: output_warping output_stiching 

output_warping: main_image_warping.o image_warping.o 
	g++ -o output_warping main_image_warping.o image_warping.o -lm 

output_stiching: main_image_stiching.o image_stiching.o 
	g++ -o output_stiching main_image_stiching.o image_stiching.o -lm 

main_image_warping.o: image_warping/main_image_warping.cpp 
	g++ -c image_warping/main_image_warping.cpp -lm

image_warping.o: image_warping/image_warping.cpp image_warping/image_warping.h #*.h
	g++ -c image_warping/image_warping.cpp 

main_image_stiching.o: image_stiching/main_image_stiching.cpp 
	g++ -c image_stiching/main_image_stiching.cpp -lm 

image_stiching.o: image_stiching/image_stiching.cpp image_stiching/image_stiching.h #*h 
	g++ -c image_stiching/image_stiching.cpp 

clean:
	rm *.o output_warping output_stiching
