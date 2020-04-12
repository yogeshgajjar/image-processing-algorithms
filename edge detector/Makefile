all: output_canny output_sobel

output_sobel: sobel_edge.o 
	g++ sobel_edge.o -o output_sobel
	
output_canny: canny_edge.o 
	g++ canny_edge.o -o output_canny `pkg-config --cflags --libs opencv`

canny_edge.o: canny_edge.cpp	
	g++ -c canny_edge.cpp

sobel_edge.o: sobel_edge.cpp 
	g++ -c sobel_edge.cpp 

clean:
	rm *.o output_canny
	rm *.o output_sobel