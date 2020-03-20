build: MiniOS.o functii_coada.o functii_stiva.o run.o waiting.o finished.o functions.o
	gcc $^ -o tema2
MiniOS.o: MiniOS.c
	gcc $^ -c -g -Wall
functii_coada.o: functii_coada.c
	gcc $^ -c -g -Wall
functii_stiva.o: functii_stiva.c
	gcc $^ -c -g -Wall
run.o: 	run.c
	gcc $^ -c -g -Wall
waiting.o: waiting.c
	gcc $^ -c -g -Wall
finished.o: finished.c
	gcc $^ -c -g -Wall
functions.o: functions.c
	gcc $^ -c -g -Wall
clean: 
	rm -rf *.o
	rm tema2
