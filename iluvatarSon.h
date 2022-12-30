//Define Guard
#define _GNU_SOURCE

//Llibreries del sistema
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/wait.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <netdb.h>
#include <arpa/inet.h>

//Define
#define escriure(x) write(1, x, strlen(x))


//Tipus propis
typedef struct
{
	char *nom;
	char *directori;
	char *ipS;
	int portS;
	char *ipC;
	int portC;

} Config;
