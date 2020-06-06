all: output_classification output_segmentation output_advance_segmentation

output_classification: texture_classification/main_texture_classification.o texture_classification/texture_classification.o 
	g++ -o texture_classification/output_classification texture_classification/main_texture_classification.o texture_classification/texture_classification.o -lm 

main_texture_classification.o: texture_classification/main_texture_classification.cpp
	g++ -c texture_classification/main_texture_classification.cpp -lm 

texture_classification.o: texture_classification/texture_classification.cpp texture_classification/texture_classification.h 
	g++ -c texture_classification/texture_classification.cpp 



output_segmentation: texture_segmentation/main_texture_segmentation.o texture_segmentation/texture_segmentation.o 
	g++ -o texture_segmentation/output_segmentation texture_segmentation/main_texture_segmentation.o texture_segmentation/texture_segmentation.o -lm

main_texture_segmentation.o: texture_segmentation/main_texture_segmentation.cpp 
	g++ -c texture_segmentation/main_texture_segmentation.cpp -lm 

texture_segmentation.o: texture_segmentation/texture_segmentation.cpp texture_segmentation/texture_segmentation.h 
	g++ -c texture_segmentation/texture_segmentation.cpp 



output_advance_segmentation: advanced_texture_segmentation/main_advance_texture_segmentation.o advanced_texture_segmentation/advance_texture_segmentation.o 
	g++ -o advanced_texture_segmentation/output_advance_segmentation advanced_texture_segmentation/main_advance_texture_segmentation.o advanced_texture_segmentation/advance_texture_segmentation.o -lm 

main_advance_texture_segmentation.o: advanced_texture_segmentation/main_advance_texture_segmentation.cpp 
	g++ -c advanced_texture_segmentation/main_advance_texture_segmentation.cpp -lm 

advance_texture_segmentation.o: advanced_texture_segmentation/advance_texture_segmentation.cpp advanced_texture_segmentation/advance_texture_segmentation.h 
	g++ -c advanced_texture_segmentation/advance_texture_segmentation.cpp 

clean:
	rm texture_classification/*.o texture_classification/output_classification
	rm texture_segmentation/*.o texture_segmentation/output_segmentation
	rm advanced_texture_segmentation/*.o advanced_texture_segmentation/output_advance_segmentation