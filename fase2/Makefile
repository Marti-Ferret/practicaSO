all: clean iluvatarSon arda

reads.o: reads.c reads.h
	gcc -c reads.c -Wall -Wextra

trames.o: trames.c	trames.h
	gcc -c trames.c -Wall -Wextra

iluvatarSon.o: iluvatarSon.c iluvatarSon.h reads.h	trames.h
	gcc -c iluvatarSon.c -Wall -Wextra -lpthread

arda.o: arda.c arda.h reads.h trames.h
	gcc -c arda.c -Wall -Wextra -lpthread

iluvatarSon: iluvatarSon.o	reads.o	trames.o
	gcc iluvatarSon.o reads.o trames.o -o iluvatarSon -Wall -Wextra -lpthread

arda: arda.o	reads.o	trames.o
	gcc arda.o reads.o	trames.o -o arda -Wall -Wextra -lpthread

servidor: 
	valgrind --track-origins=yes --leak-check=full --track-fds=yes --show-reachable=yes -s arda configArda.txt

clientMata:
	valgrind --track-origins=yes --leak-check=full --track-fds=yes --show-reachable=yes -s iluvatarSon configMata.dat -lpthread

clientMont:
	valgrind --track-origins=yes --leak-check=full --track-fds=yes --show-reachable=yes -s iluvatarSon configMont.dat -lpthread

clean:
	rm -f *.o iluvatarSon arda
