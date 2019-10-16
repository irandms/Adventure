all: adventure

buildrooms: buildrooms.c
	gcc -o buildrooms buildrooms.c

adventure: adventure.c
	gcc -o -lpthread adventure adventure.c

clean: 
	-rm adventure.o buildrooms.o
