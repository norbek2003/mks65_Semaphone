all: error.o control.o write.o
	gcc -o write write.o error.o
	gcc -o control control.o error.o
write.o: write.c semaphone.h
	gcc -c write.c
control.o: control.c semaphone.h
	gcc -c control.c
error.o: error.c semaphone.h
	gcc -c error.c
clean:
	rm -f *.o
	rm -f *~
