# all: output_canny output_sobel
all: output_dithering output_errorDiffusion output_halftoning

output_dithering: main_dithering.o dithering.o 
	g++ -o output_dithering main.o dithering.o -lm 

output_errorDiffusion: main_error_diffusion.o error_diffusion.o 
	g++ -o output_errorDiffusion main_error_diffusion.o error_diffusion.o -lm 

output_halftoning: main_color_halftoning.o color_halftoning.o
	g++ -o output_halftoning main_color_halftoning.o color_halftoning.o -lm 

main_dithering.o: dithering/main.cpp 
	g++ -c dithering/main.cpp -lm

dithering.o: dithering/dithering.cpp dithering/dithering.h #*.h
	g++ -c dithering/dithering.cpp 

main_error_diffusion.o: error_diffusion/main_error_diffusion.cpp 
	g++ -c error_diffusion/main_error_diffusion.cpp -lm 

error_diffusion.o: error_diffusion/error_diffusion.cpp error_diffusion/error_diffusion.h #*h 
	g++ -c error_diffusion/error_diffusion.cpp 

main_color_halftoning.o: color_halftoning/main_color_halftoning.cpp 
	g++ -c color_halftoning/main_color_halftoning.cpp -lm 

color_halftoning.o: color_halftoning/color_halftoning.cpp color_halftoning/color_halftoning.h 
	g++ -c color_halftoning/color_halftoning.cpp 

clean:
	rm *.o output_dithering output_errorDiffusion output_halftoning

# output: dithering.h dithering.cpp main_dithering.cpp
#     g++ -o dithering main_dithering.cpp dithering.cpp

# clean:
#     rm output