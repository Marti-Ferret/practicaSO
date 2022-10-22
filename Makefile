all:	clean	iluvatarSon

iluvatarSon.o:	iluvatarSon.c	iluvatarSon.h
	gcc -c iluvatarSon.c -Wall -Wextra

iluvatarSon:	iluvatarSon.o
	gcc	iluvatarSon.o -o iluvatarSon -Wall -Wextra

clean:
	rm -f *.o iluvatarSon

