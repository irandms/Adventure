all: adventure

buildrooms: buildrooms.c
	gcc -o buildrooms buildrooms.c

adventure: adventure.c
	gcc -o adventure adventure.c -lpthread 

test_buildrooms: buildrooms_test.c buildrooms.c
	sed -e "s/main/_main/g" buildrooms.c > buildrooms_core.c
	gcc -o test_buildrooms buildrooms_core.c buildrooms_test.c
	rm -f buildrooms_core.c

clean: 
	rm -f adventure.o buildrooms.o adventure buildrooms test_buildrooms
