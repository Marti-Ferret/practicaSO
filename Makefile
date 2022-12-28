all: clean iluvatarSon arda

reads.o: reads.c reads.h
	gcc -c reads.c -Wall -Wextra

iluvatarSon.o: iluvatarSon.c iluvatarSon.h reads.h
	gcc -c iluvatarSon.c -Wall -Wextra

arda.o: arda.c arda.h reads.h
	gcc -c arda.c -Wall -Wextra -lpthread

iluvatarSon: iluvatarSon.o	reads.o
	gcc iluvatarSon.o reads.o -o iluvatarSon -Wall -Wextra

arda: arda.o	reads.o
	gcc arda.o reads.o -o arda -Wall -Wextra -lpthread

servidor: 
	valgrind --track-origins=yes --leak-check=full --track-fds=yes --show-reachable=yes -s arda configArda.txt

clientMata:
	valgrind --track-origins=yes --leak-check=full --track-fds=yes --show-reachable=yes -s iluvatarSon configMata.dat

clientMont:
	valgrind --track-origins=yes --leak-check=full --track-fds=yes --show-reachable=yes -s iluvatarSon configMont.dat

clean:
	rm -f *.o iluvatarSon arda
