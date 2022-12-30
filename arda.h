//Define Guard
#define _GNU_SOURCE

//LLibreries del sistema
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

//Define
#define escriure(x) write(1, x, strlen(x))


//Tipus propis
typedef struct
{
	char *ip;
	int port;
	char *directori;

} Config;
