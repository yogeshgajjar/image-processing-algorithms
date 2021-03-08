all: output_count output_defect output_shrink output_pcb 

output_count: main_counting_stars.o counting_stars.o
	g++ -o output_count main_counting_stars.o counting_stars.o -lm

output_defect: main_defect_detection.o defect_detection.o 
	g++ -o output_defect main_defect_detection.o defect_detection.o -lm 

output_shrink: main_shrinking.o shrinking.o 
	g++ -o output_shrink main_shrinking.o shrinking.o -lm 

output_pcb: main_pcb_analysis.o pcb_analysis.o 
	g++ -o output_pcb main_pcb_analysis.o pcb_analysis.o -lm 

main_counting_stars.o: counting_stars/main_counting_stars.cpp 
	g++ -c counting_stars/main_counting_stars.cpp -lm

main_defect_detection.o: defect_detection/main_defect_detection.cpp 
	g++ -c defect_detection/main_defect_detection.cpp -lm

main_shrinking.o: morph_process/main_shrinking.cpp 
	g++ -c morph_process/main_shrinking.cpp -lm 

main_pcb_analysis.o: pcb_analysis/main_pcb_analysis.cpp 
	g++ -c pcb_analysis/main_pcb_analysis.cpp -lm 

counting_stars.o: counting_stars/counting_stars.cpp counting_stars/counting_stars.h 
	g++ -c counting_stars/counting_stars.cpp 

defect_detection.o: defect_detection/defect_detection.cpp defect_detection/defect_detection.h 
	g++ -c defect_detection/defect_detection.cpp 

shrinking.o: morph_process/shrinking.cpp morph_process/shrinking.h 
	g++ -c morph_process/shrinking.cpp 

pcb_analysis.o: pcb_analysis/pcb_analysis.cpp pcb_analysis/pcb_analysis.h 
	g++ -c pcb_analysis/pcb_analysis.cpp 

clean:
	rm *.o output_count output_defect output_shrink output_pcb