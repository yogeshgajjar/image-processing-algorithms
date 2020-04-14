# all: output_canny output_sobel
all: output_dithering output_errorDiffusion

output_dithering: main_dithering.o dithering.o 
	g++ -o output_dithering main_dithering.o dithering.o -lm 

output_errorDiffusion: main_error_diffusion.o error_diffusion.o 
	g++ -o output_errorDiffusion main_error_diffusion.o error_diffusion.o -lm 

main_dithering.o: main_dithering.cpp 
	g++ -c main_dithering.cpp -lm

dithering.o: dithering.cpp dithering.h *.h
	g++ -c dithering.cpp 

main_error_diffusion.o: main_error_diffusion.cpp 
	g++ -c main_error_diffusion.cpp -lm 

error_diffusion.o: error_diffusion.cpp error_diffusion.h *h 
	g++ -c error_diffusion.cpp 

clean:
	rm -f *.o output_dithering output_errorDiffusion

# output: dithering.h dithering.cpp main_dithering.cpp
#     g++ -o dithering main_dithering.cpp dithering.cpp

# clean:
#     rm output