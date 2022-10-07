all: clean iluvatarSon

iluvatarSon.o: iluvatarSon.c iluvatarSon.h
	gcc -c iluvatarSon.c -Wall -Wextra

practica: iluvatarSon.o
	gcc iluvatarSon.o -o practica -Wall -Wextra

clean:
	rm -f *.o iluvatarSon
