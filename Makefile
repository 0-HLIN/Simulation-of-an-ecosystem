

all: tests_ecosys ecosys

ecosys.o: ecosys.c ecosys.h
	gcc -c ecosys.c

main_tests.o: main_tests.c ecosys.h
	gcc -c main_tests.c

main_ecosys.o: main_ecosys.c ecosys.h
	gcc -c main_ecosys.c


tests_ecosys: main_tests.o ecosys.o
	gcc -o tests_ecosys main_tests.o ecosys.o

ecosys: main_ecosys.o ecosys.o
	gcc -o ecosys main_ecosys.o ecosys.o

clean:
	rm -f *.o tests_ecosys ecosys
	
