//Define Guard
#define _GNU_SOURCE

//Llibreries del sistema
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>

//Tipus propis
typedef struct
{
	char *nom;
	char *ip;
	int port;
	int pid;
	int fd;
	char *servidor;
	int fdMsg;
} Usuaris;

char *read_untilEnter(int fd, char character, int i);
char *read_until(int fd, char character);
