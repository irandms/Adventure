all: adventure

buildrooms: buildrooms.c
	gcc -o buildrooms buildrooms.c

adventure: adventure.c
	gcc -o adventure adventure.c -lpthread 

clean: 
	-rm adventure.o buildrooms.o
